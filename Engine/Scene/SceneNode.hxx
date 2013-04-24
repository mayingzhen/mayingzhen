#include "SceneNode.h"

namespace ma
{
	IMPL_OBJECT(SceneNode,Object)

	SceneNode::SceneNode(Scene* pSene, const char* pNodeName):
	Object(pNodeName)
	{
		m_pScene = pSene;	
		m_pParentNode = NULL;
		m_btsfPSDirty = false;
		m_btsfWSDirty = true;
		m_bmatWSDirty = true;

		MatrixIdentity(&m_matWorld);
		TransformSetIdentity(&m_tsfPS);
		TransformSetIdentity(&m_tsfWS);

		//m_aabb.SetNull();
	}

	SceneNode::~SceneNode()
	{

	}

	void SceneNode::Update()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->Update();
		}
	}

	void SceneNode::ParalleUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->ParalleUpdate(fTimeElapsed);
		}
	}

	void SceneNode::FixedUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->FixedUpdate(fTimeElapsed);
		}
	}

	void SceneNode::LateUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->LateUpdate(fTimeElapsed);
		}
	}

	void SceneNode::Render()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->Render();
		}
	}

	void SceneNode::Start()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->Start();
		}
	}

	void SceneNode::Stop()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->Stop();
		}
	}

	void					SceneNode::SetTransformWS(const NodeTransform& tsfWS)
	{
		//profile_code();

		SetTransformWSDirty();

		m_tsfWS = tsfWS;
		m_btsfWSDirty = false;
		m_btsfPSDirty = true;
		m_bmatWSDirty = true;
	}

	void					SceneNode::SetTransformPS(const NodeTransform& tsfPS)
	{
		//profile_code();

		SetTransformWSDirty();

		m_tsfPS = tsfPS;
		m_btsfPSDirty = false;
		m_btsfWSDirty = true;
		m_bmatWSDirty = true;
	}

	void					SceneNode::SetTransformWSDirty()
	{
		m_btsfWSDirty = true;
		m_bmatWSDirty = true;
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			m_arrChildNode[i]->SetTransformWSDirty();
		}
	}

	const NodeTransform&	SceneNode::GetTransformPS() 
	{
		if (m_btsfPSDirty)
		{
			UpdateTransformPS();
		}
		return m_tsfPS;
	}

	const NodeTransform&	SceneNode::GetTransformWS() 
	{
		if (m_btsfWSDirty)
		{
			UpdateTransformWS();
		}
		return m_tsfWS;
	}

	const Matrix4x4&		SceneNode::GetWorldMatrix()
	{
		if (m_bmatWSDirty)
		{
			UpdateMatWorld();
		}
		return m_matWorld;
	}

	void					SceneNode::SetPositionPS(const Vector3& vPosPS)
	{
		NodeTransform tsfPS = GetTransformPS();
		tsfPS.m_vPos = vPosPS;
		SetTransformPS(tsfPS);
	}

	const Vector3&		SceneNode::GetPositionPS() 
	{
		return GetTransformPS().m_vPos;
	}


	void					SceneNode::SetPositionWS(const Vector3& vPosWS)
	{
		NodeTransform tsfWS = GetTransformWS();
		tsfWS.m_vPos = vPosWS;
		SetTransformWS(tsfWS);
	}

	const Vector3&		SceneNode::GetPositionWS() 
	{
		return GetTransformWS().m_vPos;
	}


	void					SceneNode::UpdateMatWorld()
	{
		NodeTransform tsfWS = GetTransformWS();
		MatrixFromTransform(&m_matWorld,&GetTransformWS());
		m_bmatWSDirty = false;
	}

	void					SceneNode::UpdateTransformWS()
	{
		assert(!m_btsfPSDirty);
		if (m_pParentNode == NULL)
		{
			m_tsfWS = m_tsfPS;
		}
		else
		{
			TransformMul(&m_tsfWS, &m_tsfPS, &m_pParentNode->GetTransformWS());
		}
		m_btsfWSDirty = false;
	}

	void					SceneNode::UpdateTransformPS()
	{
		assert(!m_btsfWSDirty);
		if (m_pParentNode == NULL)
		{
			m_tsfPS = m_tsfWS;
		}
		else
		{
			TransformInvMul(&m_tsfPS,&m_tsfWS,&m_pParentNode->GetTransformWS());
		}
		m_btsfPSDirty = false;
	}


	// void					SceneNode::SetTransformDirty()
	// {
	// 	m_btsfPSDirty = true;
	// 	m_btsfWSDirty = true;
	// 	m_bmatWSDirty = true;
	// 	for (UINT i = 0; i < m_arrChildNode.size(); ++i)
	// 	{
	// 		m_arrChildNode[i]->SetTransformDirty();
	// 	}
	// }


	void					SceneNode::WorldToLocal(NodeTransform* pOutLocal,const NodeTransform* pWorld)
	{
		NodeTransform tsfWorld = GetTransformWS();
		NodeTransform tsfWorldInv;
		TransformInverse(&tsfWorldInv,&tsfWorld);
		TransformMul(pOutLocal,pWorld,&tsfWorldInv);
	}


	void					SceneNode::TranslateLS(const Vector3& vDirLS)
	{
		Vector3 vDirPS;
		QuaternionTransformVector(&vDirPS,&vDirLS,&GetTransformPS().m_qRot);
		TranslatePS(vDirPS);
	}

	void					SceneNode::TranslatePS(const Vector3& vDirPS)
	{
		Vector3 vPosPS = GetPositionPS();
		vPosPS += vDirPS;
		SetPositionPS(vPosPS);
	}

	void					SceneNode::TranslateWS(const Vector3& vDirWS)
	{
		Vector3 vPosWS = GetPositionWS();
		vPosWS += vDirWS;
		SetPositionWS(vPosWS);
	}

	void					SceneNode::RotateYAxisLS(float fDegree)
	{
		Quaternion qRot;
		const Vector3 vY(0.0f,1.0f,0.0f);
		QuaternionRotationAxis(&qRot,&vY,ToRadian(fDegree));
		RotateLS(qRot);

	}

	void					SceneNode::RotateZAxisLS(float fDegree)
	{
		Quaternion qRot;
		const Vector3 vZ(0.0f,0.0f,1.0f);
		QuaternionRotationAxis(&qRot,&vZ,ToRadian(fDegree));
		RotateLS(qRot);
	}

	void					SceneNode::RotateLS(float xDegree,float yDegree,float zDegree)
	{
		Quaternion qRot;
		EulerAngleXYZ qEuler(ToRadian(xDegree), ToRadian(yDegree), ToRadian(xDegree));
		QuaternionFromEulerAngleXYZ(&qRot,&qEuler);
		RotateLS(qRot);
	}

	void					SceneNode::RotateLS(const Quaternion& qRot)
	{
		NodeTransform tsfPS = GetTransformPS();
		QuaternionMultiply(&tsfPS.m_qRot,&qRot,&tsfPS.m_qRot);
		SetTransformPS(tsfPS);
	}

	EulerAngleXYZ			SceneNode::GetRotateLS()
	{
		EulerAngleXYZ euler;
		EulerAngleFromQuaternion(&euler,&GetTransformPS().m_qRot);
		return euler;
	}


	void SceneNode::SyncToPhysics()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			m_arrChildNode[i]->SyncToPhysics();
		}	
	}

	void SceneNode::SyncFromPhysics()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			m_arrChildNode[i]->SyncFromPhysics();
		}
	}

	void SceneNode::AddChildNode(SceneNode* pChildNode)
	{
		if (pChildNode == NULL)
			return;

		std::vector<SceneNode*>::iterator it = find(m_arrChildNode.begin(),m_arrChildNode.end(),pChildNode);
		if (it != m_arrChildNode.end())
			return;

		m_arrChildNode.push_back(pChildNode);

		pChildNode->SetParentSceneNode(this);
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

	void SceneNode::Serialize(SerializeListener& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(m_tsfPS);

		sl.SerializeObjectArray(m_arrChildNode);

		sl.EndSection();

		if ( sl.IsReading() )
		{
			for(UINT i = 0; i < m_arrChildNode.size(); ++i)
			{
				m_arrChildNode[i]->SetParentSceneNode(this);
			}
		}
	}
}
