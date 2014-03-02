#include "RenderSystem.h"
#include "RenderThread.h"
#include "RenderQueue.h"
#include "RenderContext.h"
#include "RenderSetting.h"
#include "../RenderScheme/RenderScheme.h"
#include "../Util/ScreenQuad.h"
#include "../Util/UnitSphere.h"


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

		m_pRenderContext = NULL;
		m_pRenderThread = NULL;
		m_pRenderQueue[0] = NULL;
		m_pRenderQueue[1] = NULL;

		m_pRenderContext = new RenderContext();
		SetRenderContext(m_pRenderContext);

		m_pRenderScheme = new RenderScheme();

		GameObject* pCameraObj = GetEntitySystem()->CreateGameObject("MainCamera");
		m_pMainCamera =  pCameraObj->CreateComponent<Camera>();

		m_pRenderQueue[0] = new RenderQueue();
		m_pRenderQueue[1] = new RenderQueue();

		LineRender* pLineRender = new LineRender();
		SetLineRender(pLineRender);

		m_pRenderThread = new RenderThread();

		if ( GetRenderSetting()->m_bRenderThread )
		{
			m_pRenderThread->Start();
		}
	}

	RenderSystem::~RenderSystem()
	{
		SAFE_DELETE(m_pRenderContext);

		GetEntitySystem()->DeleteGameObject(m_pMainCamera->GetGameObject());

		SAFE_DELETE(m_pRenderQueue[0]);
		SAFE_DELETE(m_pRenderQueue[1]);

		LineRender* pLineRender = GetLineRender();
		SAFE_DELETE(pLineRender);
	}

	Camera*	RenderSystem::GetMainCamera()
	{
		return m_pMainCamera;
	}

	void RenderSystem::SetMainCamera(Camera* pCamera)
	{
		m_pMainCamera = pCamera;
	}

	RenderScheme* RenderSystem::GetRenderScheme()
	{
		return m_pRenderScheme;
	}

	void RenderSystem::SetRenderScheme(RenderScheme* pRenderScheme)
	{
		SAFE_DELETE(m_pRenderScheme);
		m_pRenderScheme = pRenderScheme;
	}

	void RenderSystem::Init(HWND wndhandle)
	{
		m_pRenderThread->RC_Init(wndhandle);
	}

	void RenderSystem::Update()
	{
		profile_code();

// 		RenderQueueBuilder rqBuilder(m_pMainCamera);
// 		GetSceneSystem()->TravelScene(&rqBuilder);
// 
// 		rqBuilder.AddToRenderQueue();
		std::vector<GameObject*> arrGameObjs;

		// 1.裁剪得到视锥体内的所有物体
		Frustum camaeraFrustum;
		camaeraFrustum.Update( (m_pMainCamera->GetMatViewProj()).GetMatViewProj() );
		GetCullTree()->FindObjectsIn(&camaeraFrustum, arrGameObjs);

		for (UINT i = 0; i < arrGameObjs.size(); ++i)
		{
			//arrGameObjs[i]->UpdateTransform();

			RenderComponent* pRenderObj = arrGameObjs[i]->GetTypeComponentFirst<RenderComponent>();
			if (pRenderObj)
			{
				//pRenderObj->UpdateTransform();
				pRenderObj->AddToRenderQueue();
			}
		}

		//m_pMainCamera->AdjustPlanes( rqBuilder.GetWorldAABB() );
	}


	void RenderSystem::ShoutDown()
	{
		if (m_pRenderScheme)
			m_pRenderScheme->ShoutDown();

		GetLineRender()->ShutDown();

		ScreenQuad::ShoutDown();
		UnitSphere::ShutDown();
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
		GetRenderQueue()->Clear();

		GetLineRender()->OnFlushFrame();

		GetUISystem()->OnFlushFrame();

		if ( GetParticleSystem() )
			GetParticleSystem()->OnFlushFrame();
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
	
		GetRenderSetting()->Init();

		if (m_pRenderScheme)
			m_pRenderScheme->Init();

		GetLineRender()->Init();

		GetUISystem()->Init();

		ScreenQuad::Init();
		UnitSphere::Init();
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
		GetRenderContext()->SetCamera(m_pMainCamera);

		if (m_pRenderScheme)
			m_pRenderScheme->Render();
 		
 		GetLineRender()->Render();

		GetUISystem()->Render();
	}

	void RenderSystem::DrawRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		if (pRenderable == NULL)
			return;

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData;

		if (pSubMeshData->m_nVertexCount <= 0)
			return;

		m_pRenderContext->SetCurRenderObj(pRenderable);

		pTechnique->Bind();

		GetRenderDevice()->DrawRenderable(pRenderable,pTechnique);

		pTechnique->UnBind();
	}


	void RenderSystem::DrawDyRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		if (pRenderable == NULL)
			return;

		m_pRenderContext->SetCurRenderObj(pRenderable);

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData;

		if (pSubMeshData && pSubMeshData->m_nVertexCount <= 0)
			return;

		pTechnique->Bind();

		GetRenderDevice()->DrawDyRenderable(pRenderable,pTechnique);

		pTechnique->UnBind();
	}

	Texture* RenderSystem::CreateRenderTarget(int nWidth,int nHeight,FORMAT format,bool bDepthStencil/* = false*/)
	{
		Texture* pTarget = GetRenderDevice()->CreateTexture(nWidth,nHeight,format,bDepthStencil);
		m_pRenderThread->RC_CreateRenderTarget(pTarget);
		return pTarget;
	}

	ShaderProgram* RenderSystem::CreateShaderProgram(Technique* pTech,const char* pVSFile, const char* pPSFile,const char* pszDefine)
	{
		ShaderProgram* pShaderProgram = GetRenderDevice()->CreateShaderProgram(pTech,pVSFile,pPSFile,pszDefine);
		m_pRenderThread->RC_CreateShader(pShaderProgram);
		return pShaderProgram;
	}

	Texture* RenderSystem::SetRenderTarget(Texture* pTexture,int index)
	{
		Texture* pPreTarget = m_pRenderTarget[index];
		
		m_pRenderThread->RC_SetRenderTarget(pTexture,index);
		
		m_pRenderTarget[index] = pTexture;

		return pPreTarget;
	}

	Texture* RenderSystem::GetRenderTarget(int index)
	{
		return m_pRenderTarget[index];
	}

	Texture* RenderSystem::SetDepthStencil(Texture* pTexture)
	{
		Texture* pPreDepth = m_pDepthStencil;
		
		m_pRenderThread->RC_SetDepthStencil(pTexture);

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
	

	void RenderSystem::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s)
	{
		m_pRenderThread->RC_ClearBuffer(bColor,bDepth,bStencil,c,z,s);
	}

	void RenderSystem::TexStreamComplete(Texture* pTexture,DataStream* pDataStream)
	{
		m_pRenderThread->RC_TexStreamComplete(pTexture,pDataStream);
	}

}

