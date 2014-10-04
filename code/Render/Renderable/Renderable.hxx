#include "Renderable.h"

namespace ma
{
	Renderable::Renderable()
	{
		m_ePrimitiveType = PRIM_TRIANGLESTRIP;
		m_matWorld[0] = Matrix4::IDENTITY;
		m_matWorld[1] = Matrix4::IDENTITY;
	}

	void Renderable::InitWithMeshData(const MeshData& meshData,int index)
	{
		m_ePrimitiveType = PRIM_TRIANGLELIST;
		m_pDeclaration = meshData.GetVertexDeclar(); 
		m_pVertexBuffers = meshData.GetVertexBuffer(); 
		m_pIndexBuffer = meshData.GetIndexBuffer();
		m_pSubMeshData = meshData.GetSubMeshByIndex(index);

		Uint nBone = m_pSubMeshData->m_arrBonePalette.size();
		m_arrSkinDQ[0].resize(nBone);
		m_arrSkinDQ[1].resize(nBone);
	}

	void Renderable::Render(Technique* pTech)
	{
		GetRenderSystem()->DrawRenderable(this,pTech);
	}

	void Renderable::SetWorldMatrix(const Matrix4& matWS)
	{
		int index = GetRenderSystem()->CurThreadFill();
		m_matWorld[index] = matWS;
	}


	void Renderable::SetSkinMatrix(const Matrix4* arrMatrixs, Uint nCount)
	{
		profile_code();

		int index = GetRenderSystem()->CurThreadFill();

		VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];

		UINT nBone = m_pSubMeshData->m_arrBonePalette.size();
		ASSERT(arrSkinDQ.size() == nBone);
		if (arrSkinDQ.size() != nBone)
			return;

		for (Uint iBone = 0; iBone < nBone; ++iBone)
		{
			BoneIndex boneID = m_pSubMeshData->m_arrBonePalette[iBone];
			ASSERT(boneID >=0 && boneID < nCount);
			if (boneID < 0 || boneID >= nCount)
				continue;

			Matrix4 matSkin = arrMatrixs[boneID];
			matSkin[3][3] = 1.f;

			Vector3 pos;
			Vector3 scale;
			Quaternion rot;
			matSkin.decomposition(pos, scale, rot);
			arrSkinDQ[iBone] = DualQuaternion(rot, pos);
		}
	}
}

