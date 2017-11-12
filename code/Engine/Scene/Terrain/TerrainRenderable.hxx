#include "TerrainRenderable.h"


namespace ma
{
	TerrainRenderable::TerrainRenderable(TerrainTrunk* pParent)
	{
		m_fMateriID = 0.0f;
		m_ePrimitiveType = PRIM_TRIANGLELIST;
	}

	TerrainRenderable::~TerrainRenderable()
	{

	}

	void TerrainRenderable::UpdateUniform(TerrainTrunk* pParent)
	{
		Technique* pTech = this->m_pSubMaterial->GetShadingTechnqiue();

		Vector2 vBlendOffSet = pParent->GetTerrain()->GetBlendOffSet();
		Vector2 vCellAmount = pParent->GetTerrain()->GetCellAmount();
		AABB posAABB = pParent->GetAABB();
		AABB2D tcAABB = pParent->GetUVAABB();

		Uniform* pUniformBlenOffset = pTech->GetUniform("uBlendingOffset");
		Uniform* pUniformCellAmount = pTech->GetUniform("uCellAmount");
		Uniform* pUniformCurMaterialID = pTech->GetUniform("uCurMaterialID");

		if (pUniformBlenOffset)
		{
			pTech->SetValue(pUniformBlenOffset, vBlendOffSet);
		}

		if (pUniformCellAmount)
		{
			pTech->SetValue(pUniformCellAmount, vCellAmount);
		}

		if (pUniformCurMaterialID)
		{
			pTech->SetValue(pUniformCurMaterialID, m_fMateriID);
		}

		Vector3 pos_extent = posAABB.getHalfSize();
		Vector3 pos_center = posAABB.getCenter();
		Vector2 tc_extent = tcAABB.getHalfSize();
		Vector2	tc_center = tcAABB.getCenter();
		Vector4 tc_extent_center = Vector4(tc_extent.x, tc_extent.y, tc_center.x, tc_center.y);
		pTech->SetValue(pTech->GetUniform("pos_extent"), pos_extent);
		pTech->SetValue(pTech->GetUniform("pos_center"), pos_center);
		pTech->SetValue(pTech->GetUniform("tc_extent_center"), tc_extent_center);
	}

	void TerrainRenderable::PreRender(Technique* pTech)
	{
		pTech->Bind(this);
	}

}

