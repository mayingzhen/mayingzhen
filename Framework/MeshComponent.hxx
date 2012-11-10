#include "MeshComponent.h"

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

		if (m_pMesh == NULL || m_pGameObject == NULL)
			return;

		D3DXMATRIX worldmat = m_pGameObject->GetWorldMatrix();
		pRender->RenderMesh( &worldmat, m_pMesh->GetRendMesh(), m_pTexture->GetRendTexture() );
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
		m_pMesh = new Mesh(pszMeshPath);
		m_pMesh->Load();

		m_pTexture = new Texture(pszTexPath);
		m_pTexture->Load();
	}	

}

