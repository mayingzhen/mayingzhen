#include "SceneNode.h"
#include "CullTree.h"

namespace ma
{
	IMPL_OBJECT(SceneNode,Object)

	SceneNode::SceneNode(GameObject* pGameObj)
	{
		//m_pParentNode = NULL;
		m_pGameObject = pGameObj;

		MatrixIdentity(&m_matWorld);
		TransformSetIdentity(&m_tsfWS);
		m_bmatWSDirty = true;
	}

	SceneNode::~SceneNode()
	{

	}

// 	bool SceneNode::TravelScene(SceneVisiter* pVisiter)
// 	{
// 		bool bTraveling = pVisiter->VisiteSceneNodeBegin(this);
// 
// 		if (bTraveling)
// 		{
// 			if (NULL != m_pGameObject)
// 			{
// 				bTraveling = m_pGameObject->TravelScene(pVisiter);
// 				if(!bTraveling)
// 				{
// 					//break;
// 				}
// 			}
// 		}
// 
// 		if (bTraveling)
// 		{
// 			for (UINT nCnt = 0; nCnt < m_arrChildNode.size(); ++nCnt)
// 			{
// 				bTraveling = m_arrChildNode[nCnt]->TravelScene(pVisiter);
// 				if (!bTraveling)
// 				{
// 					break;
// 				}
// 			}
// 		}
// 
// 		pVisiter->VisiteSceneNodeEnd(this);
// 
// 		return bTraveling;
// 	}


	void SceneNode::SetTransform(const NodeTransform& tsf)
	{
		m_tsfWS = tsf;

		m_bmatWSDirty = true;

// 		UINT nComp = m_pGameObject->GetComponentNumber();
// 		for (UINT i = 0; i < nComp; ++i)
// 		{
// 			Component* pComp = m_pGameObject->GetComponentByIndex(i);
// 			if (pComp == NULL)
// 				continue;
// 
// 			pComp->UpdateTransform();
// 		}
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
		Translate(GetForward() * x);
	}

	void  SceneNode::Up(float x)
	{
		Translate(GetUp() * x);
	}

	void  SceneNode::Right(float x)
	{
		Translate(GetRight() * x);
	}

	void  SceneNode::Scale(float fScale)
	{
		NodeTransform tsf = GetTransform();
		tsf.m_fScale *= fScale;
		SetTransform(tsf);
	}

	void SceneNode::LookAt(const Vector3& vPos, const Vector3& vTarget,const Vector3& vUp)
	{
		Matrix4x4 matView;
		Vector3 vSource = vPos;
		MatrixLookAtLH(&matView,&vSource,&vTarget,&vUp);
		Matrix4x4 matWorld;
		MatrixInverse(&matWorld,NULL,&matView);
		NodeTransform tsfWS;
		TransformFromMatrix(&tsfWS,&matWorld);
		SetTransform(tsfWS);
	}

	void SceneNode::LookAt(const Vector3& vTarget,const Vector3& vUp)
	{
		Vector3 vSource = GetTransform().m_vPos;

		LookAt(vSource,vTarget,vUp);
	}

	void SceneNode::Translate(const Vector3& vTrans)
	{
		NodeTransform tsf = GetTransform(); 
		tsf.m_vPos += vTrans;
		SetTransform(tsf);
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
		m_bmatWSDirty = false;

		UpdateAABB();

		if (/*bNeedUpdate && */m_pGameObject && m_pGameObject->GetCullTree() )
		{
			m_pGameObject->GetCullTree()->UpdateObject(m_pGameObject);
			//m_nAABBChangeType &= ~ACT_NOTIFY;
		}
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
		NodeTransform tsfRot;
		TransformSetIdentity(&tsfRot);
		tsfRot.m_qRot = qRot;
		NodeTransform newTsf;
		TransformMul(&newTsf,&GetTransform(),&tsfRot);

		SetTransform(newTsf);
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

// 	void SceneNode::NotifySetParent(SceneNode* pParentNode)
// 	{
// 		m_pParentNode = pParentNode;
// 	}
// 
// 	void SceneNode::AddChildNode(SceneNode* pChildNode)
// 	{
// 		if (pChildNode == NULL)
// 			return;
// 
// 		std::vector<SceneNode*>::iterator it = find(m_arrChildNode.begin(),m_arrChildNode.end(),pChildNode);
// 		if (it != m_arrChildNode.end())
// 			return;
// 
// 		m_arrChildNode.push_back(pChildNode);
// 
// 		pChildNode->NotifySetParent(this);
// 	}
// 
// 	void SceneNode::RemoveChildNode(SceneNode* pChildNode)
// 	{
// 		if (pChildNode == NULL)
// 			return;
// 
// 		std::vector<SceneNode*>::iterator it = find(m_arrChildNode.begin(),m_arrChildNode.end(),pChildNode);
// 		if (it == m_arrChildNode.end())
// 			return;
// 
// 		m_arrChildNode.erase(it);
// 
// 		pChildNode->NotifySetParent(NULL);
// 	}


// 	m_localBound.SetNull();
// 	if (NULL != m_pGameObj)
// 	{
// 		m_pGameObj->UpdateLocalBounds();
// 		m_localBound.Merge(* m_pGameObj->GetLocalBounds());
// 	}
// 
// 
// 	const xmMatrix4x4& matWorld = GetWorldMatrix();
// 	m_aabbBound = m_localBound;
// 	m_aabbBound.Transform(matWorld);
// 
// 	for (xmUint nCnt = 0; nCnt < m_arrChildNode.size(); ++nCnt)
// 	{
// 		m_arrChildNode[nCnt]->UpdateBounds();
// 		m_aabbBound.Merge(*m_arrChildNode[nCnt]->GetBounds());
// 	}

	void SceneNode::UpdateAABB()
	{
		if (m_pGameObject)
		{
			for (UINT i = 0; i < m_pGameObject->GetComponentNumber(); ++i)
			{
				Component* pComponent = m_pGameObject->GetComponentByIndex(i);
				if (pComponent == NULL)
					continue;

				m_aabbWS.Merge( pComponent->GetAABBWS() );
			}
		}
 
// 		for (UINT nCnt = 0; nCnt < m_arrChildNode.size(); ++nCnt)
// 		{
// 			m_arrChildNode[nCnt]->UpdateAABB();
// 			m_aabbWS.Merge(m_arrChildNode[nCnt]->GetAABBWS());
// 		}

	}

	void SceneNode::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		//if ( GetParent() ) // !RootNode
		//{
			sl.Serialize(m_tsfWS);
		//}

// 		if (sl.IsReading())
// 		{
// 			UINT nSize;;
// 			sl.Serialize(nSize,"size");
// 
// 			for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
// 			{
// 				GameObject* pGameObj = GetEntitySystem()->CreateGameObject("");
// 				pGameObj->Serialize(sl);
// 			}
// 		}
// 		else
// 		{
// 			UINT nSize = m_arrChildNode.size();
// 			sl.Serialize(nSize,"size");
// 
// 			for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
// 			{
// 				SceneNode* pChild = m_arrChildNode[nCnt];
// 				GameObject* pGameObj = pChild->GetGameObject();
// 				ASSERT(pGameObj);
// 				if (pGameObj == NULL)
// 					continue;
// 
// 				pGameObj->Serialize(sl);
// 			}
// 		}

		sl.EndSection();
	}
}
