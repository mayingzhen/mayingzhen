#include "MeshComponent.h"
#include "../RenderSystem/RenderQueue.h"
#include "../RenderSystem/RenderThread.h"

namespace ma
{
	IMPL_OBJECT(MeshComponent,RenderComponent)

	MeshComponent::MeshComponent(GameObject* pGameObj):
		RenderComponent(pGameObj)
	{
		m_pMaterial = NULL;
		m_bSkin = false;
	}

	bool MeshComponent::Load(const char* pszSknPath,const char* pszMatPath)
	{
		m_pMesData = LoadResourceSync<MeshData>(pszSknPath);

		CreateRenderable(m_pMesData);

		m_pMatData = LoadResourceSync<MaterialData>(pszMatPath);
		m_pMaterial = new Material(m_pMatData);
	
		return true;
	}

	void MeshComponent::Update()
	{
// 		if (m_pMesData->GetResState() == ResLoaded && m_arrRenderable.empty() )
// 		{
// 			CreateRenderable(m_pMesData);
// 		}
// 		
// 		if (m_pMatData->GetResState() == ResLoaded && m_pMaterial == NULL)
// 		{
// 			m_pMaterial = new Material(m_pMatData);
// 		}	
	}

	bool MeshComponent::CreateRenderable(ref_ptr<MeshData> pMeshData)
	{
		if (pMeshData == NULL)
			return false;

		m_pMesData = pMeshData;

		for (UINT i = 0; i < pMeshData->GetSubMeshNumber(); ++i)
		{
			SubMeshData* pSubMesh = pMeshData->GetSubMeshByIndex(i);

			Renderable* pRenderable = new Renderable();
			pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
			pRenderable->m_pDeclaration = pMeshData->GetVertexDeclar(); 
			pRenderable->m_pVertexBuffers = pMeshData->GetVertexBuffer(); 
			pRenderable->m_pIndexBuffer =  pMeshData->GetIndexBuffer();
			pRenderable->m_pSubMeshData = pSubMesh;
			pRenderable->m_pMaterial = m_pMaterial;

			Uint nBone = pSubMesh->m_arrBonePalette.size();
			pRenderable->m_arrSkinMatrix[0].resize(nBone);
			pRenderable->m_arrSkinMatrix[1].resize(nBone);
			m_bSkin = nBone > 0 ? true : false;
			for (Uint i = 0; i < nBone; ++i)
			{
				pRenderable->m_arrSkinMatrix[0][i] = Matrix4x4::identity();
				pRenderable->m_arrSkinMatrix[1][i] = Matrix4x4::identity();
			}
 
			m_arrRenderable.push_back(pRenderable);
		}

		return true;
	}

	void MeshComponent::Show(Camera* pCamera)
	{
		if ( m_arrRenderable.empty() || m_pMaterial == NULL)
			return;

		int index = GetRenderSystem()->CurThreadFill();

		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->m_matWorld[index] = m_pSceneNode->GetWorldMatrix();
			m_arrRenderable[i]->m_pMaterial = m_pMaterial;

			GetRenderSystem()->GetRenderQueue()->AddRenderObj(RL_Solid,m_arrRenderable[i]);
		}
	}

	AABB MeshComponent::GetAABBWS()
	{
		return m_aabbWS;
	}

	void MeshComponent::Serialize(Serializer& sl, const char* pszLable/* = "MeshComponent"*/)
	{
		sl.BeginSection(pszLable);

		std::string strSknPath = m_pMesData ? m_pMesData->GetResPath() : "";
		std::string strMatPath = m_pMatData ? m_pMatData->GetResPath() : "";

		sl.Serialize(strSknPath,"SknPath");
		sl.Serialize(strMatPath,"MatPath");

		if ( sl.IsReading() )
		{
			Load(strSknPath.c_str(),strMatPath.c_str());
		}

		sl.EndSection();
	}

	void MeshComponent::SetSkinMatrix(const Matrix4x4* arrMatrixs,Uint nCount)
	{
		profile_code();
		
		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->SetSkinMatrix(arrMatrixs,nCount);
		}

	}

}

