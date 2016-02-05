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

		ShaderProgram* pShader = pTech->GetShaderProgram();

		Vector3 pos_extent = m_posAABB.getHalfSize() / 32767.5f;
		Vector3 pos_center = m_posAABB.getCenter();
		Vector2 tc_extent = m_tcAABB.getHalfSize() / 32767.5f; 
		Vector2	tc_center = m_tcAABB.getCenter(); 
		Vector4 tc_extent_center = Vector4(tc_extent.x,tc_extent.y,tc_center.x,tc_center.y);
		GetRenderSystem()->SetValue( pShader->GetUniform("pos_extent"), pos_extent );
		GetRenderSystem()->SetValue( pShader->GetUniform("pos_center"), pos_center );
		GetRenderSystem()->SetValue( pShader->GetUniform("tc_extent_center"), tc_extent_center );

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

		Vector3 pos_extent = m_posAABB.getHalfSize() / 32767.5f;
		Vector3 pos_center = m_posAABB.getCenter();
		Vector2 tc_extent = m_tcAABB.getHalfSize() / 32767.5f; 
		Vector2	tc_center = m_tcAABB.getCenter(); 
		Vector4 tc_extent_center = Vector4(tc_extent.x,tc_extent.y,tc_center.x,tc_center.y);
		GetRenderSystem()->SetValue( pShader->GetUniform("pos_extent"), pos_extent );
		GetRenderSystem()->SetValue( pShader->GetUniform("pos_center"), pos_center );
		GetRenderSystem()->SetValue( pShader->GetUniform("tc_extent_center"), tc_extent_center );
		
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

	const Matrix3x4* SkinRenderable::GetSkinMatrix() const
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_MATRIX& arrSkinMatrix = m_arrSkinMatrix[index];
		return &arrSkinMatrix[0];
	}

	void SkinRenderable::SetSkinMatrix(const Matrix3x4* arrMatrixs, uint32 nCount)
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

