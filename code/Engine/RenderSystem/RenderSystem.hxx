#include "RenderSystem.h"
#include "RenderThread.h"
#include "RenderQueue.h"
#include "RenderContext.h"
#include "../RenderScheme/RenderScheme.h"
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
		m_pRenderContext = new RenderContext();
		SetRenderContext(m_pRenderContext);

		m_pRenderThread = new RenderThread();

		m_bNeedReloadShader = false;

		m_hWnd = NULL;

		m_nPoolIndex = 0;
		m_nPoolIndexRT = 0;

		m_bThread = false;
		m_cClearClor = ColourValue::Black;
	}

	RenderSystem::~RenderSystem()
	{
		SAFE_DELETE(m_pRenderContext);

		SAFE_DELETE(m_pRenderThread);
	}

	Scene* RenderSystem::GetScene(int index)
	{
		return m_arrScene[index].get();
	}

	void RenderSystem::Init(void* wndhandle, bool bThread)
	{
		m_bThread = bThread;
		if (bThread)
		{
			m_pRenderThread->Start();
		}

		m_pRenderThread->RC_Init(wndhandle);
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
		profile_code();

		if (m_bNeedReloadShader)
		{
			g_pMaterialManager->ReLoad();
			m_bNeedReloadShader = false;
		}

		UpdatePoolId();

		for (uint32_t i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i]->Update();
		}
	}

	void RenderSystem::BegineRender()
	{
		m_pRenderThread->RC_BeginRender();
	}

	void RenderSystem::EndRender()
	{
		m_pRenderThread->RC_EndRender();
	}

	void RenderSystem::Render()
	{
		m_pRenderThread->RC_Render();
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
		for (uint32_t i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i] = NULL;
		}
		m_arrScene.clear();

		LineRender::Shoutdown();
		ScreenQuad::Shoutdown();
		UnitSphere::Shoutdown();

		for (int i = 0; i < nNumParticleBuffer; ++i)
		{
			if (m_pParticleBuffer[i])
			{
				m_pParticleBuffer[i]->UnLockVideoMemory();
				m_pParticleBuffer[i] = NULL;
			}

			if (m_pInstanceBuffer[i])
			{
				m_pInstanceBuffer[i]->UnLockVideoMemory();
				m_pInstanceBuffer[i] = NULL;
			}
		}

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


	void RenderSystem::RT_Init(void* wndhandle)
	{
		m_hWnd = wndhandle;

		GetRenderDevice()->Init(wndhandle);
		
		InitGlobeMarco();

		InitCachState();

		InitParticleVideoMemory();

		m_curViewport = GetRenderDevice()->GetViewport();

		m_pDefaultRenderPass = GetRenderDevice()->GetDefaultRenderPass();
	
		LineRender::Init();
		ScreenQuad::Init();
		UnitSphere::Init();

		Scene* pScene = new Scene("defaultScene");
		pScene->GetRenderScheme()->Init();
		pScene->GetRenderScheme()->Reset();
		m_arrScene.push_back(pScene);

		m_pComputeCommd = GetRenderDevice()->CreateComputeCommand();
	}

	void RenderSystem::RT_Reset(uint32_t nWidth,uint32_t nHeight)
	{
		// Reset Main Scene
		m_arrScene[0]->Reset(nWidth,nHeight);
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

		// we render directly to a video memory buffer
		// we need to unlock it here in case we renderered a frame without any particles
		// lock the VMEM buffer for the next frame here (to prevent a lock in the mainthread)
		// NOTE: main thread is already working on buffer+1 and we want to prepare the next one => hence buffer+2
		LockParticleVideoMemory( (m_nPoolIndexRT + (nNumParticleBuffer - 1) ) % nNumParticleBuffer );	

		OnFlushFrame();
	}

	void RenderSystem::RT_Render()
	{
		for (uint32_t i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i]->Render();
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

		//m_pRenderThread->RC_VertexDeclaComplete(pVD);

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

		MAP_STR_STR::const_iterator iter = m_mapMacros.begin();
		std::advance(iter, i);
		pszValue = iter->second.c_str();
		return iter->first.c_str();
	}

	void RenderSystem::ReloadShader()
	{
		m_bNeedReloadShader = true;
	}

	struct UIVertex
	{
		Vector2 m_pos;
		Vector2 m_uv;
		uint32_t m_color;
	};

	void RenderSystem::InitParticleVideoMemory()
	{
		for (uint32_t i = 0; i < nNumParticleBuffer; ++i)
		{
			//m_pParticleBuffer[i] = new ParallHardWareBuffer(sizeof(ParticleSystemRenderable::VERTEX), 7680 * 4, 7680 * 6);
			//m_pParticleBuffer[i]->LockVideoMemory(); 

			m_pInstanceBuffer[i] = new ParallHardWareBuffer(sizeof(InstanceRenderable::InstaceData), 1024, 0);
			m_pInstanceBuffer[i]->LockVideoMemory();
		}
	}

	ParallHardWareBuffer* RenderSystem::GetParticleBuffer()
	{
		uint32_t nIndex = m_nPoolIndex % nNumParticleBuffer;
		return m_pParticleBuffer[nIndex].get();
	}

	ParallHardWareBuffer* RenderSystem::GetRTParticleBuffer()
	{
		uint32_t nIndex = m_nPoolIndexRT % nNumParticleBuffer;
		return m_pParticleBuffer[nIndex].get();
	}

	ParallHardWareBuffer* RenderSystem::GetInstanceBuffer()
	{
		uint32_t nIndex = m_nPoolIndex % nNumParticleBuffer;
		return m_pInstanceBuffer[nIndex].get();
	}

	ParallHardWareBuffer* RenderSystem::GetRTInstaneBuffer()
	{
		uint32_t nIndex = m_nPoolIndexRT % nNumParticleBuffer;
		return m_pInstanceBuffer[nIndex].get();
	}

	void RenderSystem::LockParticleVideoMemory(uint32_t nId)
	{
		profile_code();

		// unlock the particle VMEM buffer in case no particel were rendered(and thus no unlock was called)
		UnLockParticleVideoMemory(nId);	

		// lock video memory vertex/index buffer and expose base pointer and offset
		if(m_pParticleBuffer[nId])
		{
			m_pParticleBuffer[nId]->LockVideoMemory();
		}

		if (m_pInstanceBuffer[nId])
		{
			m_pInstanceBuffer[nId]->LockVideoMemory();
		}
	}

	void RenderSystem::UnLockParticleVideoMemory(uint32_t nId)
	{
		if(m_pParticleBuffer[nId])
		{
			m_pParticleBuffer[nId]->UnLockVideoMemory();
		}

		if (m_pInstanceBuffer[nId])
		{
			m_pInstanceBuffer[nId]->UnLockVideoMemory();
		}
	}

	void RenderSystem::RT_SetPoolId(uint32_t poolId)
	{
		m_nPoolIndexRT = poolId; 
	}

	void RenderSystem::UpdatePoolId()
	{
		m_pRenderThread->RC_SetPoolId(++m_nPoolIndex);
	}

	int RenderSystem::GetPoolId()
	{
		return m_nPoolIndex;
	}

	int	RenderSystem::GetPooIdRT()
	{
		return m_nPoolIndexRT;
	}

}


