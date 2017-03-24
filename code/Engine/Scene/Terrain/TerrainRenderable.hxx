#include "TerrainRenderable.h"


namespace ma
{
	TerrainRenderable::TerrainRenderable(TerrainTrunk* pParent)
	{
		m_vBlendOffSet = pParent->GetTerrain()->GetBlendOffSet();
		m_vCellAmount = pParent->GetTerrain()->GetCellAmount();
		m_posAABB = pParent->GetAABB();
		m_tcAABB = pParent->GetUVAABB();

		m_fMateriID = 0.0f;
		m_ePrimitiveType = PRIM_TRIANGLELIST;
	}

	TerrainRenderable::~TerrainRenderable()
	{

	}

	void TerrainRenderable::Render(Technique* pTech)
	{
		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		ShaderProgram* pShader = pTech->GetShaderProgram();

		Uniform* pUniformBlenOffset = pShader->GetUniform("uBlendingOffset");
		Uniform* pUniformCellAmount = pShader->GetUniform("uCellAmount");
		Uniform* pUniformCurMaterialID = pShader->GetUniform("uCurMaterialID");

		if (pUniformBlenOffset)
		{
			GetRenderSystem()->SetValue(pUniformBlenOffset, m_vBlendOffSet);
		}

		if (pUniformCellAmount)
		{
			GetRenderSystem()->SetValue(pUniformCellAmount,m_vCellAmount);
		}
		
		if (pUniformCurMaterialID)
		{
			GetRenderSystem()->SetValue(pUniformCurMaterialID,m_fMateriID);
		}

		Vector3 pos_extent = m_posAABB.getHalfSize();
		Vector3 pos_center = m_posAABB.getCenter();
		Vector2 tc_extent = m_tcAABB.getHalfSize();
		Vector2	tc_center = m_tcAABB.getCenter();
		Vector4 tc_extent_center = Vector4(tc_extent.x,tc_extent.y,tc_center.x,tc_center.y);
		GetRenderSystem()->SetValue( pShader->GetUniform("pos_extent"), pos_extent );
		GetRenderSystem()->SetValue( pShader->GetUniform("pos_center"), pos_center );
		GetRenderSystem()->SetValue( pShader->GetUniform("tc_extent_center"), tc_extent_center );

		GetRenderSystem()->DrawRenderable(this,pTech);
	}
}

