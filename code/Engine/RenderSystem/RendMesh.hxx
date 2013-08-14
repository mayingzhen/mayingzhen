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

		m_sknPath = pMeshPath;
		m_texPath = pDiffueTexture;

		m_pMesData = DeclareResource<MeshData>(pMeshPath);
		ASSERT(m_pMesData);
		
		m_pMesData->LoadSync();

		InitWithData(m_pMesData);

		m_pTexture = DeclareResource<Texture>(pDiffueTexture);
		ASSERT(m_pTexture);

		m_pTexture->LoadAsync();

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

		pMaterial->GetParameter("u_texture")->setTexture(m_pTexture);

		SetMaterial(pMaterial);

		return true;
	}

	bool RenderMesh::InitWithData(MeshData* pMeshData)
	{
		if (pMeshData == NULL)
			return false;

		m_pMesData = pMeshData;

		for (UINT i = 0; i < pMeshData->GetSubMeshNumber(); ++i)
		{
			SubMeshData* pSubMesh = pMeshData->GetSubMeshByIndex(i);
			ASSERT(pSubMesh);
			if (pSubMesh == NULL)
				continue;

			Renderable* pRenderable = new Renderable();
			pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
			pRenderable->m_pDeclaration = pMeshData->GetVertexDeclar(); 
			pRenderable->m_pVertexBuffers = pMeshData->GetVertexBuffer(); 
			pRenderable->m_pIndexBuffer =  pMeshData->GetIndexBuffer();
			pRenderable->m_pSubMeshData = pSubMesh;

			Uint nBone = pSubMesh->m_arrBonePalette.size();
			pRenderable->m_arrSkinMatrix.resize(nBone);
			m_bSkin = nBone > 0 ? true : false;
			for (Uint i = 0; i < nBone; ++i)
			{
				pRenderable->m_arrSkinMatrix[i] = Matrix4x4::identity();
			}

			m_arrRenderable.push_back(pRenderable);
		}

		m_bSkin = true;

		return true;
	}


	void RenderMesh::SetMaterial(Material* pMaterial)
	{
		m_pMaterial = pMaterial;

		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->m_pMaterial = pMaterial;
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

			UINT nBone = pRenderable->m_pSubMeshData->m_arrBonePalette.size();
			ASSERT(pRenderable->m_arrSkinMatrix.size() == nBone);
			if (pRenderable->m_arrSkinMatrix.size() != nBone)
				continue;

			for (Uint iBone = 0; iBone < nBone; ++iBone)
			{
				BoneIndex boneID = pRenderable->m_pSubMeshData->m_arrBonePalette[iBone];
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

	AABB RenderMesh::GetBoundingAABB()
	{
		return m_pMesData->GetBoundingAABB();
	}

	void RenderMesh::Draw()
	{
// 		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
//  		{
// 			GetRenderSystem()->AddRenderable(m_arrRenderable[i]);
//  		}
		
		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			GetRenderDevice()->DrawRenderable(m_arrRenderable[i]);
		}
	}
}

