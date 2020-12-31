#include "RenderScheme.h"
#include "../RenderSystem/RenderCommand.h"
#include "../RenderSystem/RenderProfile.h"
#include "PostProcess.h"

namespace ma
{
	DeferredShading::DeferredShading()
	{
		m_bShadowMapEnable = true;

		if (m_bShadowMapEnable)
		{
			m_pDeferredShadow = new DeferredShadow();
			m_pDeferredShadow->Init();
		}

		g_pPostProcessPipeline = new PostProcessPipeline();

		SetSceneContext(m_pRenderQueue->GetSceneContext());

		Init();
	}

	DeferredShading::~DeferredShading()
	{
		SAFE_DELETE(g_pPostProcessPipeline);
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

	void SetupSSAO()
	{
		g_pPostProcessPipeline->AddRenderPass("SSAO", 1.0f, PF_FLOAT16_R);

		RefPtr<PostProcess> pSSAO = new PostProcess();
		pSSAO->SetName("SSAO");
		g_pPostProcessPipeline->AddPostProcess(pSSAO.get());

		RefPtr<PostProcessStep> pStep = new PostProcessStep();
		pStep->SetName("SSAO");
		pStep->SetOutput("SSAO");

		pStep->SetTechnique("shader/ssao.tech");
		Vector4 texSize = Vector4(800.0f, 600.0f, 1.0f / 800.0f, 1.0f / 600.0f);
		pStep->GetMaterial()->SetParameter("inputTexSize", Any(texSize));
		pSSAO->AddStep(pStep.get());
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

	void DeferredShading::SetupBasePass()
	{
		m_pBaseColor = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8, false);
		m_pNormalTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8, false);

		m_pGbufferPass = GetRenderDevice()->CreateRenderPass();
		m_pGbufferPass->AttachDepthStencil( RenderSurface(m_pDepthTex) );
		m_pGbufferPass->AttachColor(0, RenderSurface(m_pBaseColor) );
		m_pGbufferPass->AttachColor(1, RenderSurface(m_pNormalTex) );
		GetRenderSystem()->RenderPassStreamComplete(m_pGbufferPass.get());

		m_pBaseSampler = CreateSamplerState(m_pBaseColor.get(), CLAMP, TFO_POINT, false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("u_textureSceneDiffuse", [this](Renderable*)->SamplerState* {
			return m_pBaseSampler.get();
		});

		m_pNormalSampler = CreateSamplerState(m_pNormalTex.get(), CLAMP, TFO_POINT, false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("u_textureSceneNormal", [this](Renderable*)->SamplerState* {
			return m_pNormalSampler.get();
		});

		m_pGbufferStep = new RenderStep();
		m_pGbufferStep->m_strName == "GBuffer";
		m_pGbufferStep->m_pRenderPass = m_pGbufferPass;
		GetRenderSystem()->AddRenderStep(m_pGbufferStep.get());

		m_pTransluceStep = new RenderStep();
		m_pTransluceStep->m_strName = "Transluce";
		m_pTransluceStep->m_pRenderPass = m_pTranslucePass.get();

	}

	void DeferredShading::SetupLightPass()
	{
		if (m_bHDREnable)
		{
			m_pLightPass = GetRenderDevice()->CreateRenderPass();
			m_pLightPass->AttachColor(0, RenderSurface(m_pHDRColorTex) );
			RenderSurface depth(m_pDepthTex);
			depth.m_eLoadOp = LOAD_OP_LOAD;
			m_pLightPass->AttachDepthStencil(depth);
			GetRenderSystem()->RenderPassStreamComplete(m_pLightPass.get());
		}
		else
		{
			m_pLightPass = GetRenderSystem()->GetBackBufferRenderPass();
		}

		m_pLightStep = new RenderStep();
		m_pLightStep->m_strName = "Light";
		m_pLightStep->m_pRenderPass = m_pLightPass;

		for (auto& it : m_pGbufferStep->m_pRenderPass->m_arrColor)
		{
			m_pLightStep->m_vecReadTextue.push_back(it.m_pTexture);
		}
		m_pLightStep->m_vecReadTextue.push_back(m_pGbufferStep->m_pRenderPass->m_depthStencil.m_pTexture);

		GetRenderSystem()->AddRenderStep(m_pLightStep);

		// Setup Tech
		if (1)
		{
			RefPtr<DepthStencilState> pDS = CreateDepthStencilState();
			pDS->m_bDepthWrite = false;

			RefPtr<BlendState> pBS = CreateBlendState();
			pBS->m_blendDesc[0].BlendEnable = true;
			pBS->m_blendDesc[0].SrcBlend = BLEND_ONE;
			pBS->m_blendDesc[0].DestBlend = BLEND_ONE;

			RefPtr<RasterizerState> pRS = CreateRasterizerState();
			pRS->m_eCullMode = CULL_FACE_SIDE_BACK;

			RefPtr<VertexDeclaration> pDec = CreateVertexDeclaration();
			pDec->AddElement(VertexElement(0, 0, DT_FLOAT2, DU_POSITION, 0));
			pDec->AddElement(VertexElement(0, 8, DT_FLOAT2, DU_TEXCOORD, 0));

			ShaderCreateInfo Info;
			Info.m_strVSFile = "deferredlight.vert:vs_main";
			Info.m_strPSFile = "deferredlight.frag:ps_main";
			Info.m_pRenderPass = m_pLightPass;
			Info.m_pDSState = pDS;
			Info.m_pVertexDecl = pDec;
			Info.m_pRSState = pRS;

			{
				Info.m_shaderMacro = "AMBIENT_LIGHT";

				RefPtr<Technique> pAmbientLight = CreateTechnique(Info);
				pAmbientLight->SaveToXML("shader/ambientLight.tech");

				GetRenderSystem()->TechniqueStreamComplete(pAmbientLight.get());
			}

			{
				Info.m_shaderMacro = "DIRECT_LIGHT";
				Info.m_pBlendState = pBS;

				RefPtr<Technique> pDirLight = CreateTechnique(Info);
				pDirLight->SaveToXML("shader/dirlight.tech");

				GetRenderSystem()->TechniqueStreamComplete(pDirLight.get());
			}

			{
				Info.m_pVertexDecl = CreateVertexDeclaration();
				Info.m_pVertexDecl->AddElement(VertexElement(0, 0, DT_FLOAT3, DU_POSITION, 0));

				Info.m_shaderMacro = "POINT_LIGHT";
				Info.m_pBlendState = pBS;

				RefPtr<Technique> pPointLight = CreateTechnique(Info);
				pPointLight->SaveToXML("shader/pointlight.tech");

				GetRenderSystem()->TechniqueStreamComplete(pPointLight.get());
			}
		}

		
	}

	void DeferredShading::SetupHDRPass()
	{
		if (!m_bHDREnable)
			return;

		RefPtr<RenderPass> pHDRPass = GetRenderDevice()->CreateRenderPass();

		pHDRPass->AttachColor(0, RenderSurface(m_pHDRColorTex) );
		pHDRPass->AttachDepthStencil( RenderSurface(m_pDepthTex) );

		GetRenderSystem()->RenderPassStreamComplete(pHDRPass.get());

		if (0)
		{
			SetupHDR();
			g_pPostProcessPipeline->SaveToXML("postprocess.xml");
		}

		g_pPostProcessPipeline->LoadFromXML("postprocess.xml");

		RefPtr<RenderPass> pTemPass = GetRenderDevice()->CreateRenderPass();
		RefPtr<Texture> pTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8);
		RenderSurface color;
		color.m_pTexture = pTex;
		pTemPass->AttachColor(0, color);
		GetRenderSystem()->RenderPassStreamComplete(pTemPass.get());

		g_pPostProcessPipeline->Setup(pHDRPass.get(), pTemPass.get());

		m_lastStep = new PostProcessStep();
		m_lastStep->SetName("copy");
		m_lastStep->SetInput("tSrcColor", "[StageInput]");
		m_lastStep->SetOutput("[StageOutput]");
		m_lastStep->SetTechnique("shader/copy.tech");
		m_lastStep->Setup(pTemPass.get(), GetRenderSystem()->GetBaseRenderPass());
	}

	void DeferredShading::Init()
	{	
		m_pBackBaufferPass = GetRenderSystem()->GetBaseRenderPass();

		m_pDepthTex = GetRenderSystem()->CreateDepthStencil(-1, -1, PF_D24S8);

		m_pHDRColorTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_FLOAT16_RGBA);

		m_pDepthSampler = CreateSamplerState(m_pDepthTex.get(),CLAMP,TFO_POINT,false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("tDeviceDepthMapSampler", [this](Renderable*)->SamplerState* {
			return m_pDepthSampler.get();
 		} );
		
		SetupBasePass();

		SetupLightPass();

		SetupHDRPass();

		if (m_pDeferredShadow)
		{
			m_pDeferredShadow->Reset(m_pDepthTex.get());
		}

		GetRenderSystem()->SetBaseRenderPass(m_pGbufferPass.get());
		GetRenderSystem()->SetDefferedLightRenderPass(m_pLightPass.get());
		GetRenderSystem()->ReloadShader();
	}





}


