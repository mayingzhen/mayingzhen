#include "MeshComponent.h"

namespace ma
{
	IMPL_OBJECT(MeshComponent,Component)

	MeshComponent::MeshComponent()
	{
		m_pRendMesh = NULL;
		m_pTexture = NULL;
	}

	MeshComponent::~MeshComponent()
	{

	}

	void MeshComponent::Render()
	{
		if (GetRenderDevice() == NULL)
			return;

		if (m_pRendMesh == NULL || m_pGameObject == NULL)
			return;

		const D3DXMATRIX& worldmat = m_pGameObject->GetWorldMatrix();
		m_pRendMesh->m_pMesh = m_pRendMesh;
		m_pRendMesh->m_pTex = m_pTexture;
		m_pRendMesh->m_pMatWorld = &worldmat;
		GetRenderDevice()->RenderMesh(m_pRendMesh);
	}

	void MeshComponent::Update()
	{
		
	}

	void MeshComponent::Start()
	{
		//m_pRendMesh->Load();	
		//m_pTexture->Load();
	}

	void MeshComponent::Stop()
	{

	}

	void MeshComponent::Load(const char* pszMeshPath,const char* pszTexPath)
	{
		IRenderDevice* pRenderDevice = ma::GetRenderDevice();
		if (pRenderDevice == NULL)
			return;

		m_pRendMesh = pRenderDevice->CreateRendMesh();
		MeshData* pMeshData = LoadMeshFromBinaryFile(pszMeshPath);
		m_pRendMesh->InitWithData(pMeshData);

		m_pTexture = pRenderDevice->CreateRendTexture();
		m_pTexture->Load(pszTexPath);
	}	

	void MeshComponent::GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3& vMax)
	{
		if (m_pRendMesh)
		{
			m_pRendMesh->GetBoundingAABB(vMin,vMax);
		}
	}

	void MeshComponent::Serialize(SerializeListener& sl, const char* pszLable)
	{
		IRenderDevice* pRenderDevice = ma::GetRenderDevice();
		if (pRenderDevice == NULL)
			return;

		sl.BeginSection(pszLable);

		std::string sMeshPath,sTexturePath;
		if (m_pRendMesh)
		{
			MeshData* pMeshData = m_pRendMesh->GetMeshData();
			if (pMeshData)
			{
				sMeshPath = pMeshData->GetResPath();
			}
		}

		if (m_pTexture)
		{
			sTexturePath = m_pTexture->GetResPath();
		}

		sl.Serialize(sMeshPath);
		sl.Serialize(sTexturePath);

		if ( sl.IsReading() )
		{
			Load(sMeshPath.c_str(),sTexturePath.c_str());
		}

		sl.EndSection();
	}

}

