#include "AlchemyAo.h"

namespace ma
{

	AlchemyAo::AlchemyAo(void)
	{
	}

	AlchemyAo::~AlchemyAo(void)
	{
	}

	void AlchemyAo::Init()
	{
		m_pAlchemyAo = 
	
		m_pAlchemyAo = CreateTechnique("AlchemyAo","AlchemyAo","AlchemyAo","");
		m_pAlchemyAo->m_eDepthCheckMode = CMPF_ALWAYS_PASS;
	}

	void AlchemyAo::Reset(Texture* pInput, Texture* pOutput)
	{
		mTexSSAO = GetRenderSystem()->CreateRenderTarget();
		
		mTexBlur0 = GetRenderSystem()->CreateRenderTarget();
		
		mTexBlur1 = GetRenderSystem()->CreateRenderTarget();
	}

	void AlchemyAo::Shutdown()
	{

	}

	void AlchemyAo::Render()
	{
		CViewport* pPreViewport = GetRenderSystem()->_getViewport();

		// ssao
		{
			CViewport viewport(pCamera, mTexSSAO->GetBuffer()->GetRenderTarget(), 0, 0, 1, 1);
			GetRenderSystem()->_setViewport(&viewport);

			mSSAO.RenderSelf(pCamera, pPostProcess->GetNormalMap(), pPostProcess->GetPosMap());
		}

		// blurh
		{
			CViewport viewport(pCamera, mTexBlur1->GetBuffer()->GetRenderTarget(), 0, 0, 1, 1);
			GetRenderSystem()->_setViewport(&viewport);
			mBlurH.Render(pCamera, mTexSSAO.get(), NULL);
		}

		// blurv
		{
			CViewport viewport(pCamera, mTexBlur0->GetBuffer()->GetRenderTarget(), 0, 0, 1, 1);
			GetRenderSystem()->_setViewport(&viewport);
			mBlurV.Render(pCamera, mTexBlur1.get(), NULL);
		}

		// blend
		{
			CViewport viewport(pCamera, pPostProcess->SelectSrcColor()->GetBuffer()->GetRenderTarget(), 0, 0, 1, 1);
			GetRenderSystem()->_setViewport(&viewport);
			mSSAOBlend.Render(pCamera, mTexBlur0.get(), NULL);
		}

		GetRenderSystem()->_setViewport(pPreViewport);
	}
}