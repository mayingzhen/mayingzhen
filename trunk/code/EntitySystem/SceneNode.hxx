#include "SceneNode.h"

namespace ma
{
	IMPL_OBJECT(SceneNode,Object)

	SceneNode::SceneNode(GameObject* pGameObj,Scene* pSene)
	{
		m_pScene = pSene;	
		m_pParentNode = NULL;
		m_pGameObject = pGameObj;

		MatrixIdentity(&m_matWorld);
		TransformSetIdentity(&m_tsfWS);
		m_bmatWSDirty = true;
	}

	SceneNode::~SceneNode()
	{

	}

	bool SceneNode::TravelScene(SceneVisiter* pVisiter)
	{
		bool bTraveling = pVisiter->VisiteSceneNodeBegin(this);

		if (bTraveling)
		{
			if (NULL != m_pGameObject)
			{
				bTraveling = m_pGameObject->TravelScene(pVisiter);
				if(!bTraveling)
				{
					//break;
				}
			}
		}

		if (bTraveling)
		{
			for (UINT nCnt = 0; nCnt < m_arrChildNode.size(); ++nCnt)
			{
				bTraveling = m_arrChildNode[nCnt]->TravelScene(pVisiter);
				if (!bTraveling)
				{
					break;
				}
			}
		}

		pVisiter->VisiteSceneNodeEnd(this);

		return bTraveling;
	}


	void SceneNode::SetTransform(const NodeTransform& tsf)
	{
		m_tsfWS = tsf;
	}

	const NodeTransform& SceneNode::GetTransform()
	{
		return m_tsfWS;
	}

	Vector3	SceneNode::GetForward()
	{
		return GetWorldMatrix().GetRow(2);
	}

	Vector3	SceneNode::GetRight()
	{
		return GetWorldMatrix().GetRow(0);
	}
	
	Vector3	SceneNode::GetUp()
	{
		return GetWorldMatrix().GetRow(1);
	}

	void  SceneNode::Forward(float x)
	{
		m_tsfWS.m_vPos += GetForward() * x;
	}

	void  SceneNode::Up(float x)
	{
		m_tsfWS.m_vPos += GetUp() * x;	
	}

	void  SceneNode::Right(float x)
	{
		m_tsfWS.m_vPos += GetRight() * x;
	}


	void SceneNode::Translate(const Vector3& vTrans)
	{
		m_tsfWS.m_vPos += vTrans;
	}

	void SceneNode::Rotate(float xDegree,float yDegree,float zDegree)
	{
		Quaternion qRot;
		EulerAngleXYZ qEuler(ToRadian(xDegree), ToRadian(yDegree), ToRadian(xDegree));
		QuaternionFromEulerAngleXYZ(&qRot,&qEuler);
		Rotate(qRot);
	}

	const Matrix4x4& SceneNode::GetWorldMatrix()
	{
		if (m_bmatWSDirty)
		{
			UpdateMatWorld();
		}
		return m_matWorld;
	}

	void SceneNode::UpdateMatWorld()
	{
		MatrixFromTransform(&m_matWorld,&GetTransform());
		//m_bmatWSDirty = false;
	}

	void SceneNode::WorldToLocal(NodeTransform* pOutLocal,const NodeTransform* pWorld)
	{
		NodeTransform tsfWorld = GetTransform();
		NodeTransform tsfWorldInv;
		TransformInverse(&tsfWorldInv,&tsfWorld);
		TransformMul(pOutLocal,pWorld,&tsfWorldInv);
	}

	void SceneNode::Rotate(const Quaternion& qRot)
	{
		m_tsfWS.m_qRot = qRot * m_tsfWS.m_qRot;
	}

	void SceneNode::RotateYAxisLS(float fDegree)
	{
		Quaternion qRot;
		const Vector3 vY(0.0f,1.0f,0.0f);
		QuaternionRotationAxis(&qRot,&vY,ToRadian(fDegree));
		Rotate(qRot);

	}

	void SceneNode::RotateZAxisLS(float fDegree)
	{
		Quaternion qRot;
		const Vector3 vZ(0.0f,0.0f,1.0f);
		QuaternionRotationAxis(&qRot,&vZ,ToRadian(fDegree));
		Rotate(qRot);
	}

	void SceneNode::RotateXAxisLS(float fDegree)
	{
		Quaternion qRot;
		const Vector3 vZ(1.0f,0.0f,0.0f);
		QuaternionRotationAxis(&qRot,&vZ,ToRadian(fDegree));
		Rotate(qRot);
	}

	void SceneNode::NotifySetParent(SceneNode* pParentNode)
	{
		m_pParentNode = pParentNode;
	}

	void SceneNode::AddChildNode(SceneNode* pChildNode)
	{
		if (pChildNode == NULL)
			return;

		std::vector<SceneNode*>::iterator it = find(m_arrChildNode.begin(),m_arrChildNode.end(),pChildNode);
		if (it != m_arrChildNode.end())
			return;

		m_arrChildNode.push_back(pChildNode);

		pChildNode->NotifySetParent(this);
	}

	void SceneNode::RemoveChildNode(SceneNode* pChildNode)
	{
		if (pChildNode == NULL)
			return;

		std::vector<SceneNode*>::iterator it = find(m_arrChildNode.begin(),m_arrChildNode.end(),pChildNode);
		if (it == m_arrChildNode.end())
			return;

		m_arrChildNode.erase(it);

		pChildNode->NotifySetParent(NULL);
	}

	void SceneNode::UpdateAABB()
	{
		const Matrix4x4& matWorld = GetWorldMatrix();
		m_aabbWS = m_aabbLS;
		m_aabbWS.Transform(matWorld);


		for (UINT nCnt = 0; nCnt < m_arrChildNode.size(); ++nCnt)
		{
			m_arrChildNode[nCnt]->UpdateAABB();
			m_aabbWS.Merge(m_arrChildNode[nCnt]->GetAABBWS());
		}

	}

	void SceneNode::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		if (GetParent()) // !RootNode
		{
			sl.Serialize(m_tsfWS);
		}

		if (sl.IsReading())
		{
			UINT nSize;;
			sl.Serialize(nSize,"size");

			for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
			{
				GameObject* pGameObj = GetEntitySystem()->CreateGameObject("");
				pGameObj->Serialize(sl);
			}
		}
		else
		{
			UINT nSize = m_arrChildNode.size();
			sl.Serialize(nSize,"size");

			for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
			{
				SceneNode* pChild = m_arrChildNode[nCnt];
				GameObject* pGameObj = pChild->GetGameObject();
				ASSERT(pGameObj);
				if (pGameObj == NULL)
					continue;

				pGameObj->Serialize(sl);
			}
		}

		sl.EndSection();
	}
}
