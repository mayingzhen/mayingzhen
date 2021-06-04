#include "SkyBoxRenderable.h"

namespace ma
{
	SkyBoxRenderable::SkyBoxRenderable()
	{
		m_pSubMaterial = CreateSubMaterial();
		RefPtr<Technique> pTech = CreateTechnique("shader/SkyBox.tech", "", GetRenderSystem()->GetBaseRenderPass());
		m_pSubMaterial->SetShadingTechnqiue(pTech.get());
	}

	void SkyBoxRenderable::Render(Technique* pTechnique, RenderCommand* pRenderCommand, SceneContext* sc)
	{
		Matrix4 matView = sc->m_matViewProj.GetMatView();
		matView.setTrans(Vector3::ZERO);
		Matrix4 matViewProj = sc->m_matViewProj.GetMatProj() * matView;
		Matrix4 matInvViewProj = matViewProj.inverse();

		pTechnique->SetValue(pTechnique->GetUniform(VS, "inv_mvp"), matInvViewProj);

		ScreenQuad::GetRenderable()->Render(pTechnique, pRenderCommand, sc);
	}

}




