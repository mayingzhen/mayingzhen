#include "RenderScheme.h"
#include "../RenderSystem/RenderCommand.h"
#include "../RenderSystem/RenderProfile.h"
#include "PostProcess.h"

namespace ma
{
	RenderScheme::RenderScheme(Scene* pScene)
	{
		m_pScene = pScene;
		g_pPostProcessPipeline = new PostProcessPipeline();
	}

	RenderScheme::~RenderScheme()
	{
		SAFE_DELETE(g_pPostProcessPipeline);
	}

	void RenderScheme::Init()
	{
		Shoutdown();
	}

	void SetupGlow(PostProcess* pGlow)
	{

		float fScale = 0.5f;
		uint32_t iterations = 6;
		for (uint32_t i = 0; i < iterations; ++i)
		{
			std::string strDownSampleTarget = std::string("DownSampleTarget") + StringConverter::toString(i);
			g_pPostProcessPipeline->AddRenderPass(strDownSampleTarget.c_str(), fScale, PF_A8R8G8B8);

			std::string strUpSampleTarget = std::string("UpSampleTarget") + StringConverter::toString(i);
			g_pPostProcessPipeline->AddRenderPass(strUpSampleTarget.c_str(), fScale, PF_A8R8G8B8);

			fScale = fScale * 0.5f;
		}

		std::string strInput = "[StageInput]";
		std::string strOuput = "DownSampleTarget0";

		// Prefilter
		RefPtr<PostProcessStep> pPrefilter = new PostProcessStep();
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

			RefPtr<PostProcessStep> pDownSample = new PostProcessStep();
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

			RefPtr<PostProcessStep> pUpSample = new PostProcessStep();
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

	void SetupHDR()
	{
		{
			g_pPostProcessPipeline->AddRenderPass("IllumAdjust", 1, 1, PF_FLOAT32_R);
			g_pPostProcessPipeline->AddRenderPass("IllumLast", 1, 1, PF_FLOAT32_R);
			g_pPostProcessPipeline->AddRenderPass("IllumLumLog1", 4, 4, PF_FLOAT32_R);
			g_pPostProcessPipeline->AddRenderPass("IllumLumLog2", 16, 16, PF_FLOAT32_R);
			g_pPostProcessPipeline->AddRenderPass("IllumLumLog3", 64, 64, PF_FLOAT32_R);
		}

		RefPtr<PostProcess> pHDR = new PostProcess();
		pHDR->SetName("HDR");
		g_pPostProcessPipeline->AddPostProcess(pHDR.get());

	
		{
			RefPtr<PostProcessStep> pStepLog = new PostProcessStep();
			pStepLog->SetName("SumLuminanceLog");
			pStepLog->SetInput("tSrcColor", "[StageInput]");
			pStepLog->SetOutput("IllumLumLog3");
			pStepLog->SetTechnique("shader/SumLuminanceLog.tech");
			Vector4 texSize = Vector4(800.0f, 600.0f, 1.0f / 800.0f, 1.0f / 600.0f);
			pStepLog->GetMaterial()->SetParameter("inputTexSize", Any(texSize));
			pHDR->AddStep(pStepLog.get());
		}

		{
			RefPtr<PostProcessStep> pStepIterative = new PostProcessStep();
			pStepIterative->SetName("SumLuminanceIterative1");
			pStepIterative->SetInput("tSrcColor", "IllumLumLog3");
			pStepIterative->SetOutput("IllumLumLog2");
			pStepIterative->SetTechnique("shader/SumLuminanceIterative.tech");
			Vector4 texSize = Vector4(64.0f, 64.0f, 1.0f / 64.0f, 1.0f / 64.0f);
			pStepIterative->GetMaterial()->SetParameter("inputTexSize", Any(texSize));
			pHDR->AddStep(pStepIterative.get());
		}

		{
			RefPtr<PostProcessStep> pStepIterative = new PostProcessStep();
			pStepIterative->SetName("SumLuminanceIterative2");
			pStepIterative->SetInput("tSrcColor", "IllumLumLog2");
			pStepIterative->SetOutput("IllumLumLog1");
			pStepIterative->SetTechnique("shader/SumLuminanceIterative.tech");
			Vector4 texSize = Vector4(16.0f, 16.0f, 1.0f / 16.0f, 1.0f / 16.0f);
			pStepIterative->GetMaterial()->SetParameter("inputTexSize", Any(texSize));
			pHDR->AddStep(pStepIterative.get());
		}


		{
			RefPtr<PostProcessStep> pStepFinal = new PostProcessStep();
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
			RefPtr<PostProcessStep> pCopy = new PostProcessStep();
			pCopy->SetName("copy");
			pCopy->SetInput("tSrcColor", "IllumAdjust");
			pCopy->SetOutput("IllumLast");
			pCopy->SetTechnique("shader/copy.tech");
			pHDR->AddStep(pCopy.get());
		}

		SetupGlow(pHDR.get());

		{
			RefPtr<PostProcessStep> pStepToneMap = new PostProcessStep();
			pStepToneMap->SetName("ToneMap");
			pStepToneMap->SetInput("gTex_Scene", "[StageInput]");
			pStepToneMap->SetInput("gTexBloom", "UpSampleTarget0");
			pStepToneMap->SetInput("gTex_Lum", "IllumAdjust");
			pStepToneMap->SetOutput("[StageOutput]");
			pStepToneMap->SetTechnique("shader/ToneMaping.tech");
			pHDR->AddStep(pStepToneMap.get());
		}
	}

	void RenderScheme::Reset()
	{	
		m_pBackBaufferPass = GetRenderSystem()->GetDefaultRenderPass();

		m_pShadingPass = m_pBackBaufferPass;

		if (1)
		{
			RefPtr<RenderPass> pHDRPass = GetRenderDevice()->CreateRenderPass();

			RefPtr<Texture> pTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_FLOAT16_RGBA);
			RefPtr<Texture> pDepthTex = GetRenderSystem()->CreateDepthStencil(-1, -1, PF_D24S8);

			pHDRPass->AttachColor(0, pTex.get(), 0, 0);
			pHDRPass->AttachDepthStencil(pDepthTex.get());

			GetRenderSystem()->RenderPassStreamComplete(pHDRPass.get());
			
			if (0)
			{
				SetupHDR();
				g_pPostProcessPipeline->SaveToXML("postprocess.xml");
			}

			g_pPostProcessPipeline->LoadFromXML("postprocess.xml");

			g_pPostProcessPipeline->Setup(pHDRPass.get(), GetRenderSystem()->GetDefaultRenderPass());

			GetRenderSystem()->SetDefaultRenderPass(pHDRPass.get());
			GetRenderSystem()->ReloadShader();

			m_pShadingPass = pHDRPass;

		}
	}

	void RenderScheme::Shoutdown()
	{
		m_pDepthTex = NULL;
		m_pNormalTex = NULL;
		m_pDiffuseTex = NULL;
	}

	void RenderScheme::Render()
	{
		RenderQueue* pRenderQueue = m_pScene->GetRenderQueue();

		m_pShadingPass->Begine();

		{
			RENDER_PROFILE(RL_Mesh);
			pRenderQueue->RenderObjList(m_pShadingPass.get(),RL_Mesh,RP_Shading);
		}

		{
			RENDER_PROFILE(RL_Terrain);
			pRenderQueue->RenderObjList(m_pShadingPass.get(),RL_Terrain,RP_Shading);

			pRenderQueue->RenderObjList(m_pShadingPass.get(), RL_TerrainBorder, RP_Shading);

			pRenderQueue->RenderObjList(m_pShadingPass.get(), RL_TerrainSkirt, RP_Shading);
		}

		{
			RENDER_PROFILE(RL_SkyBox);
			pRenderQueue->RenderObjList(m_pShadingPass.get(), RL_SkyBox, RP_Shading);
		}

		m_pShadingPass->End();

		g_pPostProcessPipeline->Render();

		//m_pBackBaufferPass->Begine();

		{
			RENDER_PROFILE(RL_UI);
			pRenderQueue->RenderObjList(m_pBackBaufferPass.get(), RL_UI, RP_Shading);
		}

		m_pBackBaufferPass->End();
	}

	void RenderScheme::SetDeferredShadingEnabled(bool b)
	{
		Init();
		Reset();
	}

	bool RenderScheme::GetDeferredShadingEnabled() const
	{
		return m_pDeferredShading != NULL;
	}
}


