#include "Renderable.h"
#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	static vector<Matrix3x4> s_vecMatrix;
	static vector<DualQuaternion> s_vecDQ;
	static vector<float> s_vecScale;


	void SkinMeshRenderable::Render(Technique* pTech)
	{
		if (s_vecMatrix.empty())
		{
			s_vecMatrix.resize(100, Matrix3::IDENTITY);
			s_vecDQ.resize(100, DualQuaternion());
			s_vecScale.resize(100, 1.f);
		}

		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		SetPosUVAABB(pTech);

		ShaderProgram* pShader = pTech->GetShaderProgram();
		
		uint32 nNumBones = GetSkinDQCount();

		const DualQuaternion* pDQ = GetSkinDQ();
		const float* pScale = GetSkinScale();
		const Matrix3x4* pMatrix = GetSkinMatrix();
		
		if (pShader->GetUniform("boneDQ"))
		{
			GetRenderSystem()->SetValue( pShader->GetUniform("boneDQ"), (const Vector4*)pDQ, nNumBones * 2 );
			GetRenderSystem()->SetValue( pShader->GetUniform("boneScale"), (const Vector4*)pScale, Math::ICeil(nNumBones / 4.0f) );
		}
		else
		{
			GetRenderSystem()->SetValue( pShader->GetUniform("boneMatrix"), (const Vector4*)pMatrix, 3 * nNumBones );
		}

		GetRenderSystem()->DrawRenderable(this,pTech);
	}


	const DualQuaternion* SkinMeshRenderable::GetSkinDQ() const
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		if (arrSkinDQ.empty())
			return NULL;

		return &arrSkinDQ[0];
	}

	const float* SkinMeshRenderable::GetSkinScale() const
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_SCALE& arrSkinScale = m_arrScale[index];
		if (arrSkinScale.empty())
			return NULL;

		return &arrSkinScale[0];
	}

	uint32 SkinMeshRenderable::GetSkinDQCount() const 
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		return arrSkinDQ.size();
	}

	const Matrix3x4* SkinMeshRenderable::GetSkinMatrix() const
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_MATRIX& arrSkinMatrix = m_arrSkinMatrix[index];
		return arrSkinMatrix.empty() ? NULL : &arrSkinMatrix[0];
	}

	void SkinMeshRenderable::SetSkinMatrix(const Matrix3x4* arrMatrixs, uint32 nCount)
	{
		profile_code();

		int index = GetRenderSystem()->CurThreadFill();
		VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		VEC_SCALE& arrScale = m_arrScale[index];
		VEC_MATRIX& arrSkinMatrix = m_arrSkinMatrix[index];

		if (m_pSubMeshData->m_arrBonePalette.empty())
		{
			arrSkinDQ.resize(nCount);
			arrScale.resize(Math::ICeil(nCount / 4.0f) * 4,1.0f);
			arrSkinMatrix.resize(nCount);

			for (uint32 i = 0; i < nCount; ++i)
			{
				const Matrix3x4& matSkin = arrMatrixs[i];

				Vector3 pos;
				Vector3 scale;
				Quaternion rot;
				matSkin.Decompose(pos, rot, scale);
				arrSkinDQ[i] = DualQuaternion(rot, pos);
				arrScale[i] = scale.x;
				arrSkinMatrix[i] = matSkin;
			}	
		}
		else
		{
			uint32 nBoneCount = m_pSubMeshData->m_arrBonePalette.size();
			arrSkinDQ.resize( nBoneCount );
			arrScale.resize(Math::ICeil(nBoneCount / 4.0f) * 4);
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
				arrScale[i] = scale.x;
				arrSkinMatrix[i] = matSkin;
			}
		}
	}
}

