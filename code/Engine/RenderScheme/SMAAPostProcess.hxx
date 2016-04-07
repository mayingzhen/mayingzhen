#include "BlurPostProcess.h"

namespace ma
{
	SMAAPostProcess::SMAAPostProcess()
	{
		m_bLinearizeDepthEnabled = false;
	}

	SMAAPostProcess::~SMAAPostProcess()
	{
	}

	void SMAAPostProcess::Init()
	{
		if (m_bLinearizeDepthEnabled)
		{
			m_pDepthEdgeDetection = CreateTechnique("DepthEdgeDetection","smaa/DepthEdgeDetection","smaa/DepthEdgeDetection",""); 
			m_pDepthEdgeDetection->m_bStencil = true;
			m_pDepthEdgeDetection->m_eStencilfunc = CMPF_ALWAYS_PASS;
			m_pDepthEdgeDetection->m_nStencilRefValue = 1;
			m_pDepthEdgeDetection->m_nStencilMask = -1;
			m_pDepthEdgeDetection->m_nStencilWriteMask = -1;
			m_pDepthEdgeDetection->m_eStencilFail = SOP_KEEP;
			m_pDepthEdgeDetection->m_eDepthFailOp = SOP_KEEP;
			m_pDepthEdgeDetection->m_eStencilPass = SOP_REPLACE;

			//m_pDepthEdgeDetection->SaveToXML("tech/DepthEdgeDetection.tech");
		}
		else
		{
			m_pColorEdgeDetection = CreateTechnique("ColorEdgeDetection","smaa/coloredgedetection","smaa/coloredgedetection","");
			m_pColorEdgeDetection->m_bStencil = true;
			m_pColorEdgeDetection->m_eStencilfunc = CMPF_ALWAYS_PASS;
			m_pColorEdgeDetection->m_nStencilRefValue = 1;
			m_pColorEdgeDetection->m_nStencilMask = -1;
			m_pColorEdgeDetection->m_nStencilWriteMask = -1;
			m_pColorEdgeDetection->m_eStencilFail = SOP_KEEP;
			m_pColorEdgeDetection->m_eDepthFailOp = SOP_KEEP;
			m_pColorEdgeDetection->m_eStencilPass = SOP_REPLACE;

			//m_pColorEdgeDetection->SaveToXML("tech/ColorEdgeDetection.tech");
		}

		m_pBlendWeightCalculation = CreateTechnique("BlendWeightCalculation","smaa/BlendWeightCalculation","smaa/BlendWeightCalculation","");
		m_pBlendWeightCalculation->m_bStencil = true;
		m_pBlendWeightCalculation->m_eStencilfunc = CMPF_EQUAL;
		m_pBlendWeightCalculation->m_nStencilRefValue = 1;
		m_pBlendWeightCalculation->m_nStencilMask = -1;
		m_pBlendWeightCalculation->m_nStencilWriteMask = -1;
		m_pBlendWeightCalculation->m_eStencilFail = SOP_KEEP;
		m_pBlendWeightCalculation->m_eDepthFailOp = SOP_KEEP;
		m_pBlendWeightCalculation->m_eStencilPass = SOP_KEEP;

		m_pAreadTex = CreateTexture("AreaTexDX9.dds",CLAMP,TFO_BILINEAR,false);	
		m_pBlendWeightCalculation->SetParameter("areaTex",Any(m_pAreadTex));

		m_pSreachTex = CreateTexture("SearchTex.dds",CLAMP,TFO_POINT,false);
		m_pBlendWeightCalculation->SetParameter("searchTex",Any(m_pSreachTex));

		//m_pBlendWeightCalculation->SaveToXML("tech/BlendWeightCalculation.tech");

		m_pNeiborhoodBlending = CreateTechnique("NeiborhoodBlending","smaa/NeiborhoodBlending","smaa/NeiborhoodBlending","");
		m_pNeiborhoodBlending->m_bStencil = false;
		m_pNeiborhoodBlending->m_bSRGBWrite = true;

		//m_pNeiborhoodBlending->SaveToXML("tech/m_pNeiborhoodBlending.tech");
	}

	void SMAAPostProcess::Reset(Texture* pInput, Texture* pOutput)
	{
		int nWidth = pInput->GetWidth();
		int nHeight = pInput->GetHeight();

		Vector4 vRTMetrics(1.f / nWidth, 1.f / nHeight,(float)nWidth, (float)nHeight);

		m_pInputTex = pInput;
		m_pOutputTex = pOutput;
			

		m_pTexEdge = GetRenderSystem()->CreateRenderTexture(nWidth,nHeight,PF_A8R8G8B8,USAGE_RENDERTARGET); 

		m_pTexBlend = GetRenderSystem()->CreateRenderTexture(nWidth,nHeight,PF_A8R8G8B8,USAGE_RENDERTARGET);

		if (m_pDepthEdgeDetection)
		{
			m_pDepthEdgeDetection->SetParameter("tSrcColor",Any(m_pInputTex));
			m_pDepthEdgeDetection->SetParameter( "SMAA_RT_METRICS", Any(vRTMetrics) );
		}
		else
		{
			m_pColorEdgeDetection->SetParameter("tSrcColor",Any(m_pInputTex));
			m_pColorEdgeDetection->SetParameter( "SMAA_RT_METRICS", Any(vRTMetrics) );
		}

		m_pBlendWeightCalculation->SetParameter("tSrcColor",Any(m_pTexEdge));
		m_pBlendWeightCalculation->SetParameter( "SMAA_RT_METRICS", Any(vRTMetrics) );

		m_pNeiborhoodBlending->SetParameter("tSrcColor",Any(m_pInputTex));
		m_pNeiborhoodBlending->SetParameter("blendTex",Any(m_pTexBlend));
		m_pNeiborhoodBlending->SetParameter( "SMAA_RT_METRICS", Any(vRTMetrics) );
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

			ScreenQuad::Render(m_pBlendWeightCalculation.get());
		}

		// Pass3 NeiborhoodBlending
		{
			GetRenderSystem()->SetRenderTarget(m_pOutputTex.get(),0);

			m_pTexBlend->SetSRGB(true);

			ScreenQuad::Render(m_pNeiborhoodBlending.get());
		
			m_pTexBlend->SetSRGB(false);

			GetRenderSystem()->SetSRGBWite(false);
		}
	}

}
