#include "SkyBoxRenderable.h"

namespace ma
{
	SkyBoxRenderable::SkyBoxRenderable()
	{
		m_pSubMaterial = CreateSubMaterial();
		RefPtr<Technique> pTech = CreateTechnique("shader/SkyBox.tech", "", GetRenderSystem()->GetBaseRenderPass());
		m_pSubMaterial->SetShadingTechnqiue(pTech.get());
	}

	void SkyBoxRenderable::PreRender(Technique* pTech)
	{
		pTech->Bind(this);

		Matrix4 matView = GetSceneContext()->m_matViewProj.GetMatView();
		matView.setTrans(Vector3::ZERO);
		Matrix4 matViewProj = GetSceneContext()->m_matViewProj.GetMatProj() * matView;
		Matrix4 matInvViewProj = matViewProj.inverse();

		pTech->SetValue(pTech->GetUniform(VS,"inv_mvp"), matInvViewProj);
	}

	void SkyBoxRenderable::Render(Technique* pTechnique, RenderCommand* pRenderCommand)
	{
		ScreenQuad::GetRenderable()->Render(pTechnique, pRenderCommand);
	}

}




