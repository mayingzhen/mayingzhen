#include "Framework/SceneNode.h"

namespace ma
{
	IMPL_OBJECT(SceneNode,Object)
	SceneNode::SceneNode(Scene* pSene, const char* pNodeName):
	Object(pNodeName)
	{
		m_pScen = pSene;	
		m_pParentNode = NULL;

		D3DXMatrixIdentity(&m_matWorld);
		maTransformSetIdentity(&m_tsfPS);
	}

	SceneNode::~SceneNode()
	{

	}

	void SceneNode::Update()
	{
		for (UINT i = 0; i < m_vChildNodes.size(); ++i)
		{
			if (m_vChildNodes[i] == NULL)
				continue;

			m_vChildNodes[i]->Update();
		}
	}

	void SceneNode::ParalleUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_vChildNodes.size(); ++i)
		{
			if (m_vChildNodes[i] == NULL)
				continue;

			m_vChildNodes[i]->ParalleUpdate(fTimeElapsed);
		}
	}

	void SceneNode::FixedUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_vChildNodes.size(); ++i)
		{
			if (m_vChildNodes[i] == NULL)
				continue;

			m_vChildNodes[i]->FixedUpdate(fTimeElapsed);
		}
	}

	void SceneNode::LateUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_vChildNodes.size(); ++i)
		{
			if (m_vChildNodes[i] == NULL)
				continue;

			m_vChildNodes[i]->LateUpdate(fTimeElapsed);
		}
	}

	void SceneNode::Render()
	{
		for (UINT i = 0; i < m_vChildNodes.size(); ++i)
		{
			if (m_vChildNodes[i] == NULL)
				continue;

			m_vChildNodes[i]->Render();
		}
	}

	void SceneNode::Start()
	{
		for (UINT i = 0; i < m_vChildNodes.size(); ++i)
		{
			if (m_vChildNodes[i] == NULL)
				continue;

			m_vChildNodes[i]->Start();
		}
	}

	void SceneNode::Stop()
	{
		for (UINT i = 0; i < m_vChildNodes.size(); ++i)
		{
			if (m_vChildNodes[i] == NULL)
				continue;

			m_vChildNodes[i]->Stop();
		}
	}

	void SceneNode::TranslateLS(const D3DXVECTOR3& vDeltaLS)
	{
		D3DXVECTOR3 vDeltaPS;
		maQuaternionTransformVector(&vDeltaPS,&vDeltaLS,&m_tsfPS.m_qRot);
		TranslatePS(vDeltaPS);
	}

	void SceneNode::TranslatePS(const D3DXVECTOR3& vDeltaPS)
	{
		m_tsfPS.m_vPos += vDeltaPS;
	}

	void SceneNode::TranslateWS(const D3DXVECTOR3& vDeltaWS)
	{
		maNodeTransform tsfParentWorldInv;
		if (m_pParentNode)
		{
			maNodeTransform tsfParentWorld = m_pParentNode->GetTransformWS();
			maTransformInverse(&tsfParentWorldInv,&tsfParentWorld);
		}
		else
		{
			maTransformSetIdentity(&tsfParentWorldInv);
		}

		D3DXVECTOR3 vDeltaPS;
		maVec3TransformNormal(&vDeltaPS,&vDeltaWS,&tsfParentWorldInv);
		TranslatePS(vDeltaPS);
	}

	void SceneNode::RotateLS(const D3DXQUATERNION* pRot)
	{
		D3DXQuaternionMultiply(&m_tsfPS.m_qRot,pRot,&m_tsfPS.m_qRot);
	}

	void SceneNode::RotateLS(float x,float y,float z)
	{
		D3DXQUATERNION qRot;
		EulerAngleXYZ qEuler(x,y,z);
		maQuaternionFromEulerAngleXYZ(&qRot,&qEuler);
		RotateLS(&qRot);
	}

	maNodeTransform SceneNode::GetTransformWS()
	{
		if (m_pParentNode == NULL)
		{
			return m_tsfPS;
		}
		else
		{
			maNodeTransform tsfParentWS = m_pParentNode->GetTransformWS();
			maNodeTransform tsfWS;
			maTransfromMul(&tsfWS,&m_tsfPS,&tsfParentWS);
			return tsfWS;
		}
	}

	void SceneNode::SetTransformWS(const maNodeTransform& TSFWS)
	{
		if (m_pParentNode == NULL)
		{
			m_tsfPS = TSFWS;
		}
		else
		{
			maNodeTransform tsfParentWS = m_pParentNode->GetTransformWS();
			maTransfromInvMul(&m_tsfPS,&TSFWS,&tsfParentWS);
		}
	}


	void SceneNode::SyncWorld()
	{
		if (m_pParentNode != NULL)
		{
			D3DXMATRIX matPS;
			maMatrixFromTransform(&matPS,&m_tsfPS);
			D3DXMatrixMultiply(&m_matWorld,&matPS,&m_pParentNode->GetWorldMatrix());
		}
		else
		{
			maMatrixFromTransform(&m_matWorld,&m_tsfPS);
		}

		for (UINT i = 0; i < m_vChildNodes.size(); ++i)
		{
			m_vChildNodes[i]->SyncWorld();
		}
	}

	void SceneNode::SyncToPhysics()
	{
		for (UINT i = 0; i < m_vChildNodes.size(); ++i)
		{
			m_vChildNodes[i]->SyncToPhysics();
		}	
	}

	void SceneNode::SyncFromPhysics()
	{
		for (UINT i = 0; i < m_vChildNodes.size(); ++i)
		{
			m_vChildNodes[i]->SyncFromPhysics();
		}
	}

	void SceneNode::AddChildNode(SceneNode* pChildNode)
	{
		if (pChildNode == NULL)
			return;

		std::vector<SceneNode*>::iterator it = find(m_vChildNodes.begin(),m_vChildNodes.end(),pChildNode);
		if (it != m_vChildNodes.end())
			return;

		m_vChildNodes.push_back(pChildNode);

		pChildNode->SetParentSceneNode(this);
	}

	void SceneNode::Serialize(SerializeListener& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(m_tsfPS);

		sl.SerializeObjectArray(m_vChildNodes);

		sl.EndSection();

		if ( sl.IsReading() )
		{
			for(UINT i = 0; i < m_vChildNodes.size(); ++i)
			{
				m_vChildNodes[i]->SetParentSceneNode(this);
			}
		}
	}
}
