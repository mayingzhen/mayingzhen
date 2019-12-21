#include "RenderSystem.h"
#include "RenderThread.h"
#include "RenderContext.h"
#include "../Renderable/ScreenQuad.h"
#include "../Renderable/UnitSphere.h"
#include "../Material/ShaderProgram.h"
#include "../Material/ShaderManager.h"
#include "InstanceRenderable.h"



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

		m_hWnd = NULL;

		m_bThread = false;
	}

	RenderSystem::~RenderSystem()
	{
		SAFE_DELETE(m_pRenderThread);
	}

	Scene* RenderSystem::GetScene(int index)
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
			g_pMaterialManager->ReLoad();
			m_bNeedReloadShader = false;
		}

		m_scene->Update();
	}

	void RenderSystem::BegineRender()
	{
		m_pRenderThread->FlushFrame();

		m_renderStepList[CurThreadFill()].clear();

		for (auto& it : m_vecDyHBuffer)
		{
			it->BeginFrame();
		}

		m_pRenderThread->RC_BeginRender();
	}

	void RenderSystem::EndRender()
	{
		for (auto& it : m_vecDyHBuffer)
		{
			it->EndFrame();
		}

		m_pRenderThread->RC_EndRender();
	}

	void RenderSystem::Render()
	{
		this->BegineRender();

		m_scene->Render();

		m_pRenderThread->RC_AddRenderCommad( []() {
			GetRenderSystem()->RT_Render();
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

		m_pDefaultRenderPass = GetRenderDevice()->GetDefaultRenderPass();
		m_pBackBufferRenderPass = m_pDefaultRenderPass;
	
		LineRender::Init();
		ScreenQuad::Init();
		UnitSphere::Init();

		m_scene = new Scene("defaultScene");

		m_pRenderScheme = new RenderScheme();
		m_pRenderScheme->Init();
		m_pRenderScheme->Reset();

		m_pComputeCommd = GetRenderDevice()->CreateComputeCommand();

		RenderQueue* pRQ = m_scene->GetRenderQueue();
		SetRenderContext(pRQ->GetRenderContext());
	}

	void RenderSystem::RT_Reset(uint32_t nWidth,uint32_t nHeight)
	{
		// Reset Main Scene
		m_scene->Reset(nWidth,nHeight);

		m_pRenderScheme->Reset();
	}

	void RenderSystem::RT_BeginRender()
	{
        GetRenderDevice()->BeginRender();

		GetRenderDevice()->BegineCompute();
	}

	void RenderSystem::RT_EndRender()
	{
		GetRenderDevice()->EndCompute();

		GetRenderDevice()->EndRender();

		OnFlushFrame();
	}

	//MICROPROFILE_DECLARE(RT_Render);

	void RenderSystem::RT_Render()
	{
		SYSTRACE(RT_Render);

		uint32_t nCurProcess = CurThreadProcess();

		for (auto& renderStep : m_renderStepList[nCurProcess])
		{
			RenderQueue* cur_renderQueue = renderStep.m_pRenderQueue.get();
			RenderPass*  cur_renderPass = renderStep.m_pRenderPass.get();

 			SetRenderContext(cur_renderQueue->GetRenderContext());

			cur_renderPass->Begine();
 		
			cur_renderQueue->Render(cur_renderPass);

			cur_renderPass->End();
		}
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

	void RenderSystem::SetDefaultRenderPass(RenderPass* pRenderPass)
	{
		m_pDefaultRenderPass = pRenderPass;
	}

	RenderPass* RenderSystem::GetDefaultRenderPass()
	{
		return m_pDefaultRenderPass.get();
	}

	void RenderSystem::SetBackBufferRenderPass(RenderPass* pRenderPass)
	{
		m_pBackBufferRenderPass = pRenderPass;
	}

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

	RefPtr<IndexBuffer>	RenderSystem::CreateIndexBuffer(uint8_t* pData,uint32_t nSize,int nStride,HBU_USAGE eUsage,bool bShadowData)
	{
		IndexBuffer* pIB = GetRenderDevice()->CreateIndexBuffer();
		pIB->SetData(pData,nSize,nStride,eUsage,bShadowData);
		m_pRenderThread->RC_HardwareBufferStreamComplete(pIB);
		return pIB;
	}

	RefPtr<VertexBuffer> RenderSystem::CreateVertexBuffer(uint8_t* pData,uint32_t nSize,int nStride,HBU_USAGE eUsage,bool bShadowData)
	{
		VertexBuffer* pVB = GetRenderDevice()->CreateVertexBuffer();
		pVB->SetData(pData,nSize,nStride,eUsage,bShadowData);
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
		RenderDeviceType type = GetRenderDevice()->GetRenderDeviceType();
		if (type == RenderDevice_METAL)
        {
            return "shader/hlsl/";
        }
		else if (type == RenderDevice_VULKAN)
		{
			return "shader/hlsl/";
		}
        else
		{
			return "shader/hlsl/";
		}
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

	RefPtr<TransientParallHardWareBuffer> RenderSystem::CreateTransientParallHardWareBuffer(uint32_t nVertexStride, uint32_t nNumVertice, uint32_t numIndexes)
	{
		RefPtr<TransientParallHardWareBuffer> pBufferr = new TransientParallHardWareBuffer(nVertexStride, nNumVertice, numIndexes);
		m_vecDyHBuffer.push_back(pBufferr);
		return pBufferr;
	}

	void RenderSystem::AddRenderStep(RenderQueue* pQueue, RenderPass* pPass)
	{
		m_renderStepList[CurThreadFill()].emplace_back();
		RenderStep& step = m_renderStepList[CurThreadFill()].back();
		step.m_pRenderQueue = pQueue;
		step.m_pRenderPass = pPass;
	}

}



