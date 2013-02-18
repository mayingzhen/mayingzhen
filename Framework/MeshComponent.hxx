#include "Framework/MeshComponent.h"

namespace ma
{
	IMPL_OBJECT(MeshComponent,Component)

	MeshComponent::MeshComponent()
	{
		m_pMeshRes = NULL;
		m_pTexture = NULL;
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
		m_pMeshRes->Load();	
		m_pTexture->Load();
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

	void MeshComponent::GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3& vMax)
	{
		IRendMesh* pRendMesh = m_pMeshRes->GetRendMesh();
		if (pRendMesh == NULL)
			return;

		pRendMesh->GetBoundingAABB(vMin,vMax);
	}

	void MeshComponent::Serialize(SerializeListener& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		if (m_pMeshRes == NULL)
		{
			m_pMeshRes = new MeshRes(NULL);
		}
		m_pMeshRes->Serialize(sl);

		if (m_pTexture == NULL)
		{
			m_pTexture = new Texture(NULL);
		}
		m_pTexture->Serialize(sl);

		sl.EndSection();
	}

}

