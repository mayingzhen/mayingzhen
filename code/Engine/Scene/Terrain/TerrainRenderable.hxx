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
		Vector2 vBlendOffSet = pParent->GetTerrain()->GetBlendOffSet();
		Vector2 vCellAmount = pParent->GetTerrain()->GetCellAmount();
		AABB posAABB = pParent->GetAABB();
		AABB2D tcAABB = pParent->GetUVAABB();
		Vector3 pos_extent = posAABB.getHalfSize();
		Vector3 pos_center = posAABB.getCenter();
		Vector2 tc_extent = tcAABB.getHalfSize();
		Vector2	tc_center = tcAABB.getCenter();
		Vector4 tc_extent_center = Vector4(tc_extent.x, tc_extent.y, tc_center.x, tc_center.y);

		m_pSubMaterial->SetParameter("uBlendingOffset", Any(vBlendOffSet));
		m_pSubMaterial->SetParameter("uCellAmount", Any(vCellAmount));
		m_pSubMaterial->SetParameter("uCurMaterialID", Any(m_fMateriID));

		m_pSubMaterial->SetParameter("pos_extent", Any(pos_extent));
		m_pSubMaterial->SetParameter("pos_center", Any(pos_center));
		m_pSubMaterial->SetParameter("tc_extent_center", Any(tc_extent_center));
	}

}

