#include "RenderScheme.h"
#include "../RenderSystem/RenderCommand.h"
#include "../RenderSystem/RenderProfile.h"
#include "PostProcess.h"

namespace ma
{

	void RenderStep::Render()
	{
		RenderQueue* cur_renderQueue = m_pRenderQueue[GetRenderSystem()->CurThreadProcess()].get();
		RenderPass*  cur_renderPass = m_pRenderPass.get();

		SetRenderContext(cur_renderQueue->GetRenderContext());

		cur_renderPass->Begine();

		cur_renderQueue->Render(cur_renderPass);

		cur_renderPass->End();
	}

	RenderScheme::RenderScheme()
	{
		g_pPostProcessPipeline = new PostProcessPipeline();

		SetRenderContext(m_pRenderQueue[0]->GetRenderContext());

		Init();

		Reset();
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

	void RenderScheme::SetupBasePass()
	{
		m_pBaseColor = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8, false);
		m_pNormalTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8, false);

		m_pGbufferPass = GetRenderDevice()->CreateRenderPass();
		m_pGbufferPass->AttachDepthStencil(m_pDepthTex.get());
		m_pGbufferPass->AttachColor(0, m_pBaseColor.get());
		m_pGbufferPass->AttachColor(1, m_pNormalTex.get());
		GetRenderSystem()->RenderPassStreamComplete(m_pGbufferPass.get());

		m_pBaseSampler = CreateSamplerState(m_pBaseColor.get(), CLAMP, TFO_POINT, false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("u_textureSceneDiffuse", [this](Renderable*)->SamplerState* {
			return m_pBaseSampler.get();
		});

		m_pNormalSampler = CreateSamplerState(m_pNormalTex.get(), CLAMP, TFO_POINT, false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("u_textureSceneNormal", [this](Renderable*)->SamplerState* {
			return m_pNormalSampler.get();
		});
	}

	void RenderScheme::SetupLightPass()
	{
		if (m_bHDREnable)
		{
			m_pLightPass = GetRenderDevice()->CreateRenderPass();
			m_pLightPass->AttachColor(0, m_pHDRColorTex.get());
			GetRenderSystem()->RenderPassStreamComplete(m_pLightPass.get());
		}
		else
		{
			m_pLightPass = GetRenderSystem()->GetBackBufferRenderPass();
		}
	
		RefPtr<DepthStencilState> pDS = CreateDepthStencilState();
		pDS->m_bDepthWrite = false;

		RefPtr<BlendState> pBS = CreateBlendState();
		pBS->m_blendDesc[0].BlendEnable = true;
		pBS->m_blendDesc[0].SrcBlend = BLEND_ONE;
		pBS->m_blendDesc[0].DestBlend = BLEND_ONE;

		RefPtr<RasterizerState> pRS = CreateRasterizerState();
		pRS->m_eCullMode = CULL_FACE_SIDE_NONE;

		RefPtr<VertexDeclaration> pDec = CreateVertexDeclaration();
		pDec->AddElement(VertexElement(0, 0, DT_FLOAT2, DU_POSITION, 0));
		pDec->AddElement(VertexElement(0, 8, DT_FLOAT2, DU_TEXCOORD, 0));

		ShaderCreateInfo Info;
		Info.m_strVSFile = "deferredlight.vert:main";
		Info.m_strPSFile = "deferredlight.frag:main";
		Info.m_pRenderPass = m_pLightPass;
		Info.m_pDSState = pDS;
		Info.m_pVertexDecl = pDec;

		{
			Info.m_shaderMacro = "AMBIENT_LIGHT";

			m_pAmbientLight = CreateTechnique("shader/ambientlight.tech", Info);
			m_pAmbientLight->SaveToXML("shader/m_pAmbientLight.tech");

			GetRenderSystem()->TechniqueStreamComplete(m_pAmbientLight.get());
		}

		{
			Info.m_shaderMacro = "DIRECT_LIGHT";
			Info.m_pBlendState = pBS;

			m_pDirLight = CreateTechnique("shader/dirlight.tech", Info);
			m_pDirLight->SaveToXML("shader/dirlight.tech");

			GetRenderSystem()->TechniqueStreamComplete(m_pDirLight.get());
		}

		{
			Info.m_pVertexDecl = CreateVertexDeclaration();
			Info.m_pVertexDecl->AddElement(VertexElement(0, 0, DT_FLOAT3, DU_POSITION, 0));

			Info.m_shaderMacro = "POINT_LIGHT";
			Info.m_pBlendState = pBS;

			m_pPointLight = CreateTechnique("shader/pointlight.tech", Info);
			m_pPointLight->SaveToXML("shader/pointlight.tech");

			GetRenderSystem()->TechniqueStreamComplete(m_pPointLight.get());
		}

	}

	void RenderScheme::SetupHDRPass()
	{
		RefPtr<RenderPass> pHDRPass = GetRenderDevice()->CreateRenderPass();

		pHDRPass->AttachColor(0, m_pHDRColorTex.get(), 0, 0);
		pHDRPass->AttachDepthStencil(m_pDepthTex.get());

		GetRenderSystem()->RenderPassStreamComplete(pHDRPass.get());

		if (0)
		{
			SetupHDR();
			g_pPostProcessPipeline->SaveToXML("postprocess.xml");
		}

		g_pPostProcessPipeline->LoadFromXML("postprocess.xml");

		m_pTemPass = GetRenderDevice()->CreateRenderPass();
		RefPtr<Texture> pTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8);
		m_pTemPass->AttachColor(0, pTex.get(), 0, 0);
		GetRenderSystem()->RenderPassStreamComplete(m_pTemPass.get());

		g_pPostProcessPipeline->Setup(pHDRPass.get(), m_pTemPass.get());


		m_lastStep = new PostProcessStep();
		m_lastStep->SetName("copy");
		m_lastStep->SetInput("tSrcColor", "[StageInput]");
		m_lastStep->SetOutput("[StageOutput]");
		m_lastStep->SetTechnique("shader/copy.tech");
		m_lastStep->Setup(m_pTemPass.get(), GetRenderSystem()->GetDefaultRenderPass());
	}

	void RenderScheme::Reset()
	{	
		m_pBackBaufferPass = GetRenderSystem()->GetDefaultRenderPass();

		m_pDepthTex = GetRenderSystem()->CreateDepthStencil(-1, -1, PF_D24S8);

		m_pHDRColorTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_FLOAT16_RGBA);

		m_pDepthSampler = CreateSamplerState(m_pDepthTex.get(),CLAMP,TFO_POINT,false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("tDeviceDepthMapSampler", [this](Renderable*)->SamplerState* {
			return m_pDepthSampler.get();
 		} );
		
		SetupBasePass();

		SetupLightPass();

		SetupHDRPass();

		GetRenderSystem()->SetDefaultRenderPass(m_pGbufferPass.get());
		GetRenderSystem()->ReloadShader();
	}

	void RenderScheme::Shoutdown()
	{
		m_pDepthTex = NULL;
		m_pNormalTex = NULL;
		m_pBaseColor = NULL;
	}

	void RenderScheme::BasePass()
	{
		RenderQueue* pRenderQueue = m_pRenderQueue[GetRenderSystem()->CurThreadProcess()].get();

		m_pGbufferPass->Begine();

		pRenderQueue->Render(m_pGbufferPass.get(), RL_Mesh, RL_MeshTrans);

		m_pGbufferPass->End();
	}

	void RenderScheme::LightPass()
	{
		RenderQueue* pRenderQueue = m_pRenderQueue[GetRenderSystem()->CurThreadProcess()].get();

		m_pLightPass->Begine();

		RenderCommand* pCommand = m_pLightPass->GetThreadCommand(0, 0);

		pCommand->Begin();

		Vector3 cAmbientColor = Vector3::ZERO;//= m_pScene->GetAmbientColor();
		m_pAmbientLight->SetParameter("light_color", Any(cAmbientColor));
		ScreenQuad::Render(m_pAmbientLight.get(), pCommand);

		for (auto& light : pRenderQueue->GetRenderLights())
		{
			if (light.m_eType == LIGHT_DIRECTIONAL)
			{
				Uniform* pUniformDir = m_pDirLight->GetUniform(PS, "light_dir");
				Uniform* pUniformColor = m_pDirLight->GetUniform(PS, "light_color");

				m_pDirLight->SetValue(pUniformDir, Vector3::UNIT_SCALE.normalisedCopy());//light.m_vDir);
				m_pDirLight->SetValue(pUniformColor, light.m_cLightColor);

				ScreenQuad::Render(m_pDirLight.get(), pCommand);
			}
			else if (light.m_eType == LIGHT_POINT)
			{
				Uniform* pUniformColor = m_pPointLight->GetUniform(PS, "light_color");
				Uniform* pUniformPosRadius = m_pPointLight->GetUniform(PS, "light_pos_radius");

				Vector4 vPosRadius(light.m_vPos, light.m_fRadius);

				m_pPointLight->SetValue(pUniformColor, light.m_cLightColor);
				m_pPointLight->SetValue(pUniformPosRadius, vPosRadius);

				//float cameraToCenter = vPosES.length();
				//if (cameraToCenter < pPointLight->GetRadius())
				{
					//m_pPointLight->m_eCullMode = CULL_FACE_SIDE_FRONT;	
				}
				//else
				{
					//m_pPointLight->m_eCullMode = CULL_FACE_SIDE_BACK;
				}

				UnitSphere::Render(m_pPointLight.get(), light.m_vPos, light.m_fRadius, pCommand);
			}
		}

		pCommand->End();

		m_pLightPass->End();
	}

	void RenderScheme::HDRPass()
	{
		if (!m_bHDREnable)
			return;

		RenderQueue* pRenderQueue = m_pRenderQueue[GetRenderSystem()->CurThreadProcess()].get();

		g_pPostProcessPipeline->Render();

		m_pBackBaufferPass->Begine();

		m_lastStep->Render();

		pRenderQueue->Render(m_pBackBaufferPass.get(), RL_UI, RL_LAST);

		m_pBackBaufferPass->End();

	}

	void RenderScheme::Render()
	{
		BasePass();

		LightPass();

		HDRPass();
	}

}


