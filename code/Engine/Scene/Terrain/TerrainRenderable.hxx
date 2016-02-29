#include "TerrainRenderable.h"


namespace ma
{
	TerrainRenderable::TerrainRenderable(TerrainTrunk* pParent)
	{
		m_pParent = pParent;
		m_fMateriID = 0.0f;
		m_ePrimitiveType = PRIM_TRIANGLELIST;
	}

	TerrainRenderable::~TerrainRenderable()
	{

	}

	void TerrainRenderable::Render(Technique* pTech)
	{
		Terrain* pTerrain = m_pParent->GetTerrain();

		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		Uniform* pUniformBlenOffset = pTech->GetShaderProgram()->GetUniform("uBlendingOffset");
		Uniform* pUniformCellAmount = pTech->GetShaderProgram()->GetUniform("uCellAmount");
		Uniform* pUniformCurMaterialID= pTech->GetShaderProgram()->GetUniform("uCurMaterialID");

		if (pUniformBlenOffset)
		{
			GetRenderSystem()->SetValue(pUniformBlenOffset,pTerrain->GetBlendOffSet());
		}

		if (pUniformCellAmount)
		{
			GetRenderSystem()->SetValue(pUniformCellAmount,pTerrain->GetCellAmount());
		}
		
		if (pUniformCurMaterialID)
		{
			GetRenderSystem()->SetValue(pUniformCurMaterialID,m_fMateriID);
		}

		GetRenderSystem()->DrawRenderable(this,pTech);
	}
}

