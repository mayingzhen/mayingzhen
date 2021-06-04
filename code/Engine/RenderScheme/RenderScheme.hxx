#include "RenderScheme.h"
#include "../RenderSystem/RenderCommand.h"
#include "../RenderSystem/RenderProfile.h"
#include "PostProcess.h"

namespace ma
{
	DeferredShading::DeferredShading(MainRenderView* renderView)
	{
		m_pMainView = renderView;

		m_pPostProcessPipeline = new PostProcessPipeline(renderView);
	}

	DeferredShading::~DeferredShading()
	{
	}


	void DeferredShading::SetupGlow(PostProcess* pGlow)
	{
		float fScale = 0.5f;
		uint32_t iterations = 6;
		for (uint32_t i = 0; i < iterations; ++i)
		{
			std::string strDownSampleTarget = std::string("DownSampleTarget") + StringConverter::toString(i);
			m_pPostProcessPipeline->AddRenderPass(strDownSampleTarget.c_str(), fScale, PF_A8R8G8B8);

			std::string strUpSampleTarget = std::string("UpSampleTarget") + StringConverter::toString(i);
			m_pPostProcessPipeline->AddRenderPass(strUpSampleTarget.c_str(), fScale, PF_A8R8G8B8);

			fScale = fScale * 0.5f;
		}

		std::string strInput = "[StageInput]";
		std::string strOuput = "DownSampleTarget0";

		// Prefilter
		RefPtr<PostProcessStep> pPrefilter = new PostProcessStep(pGlow);
		pPrefilter->SetName("BloomPrefilter");
		pPrefilter->SetInput("_MainTex", strInput.c_str());
		pPrefilter->SetInput("_AutoExposureTex", "IllumAdjust");
		pPrefilter->SetOutput(strOuput.c_str());
		pPrefilter->SetTechnique("shader/prefilter.tech");
		pGlow->AddStep(pPrefilter.get());


		Vector4 _Threshold = Vector4(1.0f);
		pPrefilter->GetMaterial()->SetParameter("_Threshold", Any(_Threshold));

		Vector4 _Params = Vector4(1.0f);
		pPrefilter->GetMaterial()->SetParameter("_Params", Any(_Params));

		strInput = strOuput;

		//DownSample
		for (uint32_t i = 1; i < iterations; ++i)
		{
			std::string strOuputDownSample = std::string("DownSampleTarget") + StringConverter::toString(i);

			RefPtr<PostProcessStep> pDownSample = new PostProcessStep(pGlow);
			pDownSample->SetName(strOuputDownSample.c_str());
			pDownSample->SetInput("_MainTex", strOuput.c_str());
			pDownSample->SetOutput(strOuputDownSample.c_str());
			pDownSample->SetTechnique("shader/downsample.tech");
			pGlow->AddStep(pDownSample.get());

			strInput = strOuputDownSample;
		}

		//UpSample
		for (int i = iterations - 2; i >= 0; i--)
		{
			std::string strOuputUpSample = std::string("UpSampleTarget") + StringConverter::toString(i);
			
			std::string strBloom = std::string("UpSampleTarget") + StringConverter::toString(i + 1);

			RefPtr<PostProcessStep> pUpSample = new PostProcessStep(pGlow);
			pUpSample->SetName(strOuputUpSample.c_str());
			pUpSample->SetInput("_MainTex", strInput.c_str());
			pUpSample->SetInput("_BloomTex", strBloom.c_str());
			pUpSample->SetOutput(strOuputUpSample.c_str());
			pUpSample->SetTechnique("shader/upsample.tech");
			pGlow->AddStep(pUpSample.get());

			pUpSample->GetMaterial()->SetParameter("_SampleScale", Any(1.0f));

			strInput = strOuputUpSample;
		}

	}

	void DeferredShading::SetupSSAO()
	{
		m_pPostProcessPipeline->AddRenderPass("SSAO", 1.0f, PF_FLOAT16_R);

		RefPtr<PostProcess> pSSAO = new PostProcess(m_pPostProcessPipeline.get());
		pSSAO->SetName("SSAO");
		m_pPostProcessPipeline->AddPostProcess(pSSAO.get());

		RefPtr<PostProcessStep> pStep = new PostProcessStep(pSSAO.get());
		pStep->SetName("SSAO");
		pStep->SetOutput("SSAO");

		pStep->SetTechnique("shader/ssao.tech");
		Vector4 texSize = Vector4(800.0f, 600.0f, 1.0f / 800.0f, 1.0f / 600.0f);
		pStep->GetMaterial()->SetParameter("inputTexSize", Any(texSize));
		pSSAO->AddStep(pStep.get());
	}

	void DeferredShading::SetupHDR()
	{
		{
			m_pPostProcessPipeline->AddRenderPass("IllumAdjust", 1, 1, PF_FLOAT32_R);
			m_pPostProcessPipeline->AddRenderPass("IllumLast", 1, 1, PF_FLOAT32_R);
			m_pPostProcessPipeline->AddRenderPass("IllumLumLog1", 4, 4, PF_FLOAT32_R);
			m_pPostProcessPipeline->AddRenderPass("IllumLumLog2", 16, 16, PF_FLOAT32_R);
			m_pPostProcessPipeline->AddRenderPass("IllumLumLog3", 64, 64, PF_FLOAT32_R);
		}

		RefPtr<PostProcess> pHDR = new PostProcess(m_pPostProcessPipeline.get());
		pHDR->SetName("HDR");
		m_pPostProcessPipeline->AddPostProcess(pHDR.get());

	
		{
			RefPtr<PostProcessStep> pStepLog = new PostProcessStep(pHDR.get());
			pStepLog->SetName("SumLuminanceLog");
			pStepLog->SetInput("tSrcColor", "[StageInput]");
			pStepLog->SetOutput("IllumLumLog3");
			pStepLog->SetTechnique("shader/SumLuminanceLog.tech");
			Vector4 texSize = Vector4(800.0f, 600.0f, 1.0f / 800.0f, 1.0f / 600.0f);
			pStepLog->GetMaterial()->SetParameter("inputTexSize", Any(texSize));
			pHDR->AddStep(pStepLog.get());
		}

		{
			RefPtr<PostProcessStep> pStepIterative = new PostProcessStep(pHDR.get());
			pStepIterative->SetName("SumLuminanceIterative1");
			pStepIterative->SetInput("tSrcColor", "IllumLumLog3");
			pStepIterative->SetOutput("IllumLumLog2");
			pStepIterative->SetTechnique("shader/SumLuminanceIterative.tech");
			Vector4 texSize = Vector4(64.0f, 64.0f, 1.0f / 64.0f, 1.0f / 64.0f);
			pStepIterative->GetMaterial()->SetParameter("inputTexSize", Any(texSize));
			pHDR->AddStep(pStepIterative.get());
		}

		{
			RefPtr<PostProcessStep> pStepIterative = new PostProcessStep(pHDR.get());
			pStepIterative->SetName("SumLuminanceIterative2");
			pStepIterative->SetInput("tSrcColor", "IllumLumLog2");
			pStepIterative->SetOutput("IllumLumLog1");
			pStepIterative->SetTechnique("shader/SumLuminanceIterative.tech");
			Vector4 texSize = Vector4(16.0f, 16.0f, 1.0f / 16.0f, 1.0f / 16.0f);
			pStepIterative->GetMaterial()->SetParameter("inputTexSize", Any(texSize));
			pHDR->AddStep(pStepIterative.get());
		}


		{
			RefPtr<PostProcessStep> pStepFinal = new PostProcessStep(pHDR.get());
			pStepFinal->SetName("SumLuminanceFinal");
			pStepFinal->SetInput("tSrcColor", "IllumLumLog1");
			pStepFinal->SetInput("g_Texturelast", "IllumLast");
			pStepFinal->SetOutput("IllumAdjust");
			pStepFinal->SetTechnique("shader/SumLuminanceFinal.tech");
			Vector4 texSize = Vector4(4.0f, 4.0f, 1.0f / 4.0f, 1.0f / 4.0f);
			pStepFinal->GetMaterial()->SetParameter("inputTexSize", Any(texSize));
			pStepFinal->GetMaterial()->SetParameter("AdaptationRate", Any(0.5f));
			pHDR->AddStep(pStepFinal.get());
		}

		{
			RefPtr<PostProcessStep> pCopy = new PostProcessStep(pHDR.get());
			pCopy->SetName("copy");
			pCopy->SetInput("tSrcColor", "IllumAdjust");
			pCopy->SetOutput("IllumLast");
			pCopy->SetTechnique("shader/copy.tech");
			pHDR->AddStep(pCopy.get());
		}

		SetupGlow(pHDR.get());

		{
			RefPtr<PostProcessStep> pStepToneMap = new PostProcessStep(pHDR.get());
			pStepToneMap->SetName("ToneMap");
			pStepToneMap->SetInput("gTex_Scene", "[StageInput]");
			pStepToneMap->SetInput("gTexBloom", "UpSampleTarget0");
			pStepToneMap->SetInput("gTex_Lum", "IllumAdjust");
			pStepToneMap->SetOutput("[StageOutput]");
			pStepToneMap->SetTechnique("shader/ToneMaping.tech");
			pHDR->AddStep(pStepToneMap.get());
		}
	}


	void DeferredShading::Init()
	{	
		RefPtr<GbufferStep> pBufferStep = new GbufferStep();
		m_pMainView->AddRenderStep(pBufferStep.get());

		RefPtr<LinearDepthStep> pLinearDepthStep = new LinearDepthStep(pBufferStep->m_pDepthTex.get());
		m_pMainView->AddRenderStep(pLinearDepthStep.get());

		RefPtr<DeferredShadow> pShadowStep = new DeferredShadow(pBufferStep->m_pDepthTex.get());
		m_pMainView->AddRenderStep(pShadowStep.get());
		m_pMainView->SetDeferredShadowSetp(pShadowStep.get());
			
		RefPtr<DefferedLightStep> pLightStep = new DefferedLightStep(pBufferStep->m_pDepthTex.get());
		m_pMainView->AddRenderStep(pLightStep.get());


		if (0)
		{
			SetupHDR();
			m_pPostProcessPipeline->SaveToXML("postprocess.xml");
		}

		m_pPostProcessPipeline = new PostProcessPipeline(m_pMainView.get());
		m_pPostProcessPipeline->LoadFromXML("postprocess.xml");
		m_pPostProcessPipeline->Setup(pLightStep->m_pRenderPass.get(), GetRenderSystem()->GetBackBufferRenderPass());

		PostProcessStep* pLastStep = m_pPostProcessPipeline->GetLastStep();

		RefPtr<UIStep> pUIStep = new UIStep(pLastStep->GetMaterial());
		m_pMainView->AddRenderStep(pUIStep.get());

		GetRenderSystem()->ReloadShader();
	}





}


