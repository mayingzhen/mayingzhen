#include "RenderSystem.h"
#include "RenderThread.h"
#include "RenderQueue.h"
#include "RenderContext.h"
#include "../RenderScheme/RenderScheme.h"
#include "../Renderable/ScreenQuad.h"
#include "../Renderable/UnitSphere.h"
#include "../Material/ShaderProgram.h"
#include "../Scene/Particle/ParticleSystem.h"



namespace ma
{
	static RenderSystem* gpRenderSystem = NULL;

	RenderSystem* GetRenderSystem()
	{
		return gpRenderSystem;
	}

	void SetRenderSystem(RenderSystem* pRenderSystem)
	{
		gpRenderSystem = pRenderSystem;
	}

	RenderSystem::RenderSystem()
	{
		m_pDepthStencil = NULL;
		for (UINT i = 0; i < MAX_RENDER_TARGET; ++i)
		{
			m_pRenderTarget[i] = NULL;
		}
		for (int i = 0; i < MAX_SHADOW_SAMPLES_NUM; ++i)
		{
			m_arrSampState[i] = NULL;
		}

		m_pCurVertexDecla = NULL;
		m_pCurVB = NULL;
		m_pCurIB = NULL;

		m_pRenderContext = new RenderContext();
		SetRenderContext(m_pRenderContext);

		LineRender* pLineRender = new LineRender();
		SetLineRender(pLineRender);

		m_pRenderThread = new RenderThread();

		m_bNeedReloadShader = false;

		m_hWnd = NULL;

		m_bThread = false;
		m_cClearClor = ColourValue::Black;
	}

	RenderSystem::~RenderSystem()
	{
		SAFE_DELETE(m_pRenderContext);

		SAFE_DELETE(m_pRenderThread);

		LineRender* pLineRender = GetLineRender();
		SAFE_DELETE(pLineRender);
		SetLineRender(NULL);
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
			GetResourceSystem()->ReLoad<ShaderProgram>();
			m_bNeedReloadShader = false;
		}

		for (UINT i = 0; i < m_arrScene.size(); ++i)
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

	void RenderSystem::OnFlushFrame()
	{
		for (UINT i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i]->OnFlushFrame();
		}

		if ( GetLineRender() )
			GetLineRender()->OnFlushFrame();

		if ( GetParticleSystem() )
			GetParticleSystem()->OnFlushFrame();
	}
	

	void RenderSystem::RT_ShutDown()
	{	
		for (UINT i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i] = NULL;
		}
		m_arrScene.clear();

		GetLineRender()->ShutDown();

		ScreenQuad::ShoutDown();
		UnitSphere::ShutDown();

		m_pDepthStencil = NULL;
		for (int i = 0; i < MAX_RENDER_TARGET; ++i)
		{
			m_pRenderTarget[i] = NULL;
		}
		for (int i = 0; i < MAX_SHADOW_SAMPLES_NUM; ++i)
		{
			m_arrSampState[i] = NULL;
		}

		GetRenderDevice()->ShutDown();

		m_hWnd = NULL;
	}

	void RenderSystem::InitGlobeMarco()
	{
		//m_mapMacros["ENCODENORMAL"] = "1";
		m_mapMacros["MAX_DQ_NUM_BONES"] = "100";
		m_mapMacros["MAX_MAT_NUM_BONES"] = "75";

		if (GetDeviceCapabilities()->GetDepthTextureSupported())
		{
			m_mapMacros["HWPCF"] = "1";
			m_mapMacros["HWDEPTH"] = "1";
		}
	}

	void RenderSystem::InitCachState()
	{
		GetRenderDevice()->SetColorWrite(m_curState.m_bColorWrite);
		GetRenderDevice()->SetDepthWrite(m_curState.m_bDepthWrite);
		GetRenderDevice()->SetCullingMode(m_curState.m_eCullMode);
		GetRenderDevice()->SetDepthCheckMode(m_curState.m_eDepthCheckMode);
		GetRenderDevice()->SetBlendMode(m_curState.m_eBlendMode);
	}


	void RenderSystem::RT_Init(HWND wndhandle)
	{
		m_hWnd = wndhandle;

		GetRenderDevice()->Init(wndhandle);
		
		InitGlobeMarco();

		InitCachState();

		for (int i = 0; i < MAX_RENDER_TARGET; ++i)
		{
			m_pRenderTarget[i] = GetRenderDevice()->GetRenderTarget(i);
		}
		m_pDepthStencil = GetRenderDevice()->GetDepthStencil();
		m_viewport = GetRenderDevice()->GetViewport();
	
		GetLineRender()->Init();
		ScreenQuad::Init();
		UnitSphere::Init();

		Scene* pScene = new Scene("defaultScene");
		pScene->SetViewport(m_viewport);
		m_arrScene.push_back(pScene);
	}

	void RenderSystem::RT_BeginFrame()
	{
		GetRenderDevice()->BeginRender();	

		GetRenderDevice()->ClearBuffer(true,true,true,m_cClearClor,1,0);
	}

	void RenderSystem::RT_EndFrame()
	{
		GetRenderDevice()->EndRender();
	}

	void RenderSystem::RT_Render()
	{
		for (UINT i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i]->Render();
		}

		OnFlushFrame();
	}

	void RenderSystem::DrawRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		m_pRenderThread->RC_DrawRenderable(pRenderable,pTechnique);
	}

	void RenderSystem::RT_DrawRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		if (pRenderable == NULL)
			return;

		RefPtr<SubMeshData>& pSubMesh = pRenderable->m_pSubMeshData;
		if (pSubMesh && pSubMesh->m_nVertexCount <= 0)
			return;

		if (m_pCurVertexDecla != pRenderable->m_pDeclaration)
		{
			GetRenderDevice()->SetVertexDeclaration(pRenderable->m_pDeclaration.get());

			m_pCurVertexDecla = pRenderable->m_pDeclaration.get();
		}

		if (m_pCurVB != pRenderable->m_pVertexBuffers)
		{
			GetRenderDevice()->SetVertexBuffer(0,pRenderable->m_pVertexBuffers.get());

			m_pCurVB = pRenderable->m_pVertexBuffers.get();
		}

		if (m_pCurIB != pRenderable->m_pIndexBuffer)
		{
			GetRenderDevice()->SetIndexBuffer(pRenderable->m_pIndexBuffer.get());

			m_pCurIB = pRenderable->m_pIndexBuffer.get();
		}

		GetRenderDevice()->DrawRenderable(pRenderable,pTechnique);

	}


	void RenderSystem::DrawDyRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		m_pRenderThread->RC_DrawDyRenderable(pRenderable,pTechnique);
	}

	void RenderSystem::RT_DrawDyRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		if (pRenderable == NULL)
			return;

		RefPtr<SubMeshData>& pSubMesh = pRenderable->m_pSubMeshData;
		if (pSubMesh && pSubMesh->m_nVertexCount <= 0)
			return;

		// 		m_pRenderContext->SetCurRenderObj(pRenderable);
		// 
		// 		pTechnique->Bind();

		if (m_pCurVertexDecla != pRenderable->m_pDeclaration)
		{
			GetRenderDevice()->SetVertexDeclaration(pRenderable->m_pDeclaration.get());

			m_pCurVertexDecla = pRenderable->m_pDeclaration.get();
		}

		// 		if (m_pCurVB != pRenderable->m_pVertexBuffers)
		// 		{
		// 			GetRenderDevice()->SetVertexBuffer(0,pRenderable->m_pVertexBuffers.get());
		// 
		// 			m_pCurVB = pRenderable->m_pVertexBuffers.get();
		// 		}
		// 
		// 		if (m_pCurIB != pRenderable->m_pIndexBuffer)
		// 		{
		// 			GetRenderDevice()->SetIndexBuffer(pRenderable->m_pIndexBuffer.get());
		// 
		// 			m_pCurIB = pRenderable->m_pIndexBuffer.get();
		// 		}

		GetRenderDevice()->DrawDyRenderable(pRenderable,pTechnique);

		//pTechnique->UnBind();
	}

	Texture* RenderSystem::CreateRenderTexture(int nWidth,int nHeight,PixelFormat format,USAGE use)
	{
		if (nWidth == -1 || nHeight == -1)
		{
			nWidth = (int)m_viewport.width;
			nHeight = (int)m_viewport.height;
		}
		Texture* pTarget = GetRenderDevice()->CreateTexture(nWidth,nHeight,format,use);
		m_pRenderThread->RC_CreateTexture(pTarget);
		return pTarget;
	}

	Texture* RenderSystem::CreateDepthStencil(int nWidth,int nHeight,PixelFormat format)
	{
		Texture* pTarget = GetRenderDevice()->CreateTexture(nWidth,nHeight,format,USAGE_DEPTHSTENCIL);
		m_pRenderThread->RC_CreateDepthStencil(pTarget);
		return pTarget;
	}

	RefPtr<Texture> RenderSystem::SetRenderTarget(RefPtr<Texture> pTexture,int index)
	{
		RefPtr<Texture> pPreTarget = m_pRenderTarget[index];
		
		m_pRenderThread->RC_SetRenderTarget(pTexture.get(),index);
		
		m_pRenderTarget[index] = pTexture;

		return pPreTarget;
	}

	RefPtr<Texture> RenderSystem::GetRenderTarget(int index)
	{
		return m_pRenderTarget[index];
	}

	RefPtr<Texture> RenderSystem::SetDepthStencil(RefPtr<Texture> pTexture)
	{
		RefPtr<Texture> pPreDepth = m_pDepthStencil;
		
		m_pRenderThread->RC_SetDepthStencil(pTexture.get());

		m_pDepthStencil = pTexture;

		return pPreDepth;
	}

	Rectangle RenderSystem::SetViewPort(const Rectangle& viewPort)
	{
		Rectangle preViewPort = m_viewport;

		m_pRenderThread->RC_SetViewPort(viewPort);

		m_viewport = viewPort;

		return preViewPort;
	}
	
	void RenderSystem::SetShaderProgram(ShaderProgram* pShader)
	{
		if (m_pCurShader != pShader)
		{
			m_pCurShader = pShader;
			m_pRenderThread->RC_SetShaderProgram(pShader);
		}
	}

	void RenderSystem::SetBlendMode(BLEND_MODE eBlendMode)
	{
		if (m_curState.m_eBlendMode != eBlendMode)
		{
			m_curState.m_eBlendMode = eBlendMode;
			m_pRenderThread->RC_SetBlendMode(eBlendMode);
		}
	}

	void RenderSystem::SetDepthCheckMode(DEPTH_CHECK_MODE eDepthCheckMode)
	{
		if (m_curState.m_eDepthCheckMode != eDepthCheckMode)
		{
			m_curState.m_eDepthCheckMode = eDepthCheckMode;
			m_pRenderThread->RC_SetDepthCheckMode(eDepthCheckMode);
		}
	}

	void RenderSystem::SetCullMode(CULL_MODE eCullMode)
	{
		if (m_curState.m_eCullMode != eCullMode)
		{
			m_curState.m_eCullMode = eCullMode;
			m_pRenderThread->RC_SetCullMode(eCullMode);
		}
	}

	void RenderSystem::SetDepthWirte(bool b)
	{
		if (m_curState.m_bDepthWrite != b)
		{
			m_curState.m_bDepthWrite = b;
			m_pRenderThread->RC_SetDepthWrite(b);
		}
	}

	void RenderSystem::SetColorWrite(bool b)
	{
		if (m_curState.m_bColorWrite != b)
		{
			m_curState.m_bColorWrite = b;
			m_pRenderThread->RC_SetColorWrite(b);
		}
	}

	void RenderSystem::SetDepthBias(float fDepthBias)
	{
		if ( abs(m_curState.m_fDepthBias - fDepthBias) > 0.0001f )
		{
			m_curState.m_fDepthBias = fDepthBias;
			m_pRenderThread->RC_SetDepthBias(fDepthBias);
		}
	}


	void RenderSystem::SetRenderState(const RenderState& state)
	{
		SetBlendMode(state.m_eBlendMode);

		SetCullMode(state.m_eCullMode);

		SetDepthCheckMode(state.m_eDepthCheckMode);
		
		SetDepthWirte(state.m_bDepthWrite);

		SetColorWrite(state.m_bColorWrite);

		SetDepthBias(state.m_fDepthBias);
	}

	void RenderSystem::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s)
	{
		m_pRenderThread->RC_ClearBuffer(bColor,bDepth,bStencil,c,z,s);
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

	void RenderSystem::BeginProfile(const char* pszLale)
	{
		m_pRenderThread->RC_BeginProfile(pszLale);
	}

	void RenderSystem::EndProfile()
	{
		m_pRenderThread->RC_EndProfile();	
	}

	RefPtr<IndexBuffer>	RenderSystem::CreateIndexBuffer(void* pData,UINT nSize,int nStride,USAGE eUsage)
	{
		IndexBuffer* pIB = GetRenderDevice()->CreateIndexBuffer();
		pIB->SetData(pData,nSize,nStride,eUsage);
		if (eUsage != USAGE_DYNAMIC)
			m_pRenderThread->RC_HardwareBufferStreamComplete(pIB);
		return pIB;
	}

	RefPtr<VertexBuffer> RenderSystem::CreateVertexBuffer(void* pData,UINT nSize,int nStride,USAGE eUsage)
	{
		VertexBuffer* pVB = GetRenderDevice()->CreateVertexBuffer();
		pVB->SetData(pData,nSize,nStride,eUsage);
		if (eUsage != USAGE_DYNAMIC)
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

		m_pRenderThread->RC_VertexDeclaComplete(pVD);

		return pVD;
	}

	void RenderSystem::SetValue(Uniform* uniform, float value)
	{
		if (uniform == NULL)
			return;

		m_pRenderThread->RC_SetFloat(uniform,value);
	}

	void RenderSystem::SetValue(Uniform* uniform, const Vector2& value)
	{
		if (uniform == NULL)
			return;

		m_pRenderThread->RC_SetVector2(uniform,value);
	}	

	void RenderSystem::SetValue(Uniform* uniform, const Vector3& value)
	{
		if (uniform == NULL)
			return;

		m_pRenderThread->RC_SetVector3(uniform,value);
	}

	void RenderSystem::SetValue(Uniform* uniform, const Vector4& value)
	{
		if (uniform == NULL)
			return;

		m_pRenderThread->RC_SetVector4(uniform,&value,1);
	}

	void RenderSystem::SetValue(Uniform* uniform, const Matrix4& value)
	{
		m_pRenderThread->RC_SetMatrix4(uniform,&value,1);
	}

	void RenderSystem::SetValue(Uniform* uniform, const Matrix4* values, UINT count)
	{
		if (values == NULL)
			return;

		m_pRenderThread->RC_SetMatrix4(uniform,values,count);
	}

	void RenderSystem::SetValue(Uniform* uniform, const Vector4* values, UINT count)
	{
		if (values == NULL)
			return;

		m_pRenderThread->RC_SetVector4(uniform,values,count);
	}

	void RenderSystem::SetValue(Uniform* uniform, const ColourValue& value)
	{
		if (uniform == NULL)
			return;

		m_pRenderThread->RC_SetColourValue(uniform,value);
	}

	void RenderSystem::SetValue(Uniform* uniform, Texture* pTexture)
	{
		ASSERT(uniform);
		ASSERT(pTexture);

		if ( ( m_arrSampState[uniform->m_index] != pTexture && pTexture->OnLoadOver() ) || pTexture->GetUsage() != USAGE_STATIC)
		{
			m_pRenderThread->RC_SetTexture(uniform,pTexture);

			m_arrSampState[uniform->m_index] = pTexture; 
		}
	}

	const char*	RenderSystem::GetShaderPath()
	{
		if (GetRenderDevice()->GetRenderDeviceType() == RenderDevice_D3D9)
		{
			return "shader/D3D9/";
		}
		else
		{
			return "shader/gles/";
		}
	}	

	bool RenderSystem::AddMacro(const char* pszKey, const char* pszValue)
	{
		const char* pszOldValue = this->GetMacro(pszKey);
		if (pszOldValue != NULL && strcmp(pszOldValue, pszValue) == 0)
		{
			return false;
		}

		ASSERT(pszKey != NULL);
		m_mapMacros[pszKey] = pszValue;

		ReloadShader();

		return true;
	}

	const char*	RenderSystem::GetMacro(const char* pszKey) const
	{
		MAP_STR_STR::const_iterator iter = m_mapMacros.find(pszKey);
		if (iter == m_mapMacros.end())
		{
			return NULL;
		}

		return iter->second.c_str();
	}

	uint32 RenderSystem::GetNumMacros() const
	{
		return m_mapMacros.size();
	}

	const char* RenderSystem::GetMacroByIndex(uint32 i, OUT const char*& pszValue) const
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

}

