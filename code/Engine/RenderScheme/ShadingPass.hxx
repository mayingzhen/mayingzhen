#include "ShadingPass.h"
#include "HDRPostProcess.h"

namespace ma
{
	ShadingPass::ShadingPass(Scene* pScene):RenderPass(pScene)
	{
 	}

	void ShadingPass::Init()
	{
	}

	void ShadingPass::Reset()
	{
		if (m_pHdrPostprocess)
		{
			m_pShadingTex = GetRenderSystem()->CreateRenderTexture(-1,-1,PF_FLOAT16_RGBA);	
		}
	}

	void ShadingPass::SetHDREnabled(bool b)
	{
		if (b && m_pHdrPostprocess == NULL)
		{
			RefPtr<Texture>	 pOutTagget = GetRenderSystem()->GetRenderTarget(0);

			//m_pHdrPostprocess = new HDRPostProcess(m_pScene,m_pShadingTex.get(),pOutTagget.get());
			//m_pHdrPostprocess->Init(nWidth,nHeight);
		}
	}

	bool ShadingPass::GetHDREnabled() const
	{
		return m_pHdrPostprocess != NULL;
	}

	void ShadingPass::RenderObjecList()
	{
		ColourValue cClearClor = GetRenderSystem()->GetClearColor();

		GetRenderSystem()->ClearBuffer(true,true,true,cClearClor, 1.0f, 0);

		RenderQueue* pRenderQueue = m_pScene->GetRenderQueue();

		{
			RENDER_PROFILE(RL_Mesh);
			pRenderQueue->RenderObjList(RL_Mesh);
		}

		{
			RENDER_PROFILE(RL_Terrain);
			pRenderQueue->RenderObjList(RL_Terrain);
		}

		{
			RENDER_PROFILE(RL_Particle);
			pRenderQueue->RenderObjList(RL_Particle);
		}

	}

	void ShadingPass::Render()
	{
		RENDER_PROFILE(ShadingPass);

		RefPtr<Texture> pPreTarget = NULL;
		if (m_pHdrPostprocess)
		{
			pPreTarget = GetRenderSystem()->SetRenderTarget(m_pShadingTex.get());
		}

		RenderObjecList();

		if (m_pHdrPostprocess)
		{
			GetRenderSystem()->SetRenderTarget(pPreTarget);

			m_pHdrPostprocess->Render();
		}
	}

	void ShadingPass::Shoutdown()
	{
	}
}

