#include "Renderable.h"
#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
		
	void SkinMeshRenderable::Render(Technique* pTech)
	{
		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		SetPosUVAABB(pTech);

		ShaderProgram* pShader = pTech->GetShaderProgram();
		
		if (pShader->GetUniform("boneDQ"))
		{
			const DualQuaternion* pDQ = GetSkinDQ();
			uint32 nNumDQ = GetSkinDQCount();	
			GetRenderSystem()->SetValue( pShader->GetUniform("boneDQ"), (const Vector4*)pDQ, nNumDQ * 2 );
		}
		else
		{
			const Matrix3x4* pMatrix = GetSkinMatrix();
			UINT32 nNumMatrix = GetSkinMatrixCount();
			GetRenderSystem()->SetValue( pShader->GetUniform("boneMatrix"), (const Vector4*)pMatrix, nNumMatrix * 3 );
		}

		pShader->CommitChanges();

		GetRenderSystem()->DrawRenderable(this,pTech);
	}


	const DualQuaternion* SkinMeshRenderable::GetSkinDQ()
	{
		int index = GetRenderSystem()->CurThreadProcess();
		VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		if (arrSkinDQ.empty())
		{
			uint32 nBoneNum = m_pSubMeshData->m_arrBonePalette.size();
			arrSkinDQ.resize(nBoneNum);
		};

		return &arrSkinDQ[0];
	}

	uint32 SkinMeshRenderable::GetSkinDQCount() const 
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		return arrSkinDQ.size();
	}

	uint32 SkinMeshRenderable::GetSkinMatrixCount() const
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
			uint32 nBoneNum = m_pSubMeshData->m_arrBonePalette.size();
			arrSkinMatrix.resize(nBoneNum,Matrix3x4::IDENTITY);
		}

		return &arrSkinMatrix[0];
	}

	void SkinMeshRenderable::SetSkinMatrix(const Matrix3x4* arrMatrixs, uint32 nCount)
	{
		profile_code();

		int index = GetRenderSystem()->CurThreadFill();
		VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		VEC_MATRIX& arrSkinMatrix = m_arrSkinMatrix[index];

		if (m_pSubMeshData->m_arrBonePalette.empty())
		{
			arrSkinDQ.resize(nCount);
			arrSkinMatrix.resize(nCount);

			for (uint32 i = 0; i < nCount; ++i)
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
			uint32 nBoneCount = m_pSubMeshData->m_arrBonePalette.size();
			arrSkinDQ.resize( nBoneCount );
			arrSkinMatrix.resize(nCount);
			for (uint32 i = 0; i < nBoneCount; ++i)
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

