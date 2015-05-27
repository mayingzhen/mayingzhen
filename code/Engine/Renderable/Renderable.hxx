#include "Renderable.h"
#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	Renderable::Renderable()
	{
		m_ePrimitiveType = PRIM_TRIANGLESTRIP;
		m_matWorld[0] = Matrix4::IDENTITY;
		m_matWorld[1] = Matrix4::IDENTITY;
	}

	void Renderable::Render(Technique* pTech)
	{
		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		GetRenderSystem()->DrawRenderable(this,pTech);
	}

	void Renderable::SetWorldMatrix(const Matrix4& matWS)
	{
		int index = GetRenderSystem()->CurThreadFill();
		m_matWorld[index] = matWS;
	}

	const Matrix4& Renderable::GetWorldMatrix() const 
	{
		int index = GetRenderSystem()->CurThreadProcess();
		return m_matWorld[index];
	}

	void SkinRenderable::Render(Technique* pTech)
	{
		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		ShaderProgram* pShader = pTech->GetShaderProgram();
		
		uint32 nNumBones = GetSkinDQCount();

		const DualQuaternion* pDQ = GetSkinDQ();
		const float* pScale = GetSkinScale();

		GetRenderSystem()->SetValue( pShader->GetUniform("boneDQ"), (const Vector4*)pDQ, nNumBones * 2 );
		GetRenderSystem()->SetValue( pShader->GetUniform("boneScale"), (const Vector4*)pScale, Math::ICeil(nNumBones / 4.0f) );

		GetRenderSystem()->DrawRenderable(this,pTech);
	}


	const DualQuaternion* SkinRenderable::GetSkinDQ() const
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		if (arrSkinDQ.empty())
			return NULL;

		return &arrSkinDQ[0];
	}

	const float* SkinRenderable::GetSkinScale() const
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_SCALE& arrSkinScale = m_arrScale[index];
		if (arrSkinScale.empty())
			return NULL;

		return &arrSkinScale[0];
	}

	uint32 SkinRenderable::GetSkinDQCount() const 
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		return arrSkinDQ.size();
	}

	void SkinRenderable::SetSkinMatrix(const Matrix3x4* arrMatrixs, uint32 nCount)
	{
		profile_code();

		int index = GetRenderSystem()->CurThreadFill();
		VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		VEC_SCALE& arrScale = m_arrScale[index];

		if (m_pSubMeshData->m_arrBonePalette.empty())
		{
			arrSkinDQ.resize(nCount);
			arrScale.resize(Math::ICeil(nCount / 4.0f) * 4);

			for (uint32 i = 0; i < nCount; ++i)
			{
				const Matrix3x4& matSkin = arrMatrixs[i];

				Vector3 pos;
				Vector3 scale;
				Quaternion rot;
				matSkin.Decompose(pos, rot, scale);
				arrSkinDQ[i] = DualQuaternion(rot, pos);
				arrScale[i] = scale.x;
			}	
		}
		else
		{
			uint32 nBoneCount = m_pSubMeshData->m_arrBonePalette.size();
			arrSkinDQ.resize( nBoneCount );
			arrScale.resize(Math::ICeil(nBoneCount / 4.0f) * 4);
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
			}
		}
	}
}

