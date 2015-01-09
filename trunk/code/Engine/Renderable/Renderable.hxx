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

	const DualQuaternion* Renderable::GetSkinDQ() const
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		return &arrSkinDQ[0];
	}

	uint32 Renderable::GetSkinDQCount() const 
	{
		int index = GetRenderSystem()->CurThreadProcess();
		const VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];
		return arrSkinDQ.size();
	}

	void Renderable::SetSkinMatrix(const Matrix4* arrMatrixs, uint32 nCount)
	{
		profile_code();

		int index = GetRenderSystem()->CurThreadFill();
		VEC_DQ& arrSkinDQ = m_arrSkinDQ[index];

		if (m_pSubMeshData->m_arrBonePalette.empty())
		{
			arrSkinDQ.resize(nCount);

			for (uint32 i = 0; i < nCount; ++i)
			{
				Matrix4 matSkin = arrMatrixs[i];
				matSkin[3][3] = 1.f;

				Vector3 pos;
				Vector3 scale;
				Quaternion rot;
				matSkin.decomposition(pos, scale, rot);
				arrSkinDQ[i] = DualQuaternion(rot, pos);
			}	
		}
		else
		{
			arrSkinDQ.resize( m_pSubMeshData->m_arrBonePalette.size() );
			for (uint32 i = 0; i < m_pSubMeshData->m_arrBonePalette.size(); ++i)
			{
				BoneIndex boneID = m_pSubMeshData->m_arrBonePalette[i];
				ASSERT(boneID >=0 && boneID < nCount);
				if (boneID < 0 || boneID >= nCount)
					continue;

				Matrix4 matSkin = arrMatrixs[boneID];
				matSkin[3][3] = 1.f;

				Vector3 pos;
				Vector3 scale;
				Quaternion rot;
				matSkin.decomposition(pos, scale, rot);
				arrSkinDQ[i] = DualQuaternion(rot, pos);
			}
		}
	}
}

