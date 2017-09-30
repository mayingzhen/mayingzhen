#include "BlurPostProcess.h"

namespace ma
{
	SMAAPostProcess::SMAAPostProcess()
	{
	}

	SMAAPostProcess::~SMAAPostProcess()
	{
	}

	void SMAAPostProcess::Init()
	{
		m_pColorEdgeDetection = CreateTechnique("ColorEdgeDetection","smaa/coloredgedetection","smaa/coloredgedetection","");
// 		m_pColorEdgeDetection->m_eDepthCheckMode = CMPF_ALWAYS_PASS;
// 		m_pColorEdgeDetection->m_bDepthWrite = false;
// 		m_pColorEdgeDetection->m_bStencil = true;
// 		m_pColorEdgeDetection->m_eStencilfunc = CMPF_ALWAYS_PASS;
// 		m_pColorEdgeDetection->m_nStencilRefValue = 1;
// 		m_pColorEdgeDetection->m_nStencilMask = -1;
// 		m_pColorEdgeDetection->m_nStencilWriteMask = -1;
// 		m_pColorEdgeDetection->m_eStencilFail = SOP_KEEP;
// 		m_pColorEdgeDetection->m_eDepthFailOp = SOP_KEEP;
// 		m_pColorEdgeDetection->m_eStencilPass = SOP_REPLACE;
		
		RefPtr<DepthStencilState> pDSSate = CreateDepthStencilState();
		pDSSate->m_eDepthCheckMode = CMPF_ALWAYS_PASS;
		pDSSate->m_bDepthWrite = false;
		pDSSate->m_bStencil = true;
		pDSSate->m_eStencilfunc = CMPF_ALWAYS_PASS;
		//pDSSate->m_nStencilRefValue = 1;
		pDSSate->m_nStencilMask = -1;
		pDSSate->m_nStencilWriteMask = -1;
		pDSSate->m_eStencilFail = SOP_KEEP;
		pDSSate->m_eDepthFailOp = SOP_KEEP;
		pDSSate->m_eStencilPass = SOP_REPLACE;
		m_pColorEdgeDetection->SetDepthStencilState(pDSSate.get());

		//m_pColorEdgeDetection->SaveToXML("tech/ColorEdgeDetection.tech");
	

		m_pBlendWeightCalculation = CreateTechnique("BlendWeightCalculation","smaa/BlendWeightCalculation","smaa/BlendWeightCalculation","");
// 		m_pBlendWeightCalculation->m_eDepthCheckMode = CMPF_ALWAYS_PASS;
// 		m_pBlendWeightCalculation->m_bDepthWrite = false;
// 		m_pBlendWeightCalculation->m_bStencil = true;
// 		m_pBlendWeightCalculation->m_eStencilfunc = CMPF_EQUAL;
// 		m_pBlendWeightCalculation->m_nStencilRefValue = 1;
// 		m_pBlendWeightCalculation->m_nStencilMask = -1;
// 		m_pBlendWeightCalculation->m_nStencilWriteMask = -1;
// 		m_pBlendWeightCalculation->m_eStencilFail = SOP_KEEP;
// 		m_pBlendWeightCalculation->m_eDepthFailOp = SOP_KEEP;
// 		m_pBlendWeightCalculation->m_eStencilPass = SOP_KEEP;
		RefPtr<DepthStencilState> pDSSate2 = CreateDepthStencilState();
		pDSSate2->m_eDepthCheckMode = CMPF_ALWAYS_PASS;
		pDSSate2->m_eDepthCheckMode = CMPF_ALWAYS_PASS;
		pDSSate2->m_bDepthWrite = false;
		pDSSate2->m_bStencil = true;
		pDSSate2->m_eStencilfunc = CMPF_EQUAL;
		//pDSSate2->m_nStencilRefValue = 1;
		pDSSate2->m_nStencilMask = -1;
		pDSSate2->m_nStencilWriteMask = -1;
		pDSSate2->m_eStencilFail = SOP_KEEP;
		pDSSate2->m_eDepthFailOp = SOP_KEEP;
		pDSSate2->m_eStencilPass = SOP_KEEP;
		m_pBlendWeightCalculation->SetDepthStencilState(pDSSate2.get());

		m_pAreadTex = CreateSamplerState("AreaTexDX10.dds",CLAMP,TFO_BILINEAR,false);
		m_pAreadTex->SetWrapModeW(CLAMP);
		m_pBlendWeightCalculation->SetParameter("areaTex",Any(m_pAreadTex));

		m_pSreachTex = CreateSamplerState("SearchTex.dds",CLAMP,TFO_POINT,false);
		m_pSreachTex->SetWrapModeW(CLAMP);
		m_pBlendWeightCalculation->SetParameter("searchTex",Any(m_pSreachTex));

		//m_pBlendWeightCalculation->SaveToXML("tech/BlendWeightCalculation.tech");

		m_pNeiborhoodBlending = CreateTechnique("NeiborhoodBlending","smaa/NeiborhoodBlending","smaa/NeiborhoodBlending","");
// 		m_pNeiborhoodBlending->m_eDepthCheckMode = CMPF_ALWAYS_PASS;
// 		m_pNeiborhoodBlending->m_bDepthWrite = false;
		RefPtr<DepthStencilState> pDSSate3 = CreateDepthStencilState();
		pDSSate3->m_eDepthCheckMode = CMPF_ALWAYS_PASS;
		pDSSate3->m_bDepthWrite = false;
		pDSSate3->m_bStencil = false;
		m_pNeiborhoodBlending->SetDepthStencilState(pDSSate3.get());

		//m_pNeiborhoodBlending->SaveToXML("tech/m_pNeiborhoodBlending.tech");
	}

	void SMAAPostProcess::Reset(Texture* pInput, Texture* pOutput)
	{
		int nWidth = pInput->GetWidth();
		int nHeight = pInput->GetHeight();

		Vector4 vRTMetrics(1.f / nWidth, 1.f / nHeight,(float)nWidth, (float)nHeight);

		m_pInputTex = pInput;
		m_pOutputTex = pOutput;
			
		m_pTexEdge = GetRenderSystem()->CreateRenderTarget(nWidth,nHeight,1,PF_A8R8G8B8,false); 
		RefPtr<SamplerState> pSamplerEdge = CreateSamplerState(m_pTexEdge.get(),CLAMP,TFO_TRILINEAR,false);

		m_pTexBlend = GetRenderSystem()->CreateRenderTarget(nWidth,nHeight,1,PF_A8R8G8B8,false);
		RefPtr<SamplerState> pSamplerBlend = CreateSamplerState(m_pTexBlend.get(),CLAMP,TFO_TRILINEAR,false);

		RefPtr<SamplerState> pSrcSampler = CreateSamplerState(m_pInputTex.get(),CLAMP,TFO_TRILINEAR,false);
		RefPtr<SamplerState> pSrcSamplerSRGB = CreateSamplerState(m_pInputTex.get(),CLAMP,TFO_TRILINEAR,true);

		m_pColorEdgeDetection->SetParameter("tSrcColor",Any(pSrcSampler));
		m_pColorEdgeDetection->SetParameter( "SMAA_RT_METRICS", Any(vRTMetrics) );

		m_pBlendWeightCalculation->SetParameter("tSrcColor",Any(pSamplerEdge));
		m_pBlendWeightCalculation->SetParameter( "SMAA_RT_METRICS", Any(vRTMetrics) );

		m_pNeiborhoodBlending->SetParameter("tSrcColor",Any(pSrcSamplerSRGB));
		m_pNeiborhoodBlending->SetParameter("blendTex",Any(pSamplerBlend));
		m_pNeiborhoodBlending->SetParameter( "SMAA_RT_METRICS", Any(vRTMetrics) );
	}

	void SMAAPostProcess::Shutdown()
	{

	}

	void SMAAPostProcess::Render()
	{
		RENDER_PROFILE(SMAAPostProcess);

		// Pass1 EdgeDetection
		{
			//GetRenderSystem()->SetRenderTarget(0,m_pTexEdge.get());
			//GetRenderSystem()->ClearBuffer(true,false,true, ColourValue::ZERO, 1.0f, 0);

			ScreenQuad::Render(m_pColorEdgeDetection.get());
		}

	
		// Pass2 BlendWeightCalculation
		{
			//GetRenderSystem()->SetRenderTarget(0,m_pTexBlend.get());
			//GetRenderSystem()->ClearBuffer(true,false,false, ColourValue::ZERO, 1.0f, 0);

			ScreenQuad::Render(m_pBlendWeightCalculation.get());
		}

		// Pass3 NeiborhoodBlending
		{
			//GetRenderSystem()->SetRenderTarget(0,m_pOutputTex.get());

			ScreenQuad::Render(m_pNeiborhoodBlending.get());
		}
	}

}
