#include "Framework/SceneNode.h"

namespace ma
{
	SceneNode::SceneNode(Scene* pSene, const char* pNodeName):
	Object(pNodeName)
	{
		m_pScen = pSene;	
		m_pParentNode = NULL;
	}

	SceneNode::~SceneNode()
	{

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
			maTransfromMul(&tsfParentWS,&m_tsfPS,&tsfParentWS);
			return tsfWS;
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

}


