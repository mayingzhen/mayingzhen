#include "Screen.h"

namespace ma
{
	ScreenRenderProxy::ScreenRenderProxy()
	{
		m_pRenderable = ScreenQuad::CreateRenderable();

		m_pRenderable->m_nRenderOrder = RL_Transluce;

	}

	void ScreenRenderProxy::SetTech(const char* pszTech)
	{
		RefPtr<Technique> pTech = CreateTechnique(pszTech, "", nullptr);
		RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
		pSubMaterial->SetShadingTechnqiue(pTech.get());

		m_pRenderable->m_pSubMaterial = pSubMaterial;
	}

	ScreenRender::ScreenRender()
	{
		m_pRenderproxy = new ScreenRenderProxy();

		AABB aabb;
		aabb.setInfinite();
		this->SetAABB(aabb);
	}

	void ScreenRender::SetTech(const char* pTech)
	{
		static_cast<ScreenRenderProxy*>(m_pRenderproxy.get())->SetTech(pTech);
	}

	RefPtr<ScreenRender> CreateScreenRender()
	{
		return new ScreenRender();
	}
}

