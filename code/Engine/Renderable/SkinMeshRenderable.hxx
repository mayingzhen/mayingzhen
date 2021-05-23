#include "Renderable.h"
#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	void SkinMeshRenderable::PreRender(Technique* pTech, SceneContext* sc)
	{
 		pTech->Bind(this, sc);
		
		Uniform* pUniformBoneDQ = pTech->GetUniform(VS, "boneDQ");
		Uniform* pUniformBoneMatrix = pTech->GetUniform(VS, "boneMatrix");

		if (pUniformBoneDQ)
		{
			const DualQuaternion* pDQ = GetSkinDQ();
			uint32_t nNumDQ = GetSkinDQCount();	
			pTech->SetValue(pUniformBoneDQ, (const Vector4*)pDQ, nNumDQ * 2 );
		}
		else
		{
			const Matrix3x4* pMatrix = GetSkinMatrix();
			uint32_t nNumMatrix = GetSkinMatrixCount();
			pTech->SetValue(pUniformBoneMatrix, (const Vector4*)pMatrix, nNumMatrix * 3 );
		}
	}

	const DualQuaternion* SkinMeshRenderable::GetSkinDQ()
	{
		int index = GetRenderSystem()->CurThreadProcess();
		VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		if (arrSkinDQ.empty())
		{
			uint32_t nBoneNum = m_pSubMeshData->m_arrBonePalette.size();
			arrSkinDQ.resize(nBoneNum);
		};

		return &arrSkinDQ[0];
	}

	uint32_t SkinMeshRenderable::GetSkinDQCount() const 
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		return arrSkinDQ.size();
	}

	uint32_t SkinMeshRenderable::GetSkinMatrixCount() const
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_MATRIX& arrSkinDQ = m_arrSkinMatrix[index];
		return arrSkinDQ.size();
	}

	const Matrix3x4* SkinMeshRenderable::GetSkinMatrix()
	{
		int index = GetRenderSystem()->CurThreadProcess();
		VEC_MATRIX& arrSkinMatrix = m_arrSkinMatrix[index];
		if (arrSkinMatrix.empty())
		{
			uint32_t nBoneNum = m_pSubMeshData->m_arrBonePalette.size();
			arrSkinMatrix.resize(nBoneNum,Matrix3x4::IDENTITY);
		}

		return &arrSkinMatrix[0];
	}

	void SkinMeshRenderable::SetSkinMatrix(const Matrix3x4* arrMatrixs, uint32_t nCount)
	{
		int index = GetRenderSystem()->CurThreadFill();
		VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		VEC_MATRIX& arrSkinMatrix = m_arrSkinMatrix[index];

		if (m_pSubMeshData->m_arrBonePalette.empty())
		{
			arrSkinDQ.resize(nCount);
			arrSkinMatrix.resize(nCount);

			for (uint32_t i = 0; i < nCount; ++i)
			{
				const Matrix3x4& matSkin = arrMatrixs[i];

				Vector3 pos;
				Vector3 scale;
				Quaternion rot;
				matSkin.Decompose(pos, rot, scale);
				arrSkinDQ[i] = DualQuaternion(rot, pos);
				arrSkinMatrix[i] = matSkin;
			}	
		}
		else
		{
			uint32_t nBoneCount = m_pSubMeshData->m_arrBonePalette.size();
			arrSkinDQ.resize( nBoneCount );
			arrSkinMatrix.resize(nCount);
			for (uint32_t i = 0; i < nBoneCount; ++i)
			{
				BoneIndex boneID = m_pSubMeshData->m_arrBonePalette[i];
				ASSERT(boneID >=0 && boneID < nCount);
				if (boneID < 0 || boneID >= nCount)
					continue;

				const Matrix3x4& matSkin = arrMatrixs[boneID];

				Vector3 pos;
				Vector3 scale;
				Quaternion rot;
				matSkin.Decompose(pos, rot, scale);
				arrSkinDQ[i] = DualQuaternion(rot, pos);
				arrSkinMatrix[i] = matSkin;
			}
		}
	}
}

