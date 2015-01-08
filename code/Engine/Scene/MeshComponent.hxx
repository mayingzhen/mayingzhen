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

	void MeshComponent::RegisterObject(Context* context)
	{
		ACCESSOR_ATTRIBUTE(MeshComponent, "SknPath", GetMeshFile, SetMeshFile, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(MeshComponent, "MatPath", GetMaterialFile, SetMaterialFile, const char*, NULL, AM_DEFAULT);
	}

	bool MeshComponent::Load(const char* pszSknPath,const char* pszMatPath)
	{
		SetMeshFile(pszSknPath);

		SetMaterialFile(pszMatPath);

		return OnLoadOver();
	}

	const char*	MeshComponent::GetMeshFile() const
	{
		return m_pMesData ? m_pMesData->GetResPath() : NULL;
	}

	void MeshComponent::SetMeshFile(const char* pFile)
	{
		m_pMesData = LoadResource<MeshData>(pFile);
	}

	const char*	 MeshComponent::GetMaterialFile() const
	{
		return m_pMatData ? m_pMatData->GetResPath() : NULL;
	}

	void MeshComponent::SetMaterialFile(const char* pFile)
	{
		m_pMatData = LoadResource<MaterialData>(pFile);
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
		OnLoadOver();
	}

	void MeshComponent::CreateRenderable()
	{
		ASSERT(m_pMatData && m_pMesData);
		if (m_pMatData == NULL || m_pMesData == NULL)
			return;
		
		for (UINT i = 0; i < m_pMesData->GetSubMeshNumber(); ++i)
		{
			Renderable* pRenderable = new Renderable();

			pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
			pRenderable->m_pDeclaration = m_pMesData->GetVertexDeclar(); 
			pRenderable->m_pVertexBuffers = m_pMesData->GetVertexBuffer(); 
			pRenderable->m_pIndexBuffer = m_pMesData->GetIndexBuffer();
			pRenderable->m_pSubMeshData = m_pMesData->GetSubMeshByIndex(i);
			pRenderable->m_pMaterial = CreateSubMaterial();
			uint32 matID = i;//pRenderable->m_pSubMeshData->m_nMateiralID;
			ASSERT(matID < m_pMatData->GetSubMatDataNumber());
			if (matID < m_pMatData->GetSubMatDataNumber())
			{
				const SubMaterialData& subMaterData = m_pMatData->GetSubMatDataByIndex(matID);
				pRenderable->m_pMaterial->InitWithSubMatData(subMaterData);
			}

			uint32 nBone = pRenderable->m_pSubMeshData->m_arrBonePalette.size();
			pRenderable->m_arrSkinDQ[0].resize(nBone);
			pRenderable->m_arrSkinDQ[1].resize(nBone);

			m_arrRenderable.push_back(pRenderable);
		}

		m_AABB = m_pMesData->GetBoundingAABB();
	}

	bool MeshComponent::OnLoadOver()
	{
		if (m_bOnLoadOver)
			return true;

		if (m_pMesData == NULL || !m_pMesData->IsInited())
			return false;

		if (m_pMatData == NULL || !m_pMatData->IsInited())
			return false;

		CreateRenderable();
		
		OnTransformChange();

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

			GetRenderSystem()->GetRenderQueue()->AddRenderObj(RL_Solid,m_arrRenderable[i]);
		}
	}

	void MeshComponent::SetSkinMatrix(const Matrix4* arrMatrixs,uint32 nCount)
	{
		profile_code();
		
		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->SetSkinMatrix(arrMatrixs,nCount);
		}
	}

	MeshComponentPtr CreateMeshComponent(const char* pszSknPath,const char* pszMatPath)
	{
		MeshComponent* pMeshcop = new MeshComponent();
		pMeshcop->Load(pszSknPath,pszMatPath);
		return pMeshcop;
	}

}

