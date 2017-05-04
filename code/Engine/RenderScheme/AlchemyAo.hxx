#include "AlchemyAo.h"

namespace ma
{

	AlchemyAo::AlchemyAo()
	{
	}

	AlchemyAo::~AlchemyAo()
	{
	}

	void AlchemyAo::Init()
	{
		m_pAlchemyAo = CreateTechnique("AlchemyAo","AlchemyAo","AlchemyAo","");
		//m_pAlchemyAo->m_eDepthCheckMode = CMPF_ALWAYS_PASS;

		mRandomMap = CreateSamplerState("white_noise_tex.dds",REPEAT,TFO_POINT,false);
		m_pAlchemyAo->SetParameter("tRandomMap",Any(mRandomMap));

		mBlur = new BlurPostProcess();
		mBlur->Init();
	}

	void AlchemyAo::Reset(Texture* pInput, Texture* pOutput)
	{
		mTexSSAO = GetRenderSystem()->CreateRenderTarget(-1,-1,1,PF_A8R8G8B8,false);
		
		mBlur->Reset(mTexSSAO.get(),mTexSSAO.get());
	}

	void AlchemyAo::Shutdown()
	{

	}

	void AlchemyAo::Render()
	{
		// ssao
		{
			FrameBuffer fb;
			fb.AttachColor(0,mTexSSAO.get());
			fb.AttachDepthStencil(GetRenderSystem()->GetDefaultDepthStencil().get());
			GetRenderSystem()->SetFrameBuffer(&fb);
			ScreenQuad::Render(m_pAlchemyAo.get());
		}

		// blur
		{
			mBlur->Render();
		}

		// blend
		{
			//CViewport viewport(pCamera, pPostProcess->SelectSrcColor()->GetBuffer()->GetRenderTarget(), 0, 0, 1, 1);
			//GetRenderSystem()->_setViewport(&viewport);
			//ScreenQuad::Render(mBlurV.get());
		}
	}
}