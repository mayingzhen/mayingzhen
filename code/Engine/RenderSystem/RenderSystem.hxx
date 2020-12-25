#include "RenderSystem.h"
#include "RenderThread.h"
#include "RenderContext.h"
#include "RenderView.h"
#include "../Renderable/ScreenQuad.h"
#include "../Renderable/UnitSphere.h"
#include "../Material/ShaderProgram.h"
#include "../Material/ShaderManager.h"
#include "InstanceRenderable.h"
#include "../FrameGraph/FrameGraph.h"
#include "../FrameGraph/ResourceAllocator.h"



namespace ma
{

	RenderSystem* g_pRenderSystem = NULL;

	RenderSystem* GetRenderSystem()
	{
		return g_pRenderSystem;
	}

	RenderSystem::RenderSystem()
	{
		m_pRenderThread = new RenderThread();

		m_bNeedReloadShader = false;

		m_bDefferedRender = true;

		m_hWnd = NULL;

		m_bThread = false;
	}

	RenderSystem::~RenderSystem()
	{
		SAFE_DELETE(m_pRenderThread);
	}

	Scene* RenderSystem::GetScene()
	{
		return m_scene.get();
	}

	void RenderSystem::Init(void* wndhandle, int width, int height, bool bThread)
	{
		m_bThread = bThread;
		if (bThread)
		{
			m_pRenderThread->Start();
		}

		m_pRenderThread->RC_Init(wndhandle,width,height);
	}

	void RenderSystem::Reset(uint32_t nWidth, uint32_t nHeight)
	{
		m_pRenderThread->RC_Reset(nWidth,nHeight);
	}

	void RenderSystem::Shoutdown()
	{
		m_pRenderThread->RC_ShutDown();

		if (m_bThread)
			m_pRenderThread->Stop();
	}

	void RenderSystem::Update()
	{
		if (m_bNeedReloadShader)
		{
			MICROPROFILE_SCOPEI("", "Material::ReLoad", 0);
			GetRenderSystem()->FlushAndWait(); // TechniqueStreamComplete 可能还在RenderThread
			g_pMaterialManager->ReLoad();
			m_bNeedReloadShader = false;
		}

		m_scene->Update();

		for (auto& it : m_renderView)
		{
			it->Update();
		}
	}

	void RenderSystem::BegineRender()
	{
		m_pRenderThread->UpdateRenderIndex();

		m_pRenderThread->RC_BeginRender();
	}

	void RenderSystem::EndRender()
	{
		m_pRenderThread->RC_EndRender();

		m_pRenderThread->SyncRenderFrame();
	}

	void RenderSystem::Render()
	{
		MICROPROFILE_SCOPEI("", "RenderSystem::Render", 0);

		this->BegineRender();

		m_pRenderThread->RC_AddRenderCommad( [this]() {
			this->RT_Render();
		});

		this->EndRender();
	}

	void RenderSystem::BegineCompute()
	{
		GetRenderDevice()->BegineCompute();
	}

	void RenderSystem::EndCompute()
	{
		GetRenderDevice()->EndCompute();
	}

	void RenderSystem::OnFlushFrame()
	{
		LineRender::OnFlushFrame();
	}
	

	void RenderSystem::RT_ShutDown()
	{	
		m_scene = nullptr;

		LineRender::Shoutdown();
		ScreenQuad::Shoutdown();
		UnitSphere::Shoutdown();

		GetRenderDevice()->Shoutdown();

		m_hWnd = NULL;
	}

	void RenderSystem::InitGlobeMarco()
	{
		m_mapMacros["USING_SHADOW"] = "0";
		m_mapMacros["SHADOW_BLUR"] = "0";
		m_mapMacros["USING_HW_PCF"] = "1";
		m_mapMacros["ENCODENORMAL"] = "1";
		m_mapMacros["MAX_DQ_NUM_BONES"] = "100";
		m_mapMacros["MAX_MAT_NUM_BONES"] = "75";

		m_mapMacros["DEFERREDSHADING"] = m_bDefferedRender ? "1" : "0";
	}

	void RenderSystem::InitCachState()
	{
	}


	void RenderSystem::RT_Init(void* wndhandle, int width, int height)
	{
		m_hWnd = wndhandle;

		GetRenderDevice()->Init(wndhandle);
		
		InitGlobeMarco();

		InitCachState();

		m_curViewport = GetRenderDevice()->GetViewport();

		m_pBaseRenderPass = GetRenderDevice()->GetBackBufferRenderPass();
		m_pBackBufferRenderPass = m_pBaseRenderPass;
	
		LineRender::Init();
		ScreenQuad::Init();
		UnitSphere::Init();

// 		if (m_bDefferedRender)
// 		{
// 			m_pRenderScheme = new MainRenderStep();
// 		}
// 		else
// 		{
// 			m_pRenderScheme = new RenderStep();
// 		}
// 		m_pRenderScheme->m_pRenderPass = m_pBackBufferRenderPass;

		m_pComputeCommd = GetRenderDevice()->CreateComputeCommand();

		RefPtr<MainRenderView> pMainView = new MainRenderView();
		pMainView->m_name = "MainView";

		m_scene = new Scene("defaultScene");
		pMainView->m_pScene = m_scene;
		pMainView->m_pCamera = m_scene->GetCamera();

		AddRenderView(pMainView.get());
	}

	void RenderSystem::RT_Reset(uint32_t nWidth,uint32_t nHeight)
	{
		// Reset Main Scene
		m_scene->Reset(nWidth,nHeight);

		//m_pRenderScheme->Reset();
	}

	void RenderSystem::RT_BeginRender()
	{
        GetRenderDevice()->BeginRender();

		//GetRenderDevice()->BegineCompute();
	}

	void RenderSystem::RT_EndRender()
	{
		//GetRenderDevice()->EndCompute();

		GetRenderDevice()->EndRender();

		OnFlushFrame();
	}

	//MICROPROFILE_DECLARE(RT_Render);

	void RenderSystem::RT_Render()
	{
		//SYSTRACE(RT_Render);
		MICROPROFILE_SCOPEI("", "RenderSystem::RT_Render", 0);


		for (auto& render_view : m_renderView)
		{
			render_view->Render();
		}

		// 		for (size_t i = 0; i < m_renderStepList.size(); ++i)
		// 		{
		// 			RenderPass* prePass = (i > 0) ? m_renderStepList[i - 1]->m_pRenderPass.get() : nullptr;
		// 			RenderPass* nextPass = i + 1 < m_renderStepList.size() ? m_renderStepList[i + 1]->m_pRenderPass.get() : nullptr;
		// 
		// 			m_renderStepList[i]->Render(prePass, nextPass);
		// 		}



		static ResourceAllocator* reource_alloc = new ResourceAllocator();

		FrameGraph fg(*reource_alloc);

		std::string name = "final_out";
		FrameGraphTexture::Descriptor desc;
		FrameGraphTexture resc;
		resc.texture = nullptr;
		FrameGraphId<FrameGraphTexture> final_out = fg.import<FrameGraphTexture>("", desc, resc);

		for (size_t i = 0; i < m_renderStepList.size(); ++i)
		{
			struct StepPassData
			{
				RefPtr<RenderStep> step;
				RenderPass* prePass =  nullptr;
				RenderPass* nextPass = nullptr;
			};

			auto& stepPass = fg.addPass<StepPassData>("color",
				[&](FrameGraph::Builder& builder, StepPassData& data)
				{
					RenderPass* pRenderPass = m_renderStepList[i]->m_pRenderPass.get();

					if (pRenderPass == GetRenderSystem()->GetBackBufferRenderPass())
					{
						final_out = builder.write(final_out);
					}
					else
					{
						std::string name = "";
						FrameGraphTexture::Descriptor desc;
						FrameGraphTexture resc;
						resc.texture = pRenderPass->m_arrColor[0].m_pTexture;
						FrameGraphId<FrameGraphTexture> fgRT = fg.import<FrameGraphTexture>("", desc, resc);
						fgRT = builder.write(fgRT);
					}

					for (auto& it : m_renderStepList[i]->m_pReadTextue)
					{
						std::string name = "";
						FrameGraphTexture::Descriptor desc;
						FrameGraphTexture resc;
						resc.texture = it;
						FrameGraphId<FrameGraphTexture> fgRT = fg.import<FrameGraphTexture>("", desc, resc);
						fgRT = builder.read(fgRT);
					}

					//builder.sideEffect();
				
					data.step = m_renderStepList[i];
					data.prePass = (i > 0) ? m_renderStepList[i - 1]->m_pRenderPass.get() : nullptr;
					data.nextPass = i + 1 < m_renderStepList.size() ? m_renderStepList[i + 1]->m_pRenderPass.get() : nullptr;

					
				},
				[=](FrameGraphPassResources const& resources, StepPassData const& data) {

					data.step->Render(data.prePass, data.nextPass);
				}
			);
		}


		//FrameGraphId<FrameGraphTexture> input = colorPassOutput;
		//auto output = input;
		fg.present(final_out);
		//fg.moveResource(fgViewRenderTarget, output);
		fg.compile();
		//fg.export_graphviz(slog.d, view.getName());
		fg.execute();

	}

	RefPtr<Texture> RenderSystem::CreateRenderTarget(int nWidth,int nHeight,uint32_t nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType)
	{
		if (nWidth == -1 || nHeight == -1)
		{
			nWidth = (int)m_curViewport.width();
			nHeight = (int)m_curViewport.height();
		}
		Texture* pTarget = GetRenderDevice()->CreateRenderTarget(nWidth,nHeight,nMipMap,format,bSRGB,eType);
		m_pRenderThread->RC_CreateTexture(pTarget);
		return pTarget;
	}

	RefPtr<Texture> RenderSystem::CreateDepthStencil(int nWidth,int nHeight,PixelFormat format)
	{
		if (nWidth == -1 || nHeight == -1)
		{
			nWidth = (int)m_curViewport.width();
			nHeight = (int)m_curViewport.height();
		}

		Texture* pTarget = GetRenderDevice()->CreateDepthStencil(nWidth,nHeight,format);
		m_pRenderThread->RC_CreateTexture(pTarget);
		return pTarget;
	}

	void RenderSystem::SetDefferedLightRenderPass(RenderPass* pRenderPass)
	{
		m_pDefferedLightPass = pRenderPass;
	}

	RenderPass* RenderSystem::GetDefferedLightRenderPass()
	{
		return m_pDefferedLightPass.get();
	}

	void RenderSystem::SetBaseRenderPass(RenderPass* pRenderPass)
	{
		m_pBaseRenderPass = pRenderPass;
	}

	RenderPass* RenderSystem::GetBaseRenderPass()
	{
		return m_pBaseRenderPass.get();
	}

// 	void RenderSystem::SetBackBufferRenderPass(RenderPass* pRenderPass)
// 	{
// 		m_pBackBufferRenderPass = pRenderPass;
// 	}

	RenderPass*	RenderSystem::GetBackBufferRenderPass()
	{
		return m_pBackBufferRenderPass.get();
	}

	void RenderSystem::TechniqueStreamComplete(Technique* pTech)
	{
		m_pRenderThread->RC_TechniqueStreamComplete(pTech);
	}

	void RenderSystem::TexStreamComplete(Texture* pTexture)
	{
		m_pRenderThread->RC_TexStreamComplete(pTexture);
	}

	void RenderSystem::ShaderStreamComplete(ShaderProgram* pShader)
	{
		m_pRenderThread->RC_ShaderStreamComplete(pShader);
	}

	void RenderSystem::VertexDeclaComplete(VertexDeclaration* pDec)
	{
		m_pRenderThread->RC_VertexDeclaComplete(pDec);
	}

	void RenderSystem::HardwareBufferStreamComplete(HardwareBuffer* pHB)
	{
		m_pRenderThread->RC_HardwareBufferStreamComplete(pHB);
	}

	void RenderSystem::RenderPassStreamComplete(RenderPass* pRenderPass)
	{
		m_pRenderThread->RC_RenderPassStreamComplete(pRenderPass);
	}

	void RenderSystem::UpdteHardwareBuffer(HardwareBuffer* pHB, const void* data, uint32_t nSize)
	{
		m_pRenderThread->RC_UpdateHardwareBuffer(pHB, data, nSize);
	}

	void RenderSystem::SetUniformValue(Uniform* pUniform, const void* data, uint32_t nSize)
	{
		m_pRenderThread->RC_SetUniformValue(pUniform,data,nSize);
	}

	void RenderSystem::SetSampler(Uniform* pUniform, SamplerState* pSampler)
	{
		m_pRenderThread->RC_SetSampler(pUniform, pSampler);
	}

	void RenderSystem::SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer)
	{
		m_pRenderThread->RC_SetStorageBuffer(pUniform, pBuffer);
	}

	void RenderSystem::BeginProfile(const char* pszLale)
	{
		m_pRenderThread->RC_BeginProfile(pszLale);
	}

	void RenderSystem::EndProfile()
	{
		m_pRenderThread->RC_EndProfile();	
	}

	RefPtr<IndexBuffer>	RenderSystem::CreateIndexBuffer(uint8_t* pData,uint32_t nSize,int nStride,HBU_USAGE eUsage)
	{
		IndexBuffer* pIB = GetRenderDevice()->CreateIndexBuffer();
		pIB->SetData(pData,nSize,nStride,eUsage);
		m_pRenderThread->RC_HardwareBufferStreamComplete(pIB);
		return pIB;
	}

	RefPtr<VertexBuffer> RenderSystem::CreateVertexBuffer(uint8_t* pData,uint32_t nSize,int nStride,HBU_USAGE eUsage)
	{
		VertexBuffer* pVB = GetRenderDevice()->CreateVertexBuffer();
		pVB->SetData(pData,nSize,nStride,eUsage);
		m_pRenderThread->RC_HardwareBufferStreamComplete(pVB);
		return pVB;
	}

	RefPtr<VertexDeclaration> RenderSystem::CreateVertexDeclaration(VertexElement* arrElememt,uint32_t nCount)
	{
		VertexDeclaration* pVD = GetRenderDevice()->CreateVertexDeclaration();
		for (uint32_t i = 0 ; i < nCount; ++i)
		{
			pVD->AddElement(arrElememt[i]);	
		}

		return pVD;
	}


	const char*	RenderSystem::GetShaderPath()
	{
		return "shader/hlsl/";
	}	

	bool RenderSystem::AddShaderGlobaMacro(const char* pszKey, const char* pszValue)
	{
		const char* pszOldValue = this->GetShaderGlobaMacro(pszKey);
		if (pszOldValue != NULL && strcmp(pszOldValue, pszValue) == 0)
		{
			return false;
		}

		ASSERT(pszKey != NULL);
		m_mapMacros[pszKey] = pszValue;

		ReloadShader();

		return true;
	}

	const char*	RenderSystem::GetShaderGlobaMacro(const char* pszKey) const
	{
		MAP_STR_STR::const_iterator iter = m_mapMacros.find(pszKey);
		if (iter == m_mapMacros.end())
		{
			return NULL;
		}

		return iter->second.c_str();
	}

	uint32_t RenderSystem::GetNumShaderGlobaMacro() const
	{
		return m_mapMacros.size();
	}

	const char* RenderSystem::GetShaderGlobaMacroByIndex(uint32_t i, OUT const char*& pszValue) const
	{
		if (i >= m_mapMacros.size())
		{
			ASSERT(false);
			return NULL;
		}

		auto iter = m_mapMacros.begin();
		std::advance(iter, i);
		pszValue = iter->second.c_str();
		return iter->first.c_str();
	}

	void RenderSystem::ReloadShader()
	{
		m_bNeedReloadShader = true;
	}

	void RenderSystem::AddRenderStep(RefPtr<RenderStep> renderstep)
	{
		m_renderStepList.push_back(renderstep);
	}

	void RenderSystem::AddRenderView(RenderView* pRenderView)
	{
		m_pRenderThread->RC_AddRenderCommad([this,pRenderView]() {
			m_renderView.push_back(pRenderView);
			}
		);
	}

}



