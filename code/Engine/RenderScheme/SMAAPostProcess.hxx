#include "BlurPostProcess.h"

namespace ma
{
	SMAAPostProcess::SMAAPostProcess(Texture* pInputTex, Texture* pOutputTex)
		:PostProcess(pInputTex,pOutputTex)
	{
		m_bLinearizeDepthEnabled = false;
	}

	SMAAPostProcess::~SMAAPostProcess()
	{
	}

	void SMAAPostProcess::Init(int nWidth, int nHeight)
	{
		m_pDepthEdgeDetection = CreateTechnique("DepthEdgeDetection","smaa/DepthEdgeDetection","smaa/DepthEdgeDetection",""); 
		m_pColorEdgeDetection = CreateTechnique("ColorEdgeDetection","smaa/coloredgedetection","smaa/coloredgedetection","");
		m_pBlendWeightCalculation = CreateTechnique("BlendWeightCalculation","smaa/BlendWeightCalculation","smaa/BlendWeightCalculation","");
		m_pNeiborhoodBlending = CreateTechnique("NeiborhoodBlending","smaa/NeiborhoodBlending","smaa/NeiborhoodBlending","");

		m_pAreadTex = CreateTexture("_common/AreaTexDX9.dds");		

		m_pSreachTex = CreateTexture("_common/SearchTex.dds");
	}

	void SMAAPostProcess::Reset(int nWidth, int nHeight)
	{
		m_pTexEdge = GetRenderSystem()->CreateRenderTexture(nWidth,nHeight,PF_A8R8G8B8,USAGE_RENDERTARGET); 

		m_pTexBlend = GetRenderSystem()->CreateRenderTexture(nWidth,nHeight,PF_A8R8G8B8,USAGE_RENDERTARGET);

		m_pColorEdgeDetection->SetParameter("tSrcColor",Any(m_pInputTex));
		m_pDepthEdgeDetection->SetParameter("tSrcColor",Any(m_pInputTex));

		m_pBlendWeightCalculation->SetParameter("tSrcColor",Any(m_pTexEdge));
		m_pBlendWeightCalculation->SetParameter("areaTex",Any(m_pAreadTex));
		m_pBlendWeightCalculation->SetParameter("searchTex",Any(m_pSreachTex));

		m_pNeiborhoodBlending->SetParameter("tSrcColor",Any(m_pInputTex));
		m_pNeiborhoodBlending->SetParameter("blendTex",Any(m_pTexBlend));
	}

	void SMAAPostProcess::Shutdown()
	{

	}

	void SMAAPostProcess::Render()
	{
		RENDER_PROFILE(SMAAPostProcess);

		// Pass1 EdgeDetection
		GetRenderSystem()->SetRenderTarget(m_pTexEdge.get(),0);
		GetRenderSystem()->ClearBuffer(true,false,true, ColourValue::ZERO, 1.0f, 0);

		GetRenderSystem()->SetStencilCheckEnabled(true);
		GetRenderSystem()->SetStencilBufferParams(CMPF_ALWAYS_PASS, 1, -1, -1,
			SOP_KEEP, SOP_KEEP, SOP_REPLACE, false);

		if (m_bLinearizeDepthEnabled)
		{
			ScreenQuad::Render(m_pDepthEdgeDetection.get());
		}
		else 
		{
			ScreenQuad::Render(m_pColorEdgeDetection.get());
		}

		// Pass2 BlendWeightCalculation
		{
			GetRenderSystem()->SetRenderTarget(m_pTexBlend.get(),0);
			GetRenderSystem()->ClearBuffer(true,false,false, ColourValue::ZERO, 1.0f, 0);

			GetRenderSystem()->SetStencilBufferParams(CMPF_EQUAL, 1, -1, -1,
				SOP_KEEP, SOP_KEEP, SOP_KEEP, false);

			ScreenQuad::Render(m_pBlendWeightCalculation.get());
		}

		// Pass3 NeiborhoodBlending
		{
			GetRenderSystem()->SetStencilCheckEnabled(false);
			GetRenderSystem()->SetRenderTarget(m_pOutputTex.get(),0);
	
			ScreenQuad::Render(m_pNeiborhoodBlending.get());
		}
	}

}
