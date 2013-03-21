#include "SceneNode.h"

namespace ma
{
	IMPL_OBJECT(SceneNode,Object)

	SceneNode::SceneNode(Scene* pSene, const char* pNodeName):
	Object(pNodeName)
	{
		m_pScen = pSene;	
		m_pParentNode = NULL;
		m_btsfPSDirty = false;
		m_btsfWSDirty = true;
		m_bmatWSDirty = true;

		D3DXMatrixIdentity(&m_matWorld);
		maTransformSetIdentity(&m_tsfPS);
		maTransformSetIdentity(&m_tsfWS);

		m_aabb.SetNull();
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

	void					SceneNode::SetTransformWS(const maNodeTransform& tsfWS)
	{
		//profile_code();

		SetTransformWSDirty();

		m_tsfWS = tsfWS;
		m_btsfWSDirty = false;
		m_btsfPSDirty = true;
		m_bmatWSDirty = true;
	}

	void					SceneNode::SetTransformPS(const maNodeTransform& tsfPS)
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

	const maNodeTransform&	SceneNode::GetTransformPS() 
	{
		if (m_btsfPSDirty)
		{
			UpdateTransformPS();
		}
		return m_tsfPS;
	}

	const maNodeTransform&	SceneNode::GetTransformWS() 
	{
		if (m_btsfWSDirty)
		{
			UpdateTransformWS();
		}
		return m_tsfWS;
	}

	const D3DXMATRIX&		SceneNode::GetWorldMatrix()
	{
		if (m_bmatWSDirty)
		{
			UpdateMatWorld();
		}
		return m_matWorld;
	}

	void					SceneNode::SetPositionPS(const D3DXVECTOR3& vPosPS)
	{
		maNodeTransform tsfPS = GetTransformPS();
		tsfPS.m_vPos = vPosPS;
		SetTransformPS(tsfPS);
	}

	const D3DXVECTOR3&		SceneNode::GetPositionPS() 
	{
		return GetTransformPS().m_vPos;
	}


	void					SceneNode::SetPositionWS(const D3DXVECTOR3& vPosWS)
	{
		maNodeTransform tsfWS = GetTransformWS();
		tsfWS.m_vPos = vPosWS;
		SetTransformWS(tsfWS);
	}

	const D3DXVECTOR3&		SceneNode::GetPositionWS() 
	{
		return GetTransformWS().m_vPos;
	}


	void					SceneNode::UpdateMatWorld()
	{
		maNodeTransform tsfWS = GetTransformWS();
		maMatrixFromTransform(&m_matWorld,&GetTransformWS());
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
			maTransformMul(&m_tsfWS, &m_tsfPS, &m_pParentNode->GetTransformWS());
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
			maTransfromInvMul(&m_tsfPS,&m_tsfWS,&m_pParentNode->GetTransformWS());
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


	void					SceneNode::WorldToLocal(maNodeTransform* pOutLocal,const maNodeTransform* pWorld)
	{
		maNodeTransform tsfWorld = GetTransformWS();
		maNodeTransform tsfWorldInv;
		maTransformInverse(&tsfWorldInv,&tsfWorld);
		maTransformMul(pOutLocal,pWorld,&tsfWorldInv);
	}


	void					SceneNode::TranslateLS(const D3DXVECTOR3& vDirLS)
	{
		D3DXVECTOR3 vDirPS;
		maQuaternionTransformVector(&vDirPS,&vDirLS,&GetTransformPS().m_qRot);
		TranslatePS(vDirPS);
	}

	void					SceneNode::TranslatePS(const D3DXVECTOR3& vDirPS)
	{
		D3DXVECTOR3 vPosPS = GetPositionPS();
		vPosPS += vDirPS;
		SetPositionPS(vPosPS);
	}

	void					SceneNode::TranslateWS(const D3DXVECTOR3& vDirWS)
	{
		D3DXVECTOR3 vPosWS = GetPositionWS();
		vPosWS += vDirWS;
		SetPositionWS(vPosWS);
	}

	void					SceneNode::RotateYAxisLS(float fDegree)
	{
		D3DXQUATERNION qRot;
		const D3DXVECTOR3 vY(0.0f,1.0f,0.0f);
		D3DXQuaternionRotationAxis(&qRot,&vY,D3DXToRadian(fDegree));
		RotateLS(qRot);

	}

	void					SceneNode::RotateZAxisLS(float fDegree)
	{
		D3DXQUATERNION qRot;
		const D3DXVECTOR3 vZ(0.0f,0.0f,1.0f);
		D3DXQuaternionRotationAxis(&qRot,&vZ,D3DXToRadian(fDegree));
		RotateLS(qRot);
	}

	void					SceneNode::RotateLS(float xDegree,float yDegree,float zDegree)
	{
		D3DXQUATERNION qRot;
		EulerAngleXYZ qEuler(D3DXToRadian(xDegree), D3DXToRadian(yDegree), D3DXToRadian(xDegree));
		maQuaternionFromEulerAngleXYZ(&qRot,&qEuler);
		RotateLS(qRot);
	}

	void					SceneNode::RotateLS(const D3DXQUATERNION& qRot)
	{
		maNodeTransform tsfPS = GetTransformPS();
		D3DXQuaternionMultiply(&tsfPS.m_qRot,&qRot,&tsfPS.m_qRot);
		SetTransformPS(tsfPS);
	}

	EulerAngleXYZ			SceneNode::GetRotateLS()
	{
		EulerAngleXYZ euler;
		maEulerAngleFromQuaternion(&euler,&GetTransformPS().m_qRot);
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
