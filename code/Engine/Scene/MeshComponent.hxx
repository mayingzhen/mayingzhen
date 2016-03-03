#include "MeshComponent.h"
#include "../RenderSystem/RenderQueue.h"
#include "../RenderSystem/RenderThread.h"

namespace ma
{
	MeshComponent::MeshComponent()
	{
		m_bOnLoadOver = false;
	}

	void MeshComponent::RegisterAttribute()
	{
		REGISTER_OBJECT(MeshComponent,CreateMeshComponent); 

		ACCESSOR_ATTRIBUTE(MeshComponent, "SknPath", GetMeshFile, SetMeshFile, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(MeshComponent, "MatPath", GetMaterialFile, SetMaterialFile, const char*, NULL, AM_DEFAULT);
	}

	bool MeshComponent::Load(const char* pszSknPath,const char* pszMatPath)
	{
		SetMeshFile(pszSknPath);

		SetMaterialFile(pszMatPath);

		return IsReady();
	}

	const char*	MeshComponent::GetMeshFile() const
	{
		return m_pMesData ? m_pMesData->GetResPath() : NULL;
	}

	void MeshComponent::SetMeshFile(const char* pFile)
	{
		m_pMesData = CreateMeshData(pFile);

		m_bOnLoadOver = false;
		IsReady();
	}

	const char*	 MeshComponent::GetMaterialFile() const
	{
		return m_pMaterial ? m_pMaterial->GetXMLFile()->GetResPath() : NULL;
	}

	void MeshComponent::SetMaterialFile(const char* pFile)
	{
		m_pMaterial = CreateMaterial(pFile);

		m_bOnLoadOver = false;
		IsReady();
	}

	UINT MeshComponent::GetRenderableCount(uint32 nLod) const
	{
		return m_arrLodRenderable[nLod].size();
	}

	Renderable*	MeshComponent::GetRenderableByIndex(uint32 nLod,UINT index) const
	{
		return m_arrLodRenderable[nLod][index].get();
	}

	UINT32 MeshComponent::GetSubMaterialCount(uint32 nLod)
	{
		return m_arrLodRenderable[nLod].size();
	}

	SubMaterial* MeshComponent::GetSubMaterial(uint32 nLod,UINT index)
	{
		return m_arrLodRenderable[index][index]->GetMaterial();
	}

	void MeshComponent::Update()
	{
		IsReady();

		RenderComponent::Update();
	}

	void MeshComponent::CreateRenderable()
	{
		ASSERT(m_pMaterial && m_pMesData);
		if (m_pMaterial == NULL || m_pMesData == NULL)
			return;
		
		for (uint32 iLod = 0; iLod < m_pMesData->GetLodNumerber(); ++iLod)
		{
			VEC_RENDERABLE arrRenderable;
			for (UINT iSub = 0; iSub < m_pMesData->GetSubMeshNumber(iLod); ++iSub)
			{
				MeshRenderable* pRenderable = new MeshRenderable();

				pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
				pRenderable->m_pDeclaration = m_pMesData->GetVertexDeclar(); 
				pRenderable->m_pVertexBuffer = m_pMesData->GetVertexBuffer(); 
				pRenderable->m_pIndexBuffer = m_pMesData->GetIndexBuffer();
				pRenderable->m_pSubMeshData = m_pMesData->GetSubMeshByIndex(iLod,iSub);
				pRenderable->m_pSubMaterial = m_pMaterial->GetSubMaterialByIndex(iLod,iSub); 
				pRenderable->m_posAABB = m_pMesData->GetBoundingAABB();
				pRenderable->m_tcAABB = m_pMesData->GetUVBoundingAABB();

				arrRenderable.push_back(pRenderable);
			}
			m_arrLodRenderable.push_back(arrRenderable);
		}

		SetAABB(m_pMesData->GetBoundingAABB());
	}

	bool MeshComponent::IsReady()
	{
		if (m_bOnLoadOver)
			return true;

		if (m_pMesData == NULL || !m_pMesData->IsReady())
			return false;

		if (m_pMaterial == NULL || !m_pMaterial->IsReady())
			return false;

		CreateRenderable();
		
		MarkDirty();

		m_bOnLoadOver = true;
		
		return true;
	}

	void MeshComponent::Show(Camera* pCamera)
	{
		RenderComponent::Show(pCamera);

		if ( m_arrLodRenderable.empty() )
			return;

		uint32 nLod = 0;

		for (UINT i = 0; i < m_arrLodRenderable[nLod].size(); ++i)
		{
			 m_arrLodRenderable[nLod][i]->SetWorldMatrix( m_pSceneNode->GetMatrixWS() );

			m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Mesh, m_arrLodRenderable[nLod][i].get());
		}
	}

	RefPtr<MeshComponent> CreateMeshComponent()
	{
		return new MeshComponent();
	}

}

