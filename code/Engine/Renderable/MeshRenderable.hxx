#include "Renderable.h"
#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	MeshRenderable::MeshRenderable()
	{

	}

	void MeshRenderable::Render(Technique* pTech)
	{
		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		SetPosUVAABB(pTech);

		pTech->CommitChanges();

		GetRenderSystem()->DrawRenderable(this,pTech);
	}

	void MeshRenderable::SetPosUVAABB(Technique* pTech)
	{
		ShaderProgram* pShader = pTech->GetShaderProgram();

		if (pShader->GetUniform("pos_extent") == NULL)
			return;

		Vector3 pos_extent = m_posAABB.getHalfSize();
		Vector3 pos_center = m_posAABB.getCenter();
		Vector2 tc_extent = m_tcAABB.getHalfSize();
		Vector2	tc_center = m_tcAABB.getCenter(); 
		Vector4 tc_extent_center = Vector4(tc_extent.x,tc_extent.y,tc_center.x,tc_center.y);
		GetRenderSystem()->SetValue( pShader->GetUniform("pos_extent"), pos_extent );
		GetRenderSystem()->SetValue( pShader->GetUniform("pos_center"), pos_center );
		GetRenderSystem()->SetValue( pShader->GetUniform("tc_extent_center"), tc_extent_center );
	}
}

