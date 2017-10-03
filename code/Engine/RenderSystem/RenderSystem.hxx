#include "RenderSystem.h"
#include "RenderThread.h"
#include "RenderQueue.h"
#include "RenderContext.h"
#include "../RenderScheme/RenderScheme.h"
#include "../Renderable/ScreenQuad.h"
#include "../Renderable/UnitSphere.h"
#include "../Material/ShaderProgram.h"
#include "../Material/ShaderManager.h"




namespace ma
{
	RenderSystem* g_pRenderSystem = NULL;

	RenderSystem* GetRenderSystem()
	{
		return g_pRenderSystem;
	}

	RenderSystem::RenderSystem()
	{
		m_pDepthStencil = NULL;
		for (UINT i = 0; i < MAX_RENDER_TARGET; ++i)
		{
			m_pRenderTarget[i] = NULL;
		}
		for (int i = 0; i < MAX_TEXTURE_UNITS; ++i)
		{
			m_arrSampState[i] = NULL;
		}

		m_pCurVertexDecla = NULL;
		m_pCurVB = NULL;
		m_pCurIB = NULL;

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

	void RenderSystem::Init(HWND wndhandle, bool bThread)
	{
		m_bThread = bThread;
		if (bThread)
		{
			m_pRenderThread->Start();
		}

		m_pRenderThread->RC_Init(wndhandle);
	}

	void RenderSystem::Reset(uint32 nWidth, uint32 nHeight)
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
			g_pShaderManager->ReLoad();
			m_bNeedReloadShader = false;
		}

		UpdatePoolId();

		for (UINT i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i]->Update();
		}
	}

	void RenderSystem::BegineRender()
	{
		for (UINT32 i = 0; i < MAX_TEXTURE_UNITS; ++i)
		{
			m_arrSampState[i] = NULL;
		}
        
        m_pCurShader = NULL;

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

	void RenderSystem::OnFlushFrame()
	{
		for (UINT i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i]->OnFlushFrame();
		}

		LineRender::OnFlushFrame();
	}
	

	void RenderSystem::RT_ShutDown()
	{	
		for (UINT i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i] = NULL;
		}
		m_arrScene.clear();

		LineRender::Shoutdown();
		ScreenQuad::Shoutdown();
		UnitSphere::Shoutdown();

		m_pDepthStencil = NULL;
		for (int i = 0; i < MAX_RENDER_TARGET; ++i)
		{
			m_pRenderTarget[i] = NULL;
		}
		for (int i = 0; i < MAX_SHADOW_SAMPLES_NUM; ++i)
		{
			m_arrSampState[i] = NULL;
		}
		for (int i = 0; i < nNumParticleBuffer; ++i)
		{
			if (m_pParticleBuffer[i])
			{
				m_pParticleBuffer[i]->UnLockVideoMemory();
				m_pParticleBuffer[i] = NULL;
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
// 		GetRenderDevice()->SetColorWrite( m_curState.m_bColorWrite );
// 		GetRenderDevice()->SetDepthWrite( m_curState.m_bDepthWrite );
// 		GetRenderDevice()->SetCullingMode( m_curState.m_eCullMode );
// 		GetRenderDevice()->SetDepthCheckMode( m_curState.m_eDepthCheckMode );
// 		GetRenderDevice()->SetBlendMode( m_curState.m_eBlendMode );
// 		GetRenderDevice()->SetStencilEnable(m_curState.m_bStencil);
// 		GetRenderDevice()->SetStencilBufferParams(m_curState.m_eStencilfunc,
// 			m_curState.m_nStencilRefValue,m_curState.m_nStencilMask,
// 			m_curState.m_nStencilWriteMask,m_curState.m_eStencilFail,
// 			m_curState.m_eDepthFailOp,m_curState.m_eStencilPass);
	}


	void RenderSystem::RT_Init(HWND wndhandle)
	{
		m_hWnd = wndhandle;

		GetRenderDevice()->Init(wndhandle);
		
		InitGlobeMarco();

		InitCachState();

		InitParticleVideoMemory();

// 		for (int i = 0; i < MAX_RENDER_TARGET; ++i)
// 		{
// 			m_pRenderTarget[i] = GetRenderDevice()->GetDefaultRenderTarget(i);
// 		}
// 		m_pDepthStencil = GetRenderDevice()->GetDefaultDepthStencil();
		m_curViewport = GetRenderDevice()->GetViewport();
	
		LineRender::Init();
		ScreenQuad::Init();
		UnitSphere::Init();

		Scene* pScene = new Scene("defaultScene");
		pScene->GetRenderScheme()->Init();
		pScene->GetRenderScheme()->Reset();
		pScene->SetViewport(m_curViewport);
		m_arrScene.push_back(pScene);
	}

	void RenderSystem::RT_Reset(uint32 nWidth,uint32 nHeight)
	{
		// Reset Main Scene
		m_arrScene[0]->Reset(nWidth,nHeight);
	}

	void RenderSystem::RT_BeginRender()
	{
        GetRenderDevice()->BeginRender();
		
//         for (uint32 i = 0; i < MAX_TEXTURE_UNITS; ++i)
// 		{
// 			GetRenderDevice()->SetTexture(i,NULL,true);
// 		}
	}

	void RenderSystem::RT_EndRender()
	{
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
		for (UINT i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i]->Render();
		}
	}

// 	void RenderSystem::DrawRenderable(Renderable* pRenderable,Technique* pTechnique)
// 	{
// 		m_pRenderThread->RC_DrawRenderable(pRenderable,pTechnique);
// 	}

// 	void RenderSystem::RT_DrawRenderable(Renderable* pRenderable,Technique* pTechnique)
// 	{
// 		if (pRenderable == NULL)
// 			return;
// 
// 		RefPtr<SubMeshData>& pSubMesh = pRenderable->m_pSubMeshData;
// 		if (pSubMesh && pSubMesh->m_nVertexCount <= 0)
// 			return;
// 
// 		//if (m_pCurVB != pRenderable->m_pVertexBuffer)
// 		{
// 			GetRenderDevice()->SetVertexBuffer(0,pRenderable->m_pVertexBuffer.get(),pRenderable->m_pCommand);
// 
// 			m_pCurVB = pRenderable->m_pVertexBuffer.get();
// 		}
// 
// 		//if (m_pCurIB != pRenderable->m_pIndexBuffer)
// 		{
// 			GetRenderDevice()->SetIndexBuffer(pRenderable->m_pIndexBuffer.get(), pRenderable->m_pCommand);
// 
// 			m_pCurIB = pRenderable->m_pIndexBuffer.get();
// 		}
// 
// 		GetRenderDevice()->DrawRenderable(pRenderable,pTechnique);
// 
// 	}

	RefPtr<Texture> RenderSystem::CreateRenderTarget(int nWidth,int nHeight,UINT32 nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType)
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

	RefPtr<Texture> RenderSystem::CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess)
	{
		if (nWidth == -1 || nHeight == -1)
		{
			nWidth = (int)m_curViewport.width();
			nHeight = (int)m_curViewport.height();
		}

		Texture* pTarget = GetRenderDevice()->CreateDepthStencil(nWidth,nHeight,format,bTypeLess);
		m_pRenderThread->RC_CreateTexture(pTarget);
		return pTarget;
	}

// 	void RenderSystem::BeginRenderPass(FrameBuffer* pFB)
// 	{
// 		m_pRenderThread->RC_BeginRenderPass(pFB);
// 	}
// 
// 	void RenderSystem::EndRenderPass(FrameBuffer* pFB)
// 	{
// 		m_pRenderThread->RC_EndRenderPass(pFB);
// 	}
// 
// 	void RenderSystem::SetFrameBuffer(FrameBuffer* pFB)
// 	{
// 		m_pRenderThread->RC_SetFrameBuffer(pFB);
// 	}

// 	RefPtr<Texture> RenderSystem::SetRenderTarget(int index,Texture* pTexture, int level, int array_index, int face)
// 	{
// 		RefPtr<Texture> pPreTarget = m_pRenderTarget[index];
// 		
// 		m_pRenderThread->RC_SetRenderTarget(index,pTexture,level,array_index,face);
// 		
// 		m_pRenderTarget[index] = pTexture;
// 
// 		return pPreTarget;
// 	}


	RefPtr<Texture> RenderSystem::GetRenderTarget(int index)
	{
		return m_pRenderTarget[index];
	}

// 	RefPtr<Texture> RenderSystem::GetDefaultRenderTarget()
// 	{
// 		return GetRenderDevice()->GetDefaultRenderTarget();
// 	}
// 
// 	RefPtr<Texture> RenderSystem::GetDefaultDepthStencil()
// 	{
// 		return GetRenderDevice()->GetDefaultDepthStencil();
// 	}

// 	RefPtr<Texture> RenderSystem::SetDepthStencil(RefPtr<Texture> pTexture)
// 	{
// 		RefPtr<Texture> pPreDepth = m_pDepthStencil;
// 		
// 		m_pRenderThread->RC_SetDepthStencil(pTexture.get());
// 
// 		m_pDepthStencil = pTexture.get();
// 
// 		return pPreDepth;
// 	}

// 	Rectangle RenderSystem::SetViewPort(const Rectangle& viewPort)
// 	{
// 		Rectangle preViewPort = m_curViewport;
// 
// 		m_pRenderThread->RC_SetViewPort(viewPort);
// 
// 		m_curViewport = viewPort;
// 
// 		return preViewPort;
// 	}

	RenderPass* RenderSystem::GetDefaultRenderPass()
	{
		return GetRenderDevice()->GetDefaultRenderPass();
	}
	
// 	void RenderSystem::SetShaderProgram(ShaderProgram* pShader)
// 	{
// 		if (m_pCurShader != pShader)
// 		{
// 			m_pCurShader = pShader;
// 			m_pRenderThread->RC_SetShaderProgram(pShader);
// 		}
// 	}
// 
// 	void RenderSystem::SetBlendState(const BlendState* pBlendState)
// 	{
// 		m_pRenderThread->RC_SetBlendState(pBlendState);
// 	}
// 
// 	void RenderSystem::SetDepthStencilState(const DepthStencilState* pDSState, UINT nStencilRef)
// 	{
// 		m_pRenderThread->RC_SetDepthStencilState(pDSState, nStencilRef);
// 	}
// 
// 	void RenderSystem::SetVertexDeclaration(const VertexDeclaration* pVertexDecl)
// 	{
// 		m_pRenderThread->RC_SetVertexDeclation(pVertexDecl);
// 	}
// 
// 	void RenderSystem::SetRasterizerState(const RasterizerState* pRSState)
// 	{
// 		m_pRenderThread->RC_SetRasterizerState(pRSState);
// 	}
// 
// 	void RenderSystem::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s)
// 	{
// 		m_pRenderThread->RC_ClearBuffer(bColor,bDepth,bStencil,c,z,s);
// 	}

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

	void RenderSystem::BlendStateStreamComplete(BlendState* pBSState)
	{
		m_pRenderThread->RC_BlendStateStreamComplete(pBSState);
	}

	void RenderSystem::DepthStencilStateStreamComplete(DepthStencilState* pDSState)
	{
		m_pRenderThread->RC_DepthStencilStateStreamComplete(pDSState);
	}

	void RenderSystem::RasterizerStateStreamComplete(RasterizerState* pRSState)
	{
		m_pRenderThread->RC_RasterizerStateStreamComplete(pRSState);
	}

	void RenderSystem::BeginProfile(const char* pszLale)
	{
		m_pRenderThread->RC_BeginProfile(pszLale);
	}

	void RenderSystem::EndProfile()
	{
		m_pRenderThread->RC_EndProfile();	
	}

	RefPtr<IndexBuffer>	RenderSystem::CreateIndexBuffer(uint8* pData,UINT nSize,int nStride,HBU_USAGE eUsage,bool bShadowData)
	{
		IndexBuffer* pIB = GetRenderDevice()->CreateIndexBuffer();
		pIB->SetData(pData,nSize,nStride,eUsage,bShadowData);
		m_pRenderThread->RC_HardwareBufferStreamComplete(pIB);
		return pIB;
	}

	RefPtr<VertexBuffer> RenderSystem::CreateVertexBuffer(uint8* pData,UINT nSize,int nStride,HBU_USAGE eUsage,bool bShadowData)
	{
		VertexBuffer* pVB = GetRenderDevice()->CreateVertexBuffer();
		pVB->SetData(pData,nSize,nStride,eUsage,bShadowData);
		m_pRenderThread->RC_HardwareBufferStreamComplete(pVB);
		return pVB;
	}

	RefPtr<VertexDeclaration> RenderSystem::CreateVertexDeclaration(VertexElement* arrElememt,uint32 nCount)
	{
		VertexDeclaration* pVD = GetRenderDevice()->CreateVertexDeclaration();
		for (UINT i = 0 ; i < nCount; ++i)
		{
			pVD->AddElement(arrElememt[i]);	
		}

		//m_pRenderThread->RC_VertexDeclaComplete(pVD);

		return pVD;
	}

// 	void RenderSystem::SetValue(Uniform* uniform, int value)
// 	{
// 		if (uniform == NULL)
// 			return;
// 
// 		m_pRenderThread->RC_SetInt(uniform,value);
// 	}
// 
// 	void RenderSystem::SetValue(Uniform* uniform, float value)
// 	{
// 		if (uniform == NULL)
// 			return;
// 
// 		m_pRenderThread->RC_SetFloat(uniform,value);
// 	}
// 
// 	void RenderSystem::SetValue(Uniform* uniform, const Vector2& value)
// 	{
// 		if (uniform == NULL)
// 			return;
// 
// 		m_pRenderThread->RC_SetVector2(uniform,value);
// 	}	
// 
// 	void RenderSystem::SetValue(Uniform* uniform, const Vector3& value)
// 	{
// 		if (uniform == NULL)
// 			return;
// 
// 		m_pRenderThread->RC_SetVector3(uniform,value);
// 	}
// 
// 	void RenderSystem::SetValue(Uniform* uniform, const Vector4& value)
// 	{
// 		if (uniform == NULL)
// 			return;
// 
// 		m_pRenderThread->RC_SetVector4(uniform,&value,1);
// 	}
// 
// 	void RenderSystem::SetValue(Uniform* uniform, const Matrix4& value)
// 	{
// 		m_pRenderThread->RC_SetMatrix4(uniform,&value,1);
// 	}
// 
// 	void RenderSystem::SetValue(Uniform* uniform, const Matrix4* values, UINT count)
// 	{
// 		if (values == NULL)
// 			return;
// 
// 		m_pRenderThread->RC_SetMatrix4(uniform,values,count);
// 	}
// 
// 	void RenderSystem::SetValue(Uniform* uniform, const Vector4* values, UINT count)
// 	{
// 		if (values == NULL)
// 			return;
// 
// 		m_pRenderThread->RC_SetVector4(uniform,values,count);
// 	}
// 
// 	void RenderSystem::SetValue(Uniform* uniform, const ColourValue& value)
// 	{
// 		if (uniform == NULL)
// 			return;
// 
// 		m_pRenderThread->RC_SetColourValue(uniform,value);
// 	}
// 
// 	void RenderSystem::SetValue(Uniform* uniform, Texture* pTexture)
// 	{
// 		ASSERT(uniform);
// 		//ASSERT(pTexture);
// 
// 		//if ( m_arrSampState[uniform->m_index]->GetTexture() != pTexture )
// 		{
// 			m_pRenderThread->RC_SetTexture(uniform,pTexture);
// 		}
// 	}
// 
// 	void RenderSystem::SetValue(Uniform* uniform, SamplerState* pTexture)
// 	{
// 		ASSERT(uniform);
// 		ASSERT(pTexture);
// 
// 		//if ( m_arrSampState[uniform->m_index] != pTexture )
// 		{
// 			m_pRenderThread->RC_SetSamplerState(uniform,pTexture);
// 
// 			m_arrSampState[uniform->GetIndex()] = pTexture; 
// 		}
// 	}

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
			return "shader/gles/";
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

	uint32 RenderSystem::GetNumShaderGlobaMacro() const
	{
		return m_mapMacros.size();
	}

	const char* RenderSystem::GetShaderGlobaMacroByIndex(uint32 i, OUT const char*& pszValue) const
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

	void RenderSystem::InitParticleVideoMemory()
	{
		for (uint32 i = 0; i < nNumParticleBuffer; ++i)
		{
			//m_pParticleBuffer[i] = new ParallHardWareBuffer(sizeof(ParticleSystemRenderable::VERTEX), 7680 * 4, 7680 * 6);
			//m_pParticleBuffer[i]->LockVideoMemory(); 
		}
	}

	ParallHardWareBuffer* RenderSystem::GetParticleBuffer()
	{
		uint32 nIndex = m_nPoolIndex % nNumParticleBuffer;
		return m_pParticleBuffer[nIndex].get();
	}

	ParallHardWareBuffer* RenderSystem::GetRTParticleBuffer()
	{
		uint32 nIndex = m_nPoolIndexRT % nNumParticleBuffer;
		return m_pParticleBuffer[nIndex].get();
	}

	void RenderSystem::LockParticleVideoMemory(uint32 nId)
	{
		profile_code();

		// unlock the particle VMEM buffer in case no particel were rendered(and thus no unlock was called)
		UnLockParticleVideoMemory(nId);	

		// lock video memory vertex/index buffer and expose base pointer and offset
		if(m_pParticleBuffer[nId])
		{
			m_pParticleBuffer[nId]->LockVideoMemory();
		}
	}

	void RenderSystem::UnLockParticleVideoMemory(uint32 nId)
	{
		if(m_pParticleBuffer[nId])
		{
			m_pParticleBuffer[nId]->UnLockVideoMemory();
		}
	}

	void RenderSystem::RT_SetPoolId(uint32 poolId)
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

	bool RenderSystem::ResizeInstancingBuffer(unsigned numInstances)
	{
		//if (!instancingBuffer_ || !dynamicInstancing_)
		//	return false;

		unsigned oldSize = instancingBuffer_->GetSize() / instancingBuffer_->GetStride();
		if (numInstances <= oldSize)
			return true;

		static const int INSTANCING_BUFFER_DEFAULT_SIZE = 1024;

		unsigned newSize = INSTANCING_BUFFER_DEFAULT_SIZE;
		while (newSize < numInstances)
			newSize <<= 1;

// 		const PODVector<VertexElement> instancingBufferElements = CreateInstancingBufferElements(numExtraInstancingBufferElements_);
// 		if (!instancingBuffer_->SetSize(newSize, instancingBufferElements, true))
// 		{
// 			URHO3D_LOGERROR("Failed to resize instancing buffer to " + String(newSize));
// 			// If failed, try to restore the old size
// 			instancingBuffer_->SetSize(oldSize, instancingBufferElements, true);
// 			return false;
// 		}

		instancingBuffer_ = GetRenderSystem()->CreateVertexBuffer(NULL, newSize, sizeof(Matrix3x4));


		return true;
	}

}

