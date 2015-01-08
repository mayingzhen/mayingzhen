#include "RenderSystem.h"
#include "RenderThread.h"
#include "RenderQueue.h"
#include "RenderContext.h"
#include "RenderSetting.h"
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

		m_pCurVertexDecla = NULL;
		m_pCurVB = NULL;
		m_pCurIB = NULL;

		m_pRenderContext = NULL;
		m_pRenderThread = NULL;
		m_pRenderQueue[0] = NULL;
		m_pRenderQueue[1] = NULL;

		m_pRenderContext = new RenderContext();
		SetRenderContext(m_pRenderContext);

		m_pRenderQueue[0] = new RenderQueue();
		m_pRenderQueue[1] = new RenderQueue();

		LineRender* pLineRender = new LineRender();
		SetLineRender(pLineRender);

		m_pRenderThread = new RenderThread();
	}

	RenderSystem::~RenderSystem()
	{
		SAFE_DELETE(m_pRenderContext);

		SAFE_DELETE(m_pRenderThread);

		SAFE_DELETE(m_pRenderQueue[0]);
		SAFE_DELETE(m_pRenderQueue[1]);

		LineRender* pLineRender = GetLineRender();
		SAFE_DELETE(pLineRender);
		SetLineRender(NULL);
	}

	Scene* RenderSystem::GetScene(int index)
	{
		return m_arrScene[index].get();
	}

	void RenderSystem::Init(HWND wndhandle)
	{
		if ( GetRenderSetting()->m_bRenderThread )
		{
			m_pRenderThread->Start();
		}

		m_pRenderThread->RC_Init(wndhandle);
	}

	void RenderSystem::ShoutDown()
	{
		m_pRenderThread->RC_ShutDown();

		if (GetRenderSetting()->m_bRenderThread)
			m_pRenderThread->Stop();
	}

	void RenderSystem::Update()
	{
		profile_code();

		for (UINT i = 0; i < m_arrScene.size(); ++i)
		{
			m_arrScene[i]->Update();
		}
	}

	RenderQueue* RenderSystem::GetRenderQueue()
	{
		int index = m_pRenderThread->GetThreadList();
		return m_pRenderQueue[index];
	}

	void RenderSystem::BeginFrame()
	{
		m_pRenderThread->RC_BeginFrame();
	}

	void RenderSystem::EndFrame()
	{
		profile_code();

		m_pRenderThread->RC_EndFrame();

		OnFlushFrame();
	}

	void RenderSystem::Render()
	{
		m_pRenderThread->RC_Render();
	}

	void RenderSystem::OnFlushFrame()
	{
		if ( GetRenderQueue() )
			GetRenderQueue()->Clear();

		if ( GetLineRender() )
			GetLineRender()->OnFlushFrame();

		if ( GetUISystem() )
			GetUISystem()->OnFlushFrame();

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
		
		//GetUISystem()->Shutdown();

		GetLineRender()->ShutDown();

		ScreenQuad::ShoutDown();
		UnitSphere::ShutDown();

		m_pDepthStencil = NULL;
		for (int i = 0; i < MAX_RENDER_TARGET; ++i)
		{
			m_pRenderTarget[i] = NULL;
		}
		GetRenderDevice()->ShutDown();

		m_pRenderQueue[0]->Clear();
		m_pRenderQueue[1]->Clear();

		m_hWnd = NULL;
	}

	void RenderSystem::RT_Init(HWND wndhandle)
	{
		m_hWnd = wndhandle;

		GetRenderDevice()->Init(wndhandle);

		for (int i = 0; i < MAX_RENDER_TARGET; ++i)
		{
			m_pRenderTarget[i] = GetRenderDevice()->GetRenderTarget(i);
		}
		m_pDepthStencil = GetRenderDevice()->GetDepthStencil();
		m_viewport = GetRenderDevice()->GetViewport();
	
		GetLineRender()->Init();

		//GetUISystem()->Init();

		ScreenQuad::Init();
		UnitSphere::Init();

		Scene* pScene = new Scene("defaultScene");
		pScene->SetViewport(m_viewport);
		m_arrScene.push_back(pScene);

		for (UINT i = 0; i < MAX_SAMPSTATE; ++i)
		{
			GetRenderDevice()->SetTextureFilter(i,m_arrSampState[i].GetFilterMode());
			GetRenderDevice()->SetTextureWrap(i,m_arrSampState[i].GetWrapMode());
		}
	}

	void RenderSystem::RT_BeginFrame()
	{
		GetRenderDevice()->BeginRender();	

		GetRenderDevice()->ClearBuffer(true,true,true,GetRenderSetting()->m_cClearClor,1,0);
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
	}

	void RenderSystem::DrawRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		if (pRenderable == NULL)
			return;
		
		RefPtr<SubMeshData>& pSubMesh = pRenderable->m_pSubMeshData;
		if (pSubMesh && pSubMesh->m_nVertexCount <= 0)
			return;

		m_pRenderContext->SetCurRenderObj(pRenderable);

		pTechnique->Bind();
		
		if (m_pCurVertexDecla != pRenderable->m_pDeclaration && pRenderable->m_pDeclaration->IsActive())
		{
			GetRenderDevice()->SetVertexDeclaration(pRenderable->m_pDeclaration.get());

			m_pCurVertexDecla = pRenderable->m_pDeclaration.get();
		}
		
		if (m_pCurVB != pRenderable->m_pVertexBuffers && pRenderable->m_pVertexBuffers->IsActive())
		{
			GetRenderDevice()->SetVertexBuffer(0,pRenderable->m_pVertexBuffers.get());

			m_pCurVB = pRenderable->m_pVertexBuffers.get();
		}
		
		if (m_pCurIB != pRenderable->m_pIndexBuffer && pRenderable->m_pIndexBuffer->IsActive())
		{
			GetRenderDevice()->SetIndexBuffer(pRenderable->m_pIndexBuffer.get());

			m_pCurIB = pRenderable->m_pIndexBuffer.get();
		}

		GetRenderDevice()->DrawRenderable(pRenderable,pTechnique);

		pTechnique->UnBind();
	}


	void RenderSystem::DrawDyRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		if (pRenderable == NULL)
			return;

		if (pRenderable->m_pSubMeshData->m_nVertexCount <= 0)
			return;

		m_pRenderContext->SetCurRenderObj(pRenderable);

		pTechnique->Bind();

		GetRenderDevice()->DrawDyRenderable(pRenderable,pTechnique);

		pTechnique->UnBind();
	}

	Texture* RenderSystem::CreateRenderTarget(int nWidth,int nHeight,PixelFormat format,bool bDepthStencil/* = false*/)
	{
		Texture* pTarget = GetRenderDevice()->CreateTexture(nWidth,nHeight,format,bDepthStencil ? USAGE_DEPTHSTENCIL : USAGE_RENDERTARGET);
		m_pRenderThread->RC_CreateRenderTarget(pTarget);
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
		if (m_pCurShader == pShader)
			return;

		pShader->Bind();

		m_pCurShader = pShader;
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

	RefPtr<IndexBuffer>	RenderSystem::CreateIndexBuffer(void* pData,UINT nSize,int nStride,USAGE eUsage)
	{
		IndexBuffer* pIB = GetRenderDevice()->CreateIndexBuffer();
		pIB->SetData(pData,nSize,nStride,eUsage);
		m_pRenderThread->RC_HardwareBufferStreamComplete(pIB);
		return pIB;
	}

	RefPtr<VertexBuffer> RenderSystem::CreateVertexBuffer(void* pData,UINT nSize,int nStride,USAGE eUsage)
	{
		VertexBuffer* pVB = GetRenderDevice()->CreateVertexBuffer();
		pVB->SetData(pData,nSize,nStride,eUsage);
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
		m_pRenderThread->RC_SetFloat(uniform,value);
	}

	void RenderSystem::SetValue(Uniform* uniform, const Matrix4& value)
	{
		m_pRenderThread->RC_SetMatrix4(uniform,value);
	}

	void RenderSystem::SetValue(Uniform* uniform, const Vector2& value)
	{
		m_pRenderThread->RC_SetVector2(uniform,value);
	}	

	void RenderSystem::SetValue(Uniform* uniform, const Vector3& value)
	{
		m_pRenderThread->RC_SetVector3(uniform,value);
	}

	void RenderSystem::SetValue(Uniform* uniform, const Vector4& value)
	{
		m_pRenderThread->RC_SetVector4(uniform,value);
	}

	void RenderSystem::SetValue(Uniform* uniform, const Vector4* values, UINT count)
	{
		m_pRenderThread->RC_SetVector4Count(uniform,values,count);
	}

	void RenderSystem::SetValue(Uniform* uniform, const ColourValue& value)
	{
		m_pRenderThread->RC_SetColourValue(uniform,value);
	}

	void RenderSystem::SetValue(Uniform* uniform, const SamplerState* sampler)
	{
		ASSERT(uniform && sampler);
		if (uniform == NULL || sampler == NULL)
			return;
			
		if (m_arrSampState[uniform->m_location].m_pTexture != sampler->m_pTexture && sampler->m_pTexture->IsInited() )
		{
			m_pRenderThread->RC_SetTexture(uniform,sampler->m_pTexture.get());

			m_arrSampState[uniform->m_location].m_pTexture = sampler->m_pTexture;
		}

		if (m_arrSampState[uniform->m_location].m_eWrap != sampler->m_eWrap)
		{
			m_pRenderThread->RC_SetTextureWrap(uniform->m_location,sampler->m_eWrap);

			m_arrSampState[uniform->m_location].m_eWrap = sampler->m_eWrap;
		}
		
		if (m_arrSampState[uniform->m_location].m_eFilter != sampler->m_eFilter)
		{
			m_pRenderThread->RC_SetTextureFilter(uniform->m_location,sampler->m_eFilter);

			m_arrSampState[uniform->m_location].m_eFilter = sampler->m_eFilter;
		}

	}

	void RenderSystem::SetValue(Uniform* uniform, const Texture* pTexture)
	{
		ASSERT(uniform);
		ASSERT(pTexture);

		if (m_arrSampState[uniform->m_location].m_pTexture != pTexture && pTexture->GetResState() == ResInited)
		{
			m_pRenderThread->RC_SetTexture(uniform,(Texture*)pTexture);

			m_arrSampState[uniform->m_location].m_pTexture = (Texture*)pTexture;
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

}

