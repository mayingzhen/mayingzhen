#include "RendMesh.h"

namespace ma
{
	RenderMesh::RenderMesh()
	{
		m_pMesData = NULL;
		m_pMaterial = NULL;
		m_bSkin = false;
	}
	
	bool RenderMesh::Load(const char* pMeshPath,const char* pDiffueTexture)
	{
		if (pMeshPath == NULL)
			return false;

		MeshData* pMeshData = SafeCast<MeshData>(ResourceManager::DeclareResource(pMeshPath));
		ASSERT(pMeshData);
		if (pMeshData)
		{
			pMeshData->Load();

			InitWithData(pMeshData);
		}
			
	
		if (pDiffueTexture)
		{
			TextureData* pTextureData = SafeCast<TextureData>(ResourceManager::DeclareResource(pDiffueTexture));
			ASSERT(pTextureData);
			if (pTextureData == NULL)
				return false;

			pTextureData->Load();
			Sampler* sampler = Sampler::create(pTextureData->GetRenderTexture()); // +ref texture
			
			std::string sMaterFlag;
			if (m_bSkin)
			{
				sMaterFlag = "DIFFUSE;SKIN; SKIN_MATRIX_COUNT 55";
			}
			else
			{
				sMaterFlag = "DIFFUSE";
			}
		
			Material* pMaterial = new Material(sMaterFlag.c_str(),"default");
			
			pMaterial->GetParameter("u_texture")->setSampler(sampler);

			SetMaterial(pMaterial);

			
		}

		return true;
	}

	bool RenderMesh::InitWithData(MeshData* pMeshData)
	{
		if (pMeshData == NULL)
			return false;

		m_pMesData = pMeshData;

		void* pIndexData = pMeshData->GetIndexBuffer();
		int nIndexSize = pMeshData->GetIndexBufferSize();
		INDEX_TYPE eIndexType = pMeshData->GetIndexType() == INDEX_TYPE_U16 ? INDEX_TYPE_U16 : INDEX_TYPE_U32; 
		IndexBuffer* pIndexBuffer = GetRenderDevice()->CreateIndexBuffer(pIndexData,nIndexSize,eIndexType);
		pIndexBuffer->Active();

		void* pVertexData = pMeshData->GetVertexBuffer();
		int nVertexDataSize = pMeshData->GetVertexBufferSize();
		int nVertexStride = pMeshData->GetVertexStride();
		VertexBuffer* pVertexBuffer = GetRenderDevice()->CreateVertexBuffer(pVertexData,nVertexDataSize, nVertexStride);
		pVertexBuffer->Active();
		
		VertexDeclaration* pDeclaration = GetRenderDevice()->CreateVertexDeclaration();	
		pDeclaration->Init( pMeshData->GetVertexType() );
		pDeclaration->Active();

		for (UINT i = 0; i < pMeshData->GetSubMeshNumber(); ++i)
		{
			SubMeshData* pSubMesh = pMeshData->GetSubMeshByIndex(i);
			ASSERT(pSubMesh);
			if (pSubMesh == NULL)
				continue;

			Renderable* pRenderable = new Renderable();
			pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
			pRenderable->m_pDeclaration = pDeclaration;
			pRenderable->m_pVertexBuffers = pVertexBuffer;
			pRenderable->m_pIndexBuffer = pIndexBuffer;
			pRenderable->m_nIndexCount = pSubMesh->m_nIndexCount;
			pRenderable->m_nIndexStart = pSubMesh->m_nIndexStart;
			pRenderable->m_nVertexStart = pSubMesh->m_nVertexStart;
			pRenderable->m_nVertexCount = pSubMesh->m_nVertexCount;

			Uint nBone = pSubMesh->m_arrBonePalette.size();
			pRenderable->m_arrBonePalette.resize(nBone);
			pRenderable->m_arrSkinMatrix.resize(nBone);
			for (Uint i = 0; i < nBone; ++i)
			{
				m_bSkin = true;
				pRenderable->m_arrBonePalette[i] = pSubMesh->m_arrBonePalette[i];
				pRenderable->m_arrSkinMatrix[i] = Matrix4x4::identity();
			}

			m_arrRenderable.push_back(pRenderable);
		}

		return true;
	}


	void RenderMesh::SetMaterial(Material* pMaterial)
	{
		m_pMaterial = pMaterial;

		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->m_pMaterial = pMaterial;
			m_pMaterial->SetRenderable(m_arrRenderable[i]);
		}
	}

	void RenderMesh::SetSkinMatrix(const Matrix4x4* arrMatrixs,Uint nCount)
	{
		for (UINT iRenderable = 0; iRenderable < m_arrRenderable.size(); ++iRenderable)
		{
			Renderable* pRenderable = m_arrRenderable[iRenderable];
			ASSERT(pRenderable);
			if (pRenderable == NULL)
				continue;

			UINT nBone = pRenderable->m_arrBonePalette.size();
			ASSERT(pRenderable->m_arrSkinMatrix.size() == nBone);
			if (pRenderable->m_arrSkinMatrix.size() != nBone)
				continue;

			for (Uint iBone = 0; iBone < nBone; ++iBone)
			{
				BoneIndex boneID = pRenderable->m_arrBonePalette[iBone];
				ASSERT(boneID >=0 && boneID < nCount);
				if (boneID < 0 || boneID >= nCount)
					continue;

				pRenderable->m_arrSkinMatrix[iBone] = arrMatrixs[boneID];
			}
		}
	}

	void RenderMesh::SetWorldMatrix(const Matrix4x4& matWorld)
	{
		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->m_matWorld = matWorld;
		}
	}

	void RenderMesh::Draw()
	{
		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
 		{
			RenderQueue::AddRenderable(m_arrRenderable[i]);
 		}
		

// 		if (pTechName)
// 		{
// 			m_pMaterial->SetCurTechnqiue(pTechName);
// 		}
// 
// 		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
// 		{
// 			GetRenderDevice()->DrawRenderable(m_arrRenderable[i]);
// 		}
	}
}

