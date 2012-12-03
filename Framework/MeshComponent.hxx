#include "Framework/MeshComponent.h"

namespace ma
{
	MeshComponent::MeshComponent()
	{

	}

	MeshComponent::~MeshComponent()
	{

	}

	void MeshComponent::Render()
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		if (m_pMeshRes == NULL || m_pGameObject == NULL)
			return;

		D3DXMATRIX worldmat = m_pGameObject->GetWorldMatrix();
		pRender->RenderMesh( &worldmat, m_pMeshRes->GetRendMesh(), m_pTexture->GetRendTexture() );
	}

	void MeshComponent::Update()
	{

	}

	void MeshComponent::Start()
	{

	}

	void MeshComponent::Stop()
	{

	}

	void MeshComponent::Load(const char* pszMeshPath,const char* pszTexPath)
	{
		m_pMeshRes = new MeshRes(pszMeshPath);
		m_pMeshRes->Load();

		m_pTexture = new Texture(pszTexPath);
		m_pTexture->Load();
	}	

}

