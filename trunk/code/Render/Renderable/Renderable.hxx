#include "Renderable.h"

namespace ma
{
	Renderable::Renderable()
	{
		m_ePrimitiveType = PRIM_TRIANGLESTRIP;
		MatrixIdentity(&m_matWorld[0]);
		MatrixIdentity(&m_matWorld[1]);
	}

	void Renderable::Render(Technique* pTech)
	{
		GetRenderSystem()->DrawRenderable(this,pTech);
	}

	void Renderable::SetWorldMatrix(const Matrix4x4& matWS)
	{
		int index = GetRenderSystem()->CurThreadFill();
		m_matWorld[index] = matWS;
	}


	void Renderable::SetSkinMatrix(const Matrix4x4* arrMatrixs, Uint nCount)
	{
		profile_code();

		int index = GetRenderSystem()->CurThreadFill();

		std::vector<Matrix4x4>& arrSkinMatrix = m_arrSkinMatrix[index];

		UINT nBone = m_pSubMeshData->m_arrBonePalette.size();
		ASSERT(arrSkinMatrix.size() == nBone);
		if (arrSkinMatrix.size() != nBone)
			return;

		for (Uint iBone = 0; iBone < nBone; ++iBone)
		{
			BoneIndex boneID = m_pSubMeshData->m_arrBonePalette[iBone];
			ASSERT(boneID >=0 && boneID < nCount);
			if (boneID < 0 || boneID >= nCount)
				continue;

			arrSkinMatrix[iBone] = arrMatrixs[boneID];
		}
	}
}

