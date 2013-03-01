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

		const D3DXMATRIX& worldmat = m_pGameObject->GetWorldMatrix();
		pRender->RenderMesh( &worldmat, m_pMeshRes, m_pTexture );
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
		IRenderDevice* pRenderDevice = ma::GetRenderDevice();
		if (pRenderDevice == NULL)
			return;

		m_pMeshRes = pRenderDevice->CreateRendMesh(pszMeshPath);
		m_pMeshRes->Load();

		//m_pTexture = pRenderDevice->CreateRendTexture(pszTexPath);
		//m_pTexture->Load();
	}	

	void MeshComponent::GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3& vMax)
	{
		if (m_pMeshRes)
			m_pMeshRes->GetBoundingAABB(vMin,vMax);
	}

	void MeshComponent::Serialize(SerializeListener& sl, const char* pszLable)
	{
		IRenderDevice* pRenderDevice = ma::GetRenderDevice();
		if (pRenderDevice == NULL)
			return;

		sl.BeginSection(pszLable);

		if (m_pMeshRes == NULL)
		{
			m_pMeshRes = pRenderDevice->CreateRendMesh(NULL);
		}
		m_pMeshRes->Serialize(sl);

		if (m_pTexture == NULL)
		{
			//m_pTexture = pRenderDevice->CreateRendTexture(NULL);
		}
		//m_pTexture->Serialize(sl);

		sl.EndSection();
	}

}

