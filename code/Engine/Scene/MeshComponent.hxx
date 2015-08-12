#include "MeshComponent.h"
#include "../RenderSystem/RenderQueue.h"
#include "../RenderSystem/RenderThread.h"

namespace ma
{
	IMPL_OBJECT(MeshComponent,RenderComponent)	

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
		m_pMesData = LoadResource<MeshData>(pFile);

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

	UINT MeshComponent::GetRenderableCount() const
	{
		return m_arrRenderable.size();
	}

	Renderable*	MeshComponent::GetRenderableByIndex(UINT index) const
	{
		return m_arrRenderable[index].get();
	}

	UINT32 MeshComponent::GetSubMaterialCount()
	{
		return m_arrRenderable.size();
	}

	SubMaterial* MeshComponent::GetSubMaterial(UINT index)
	{
		return m_arrRenderable[index]->GetMaterial();
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
		
		for (UINT i = 0; i < m_pMesData->GetSubMeshNumber(); ++i)
		{
			Renderable* pRenderable = new Renderable();

			pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
			pRenderable->m_pDeclaration = m_pMesData->GetVertexDeclar(); 
			pRenderable->m_pVertexBuffers = m_pMesData->GetVertexBuffer(); 
			pRenderable->m_pIndexBuffer = m_pMesData->GetIndexBuffer();
			pRenderable->m_pSubMeshData = m_pMesData->GetSubMeshByIndex(i);
			pRenderable->m_pSubMaterial = m_pMaterial->GetSubMaterialByIndex(i); 

			m_arrRenderable.push_back(pRenderable);
		}

		m_AABB = m_pMesData->GetBoundingAABB();
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

		if ( m_arrRenderable.empty() )
			return;

		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->SetWorldMatrix( m_pSceneNode->GetMatrixWS() );

			m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Solid, m_arrRenderable[i].get());
		}
	}

	RefPtr<MeshComponent> CreateMeshComponent()
	{
		return new MeshComponent();
	}

	IMPL_OBJECT(SkinMeshComponent,MeshComponent)	

	void SkinMeshComponent::CreateRenderable()
	{
		ASSERT(m_pMaterial && m_pMesData);
		if (m_pMaterial == NULL || m_pMesData == NULL)
			return;

		for (UINT i = 0; i < m_pMesData->GetSubMeshNumber(); ++i)
		{
			Renderable* pRenderable = new SkinRenderable();

			pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
			pRenderable->m_pDeclaration = m_pMesData->GetVertexDeclar(); 
			pRenderable->m_pVertexBuffers = m_pMesData->GetVertexBuffer(); 
			pRenderable->m_pIndexBuffer = m_pMesData->GetIndexBuffer();
			pRenderable->m_pSubMeshData = m_pMesData->GetSubMeshByIndex(i);
			pRenderable->m_pSubMaterial = m_pMaterial->GetSubMaterialByIndex(i); 
	
			m_arrRenderable.push_back(pRenderable);
		}

		m_AABB = m_pMesData->GetBoundingAABB();
	}

	void SkinMeshComponent::RegisterAttribute()
	{
		REGISTER_OBJECT(SkinMeshComponent,CreateSkinMeshComponent); 

		COPY_BASE_ATTRIBUTES(SkinMeshComponent,MeshComponent);
	}

	void SkinMeshComponent::SetSkinMatrix(const Matrix3x4* arrMatrixs,uint32 nCount)
	{
		profile_code();
		
		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			SkinRenderable* pSkinRenderable = (SkinRenderable*)m_arrRenderable[i].get();
			pSkinRenderable->SetSkinMatrix(arrMatrixs,nCount);
		}
	}

	RefPtr<SkinMeshComponent> CreateSkinMeshComponent()
	{
		return new SkinMeshComponent();
	}

}

