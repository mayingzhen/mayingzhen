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

	void SetupGlow()
	{
		RefPtr<PostProcess> pGlow = new PostProcess();
		g_pPostProcessPipeline->AddPostProcess(pGlow.get());

		//std::string strDownSampleTarget = std::string("DownSampleTarget") + StringConverter::toString(i);
		//g_pPostProcessPipeline->AddRenderPass(strDownSampleTarget.c_str(), 1, 1, PF_FLOAT32_R);
		
		int iterations = 6;
		for (uint32_t i = 0; i < iterations; ++i)
		{
			std::string strDownSampleTarget = std::string("DownSampleTarget") + StringConverter::toString(i);
			g_pPostProcessPipeline->AddRenderPass(strDownSampleTarget.c_str(), 1, 1, PF_FLOAT32_R);

			std::string strUpSampleTarget = std::string("UpSampleTarget") + StringConverter::toString(i);
			g_pPostProcessPipeline->AddRenderPass(strUpSampleTarget.c_str(), 1, 1, PF_FLOAT32_R);
		}

		//DownSample
		std::string strInput = "[StageInput]";
		for (uint32_t i = 0; i < iterations; ++i)
		{
			std::string strOuput = std::string("DownSampleTarget") + StringConverter::toString(i);

			RefPtr<PostProcessStep> pDownSample = new PostProcessStep();
			pDownSample->SetInput("tSrcColor", strInput.c_str());
			pDownSample->SetOutput(strOuput.c_str());
			pDownSample->SetTechnique("shader/downsample.tech");
			pGlow->AddStep(pDownSample.get());

			strInput = strOuput;
		}

		//UpSample
		for (int i = iterations - 2; i >= 0; i--)
		{
			std::string strOuput = std::string("UpSampleTarget") + StringConverter::toString(i);

			RefPtr<PostProcessStep> pUpSample = new PostProcessStep();
			pUpSample->SetInput("tSrcColor", strInput.c_str());
			pUpSample->SetOutput(strOuput.c_str());
			pUpSample->SetTechnique("shader/upsample.tech");
			pGlow->AddStep(pUpSample.get());

			strInput = strOuput;
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
		g_pPostProcessPipeline->AddPostProcess(pHDR.get());

		{
			RefPtr<PostProcessStep> pStepLog = new PostProcessStep();
			pStepLog->SetInput("g_TexSrc", "[StageInput]");
			pStepLog->SetOutput("IllumLumLog3");
			pStepLog->SetTechnique("shader/SumLuminanceLog.tech");
			pHDR->AddStep(pStepLog.get());
		}

		{
			RefPtr<PostProcessStep> pStepIterative = new PostProcessStep();
			pStepIterative->SetInput("tSrcColor", "IllumLumLog3");
			pStepIterative->SetOutput("IllumLumLog2");
			pStepIterative->SetTechnique("shader/SumLuminanceIterative.tech");
			pHDR->AddStep(pStepIterative.get());
		}

		{
			RefPtr<PostProcessStep> pStepIterative = new PostProcessStep();
			pStepIterative->SetInput("g_TextureSrc", "IllumLumLog2");
			pStepIterative->SetOutput("IllumLumLog1");
			pStepIterative->SetTechnique("shader/SumLuminanceIterative.tech");
			pHDR->AddStep(pStepIterative.get());
		}

		{
			RefPtr<PostProcessStep> pStepFinal = new PostProcessStep();
			pStepFinal->SetInput("g_TextureSrc", "IllumLumLog1");
			pStepFinal->SetInput("g_TexLumLast", "IllumLast");
			pStepFinal->SetOutput("IllumAdjust");
			pStepFinal->SetTechnique("shader/SumLuminanceFinal.tech");
			pHDR->AddStep(pStepFinal.get());
		}

		{
			RefPtr<PostProcessStep> pCopy = new PostProcessStep();
			pCopy->SetInput("tSrcColor", "IllumAdjust");
			pCopy->SetOutput("IllumLast");
			pCopy->SetTechnique("shader/copy.tech");
			pHDR->AddStep(pCopy.get());
		}

		{
			RefPtr<PostProcessStep> pStepToneMap = new PostProcessStep();
			pStepToneMap->SetInput("gTex_Scene", "[StageInput]");
			pStepToneMap->SetInput("gTex_Lum", "IllumAdjust");
			pStepToneMap->SetOutput("[StageOutput]");
			pStepToneMap->SetTechnique("shader/ToneMaping.tech");
			pHDR->AddStep(pStepToneMap.get());
		}
	}

	void RenderScheme::Reset()
	{	
		m_pShadingPass = GetRenderSystem()->GetDefaultRenderPass();

		if (0)
		{
			RefPtr<RenderPass> pHDRPass = GetRenderDevice()->CreateRenderPass();

			RefPtr<Texture> pTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_FLOAT16_RGBA);
			RefPtr<Texture> pDepthTex = GetRenderSystem()->CreateDepthStencil(-1, -1, PF_D24S8);

			pHDRPass->AttachColor(0, pTex.get(), 0, 0);
			pHDRPass->AttachDepthStencil(pDepthTex.get());

			GetRenderSystem()->RenderPassStreamComplete(pHDRPass.get());
			
			SetupHDR();

			SetupGlow();

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

	void RenderScheme::BeginRender()
	{
		m_pShadingPass->Begine();
	}

	void RenderScheme::Render()
	{
		RenderQueue* pRenderQueue = m_pScene->GetRenderQueue();

		//m_pShadingPass->Begine();

		{
			RENDER_PROFILE(RL_Mesh);
			pRenderQueue->RenderObjList(m_pShadingPass.get(),RL_Mesh,RP_Shading);
		}

		{
			RENDER_PROFILE(RL_Terrain);
			pRenderQueue->RenderObjList(m_pShadingPass.get(),RL_Terrain,RP_Shading);
		}

		{
			RENDER_PROFILE(RL_SkyBox);
			pRenderQueue->RenderObjList(m_pShadingPass.get(), RL_SkyBox, RP_Shading);
		}

		//m_pShadingPass->End();

		//g_pPostProcessPipeline->Render();
	}

	void RenderScheme::EndRender()
	{
		m_pShadingPass->End();

		g_pPostProcessPipeline->Render();
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


