#include "RenderSystem.h"
#include "RenderThread.h"
#include "RenderQueue.h"
#include "RenderContext.h"
#include "RenderSetting.h"
#include "../RenderScheme/RenderScheme.h"
#include "../Util/ScreenQuad.h"
#include "../Util/UnitSphere.h"
#include "RenderQueueBuilder.h"


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
		m_pRenderContext = NULL;
		m_pRenderThread = NULL;
		m_pRenderQueue[0] = NULL;
		m_pRenderQueue[1] = NULL;

		m_pRenderContext = new RenderContext();
		SetRenderContext(m_pRenderContext);

		GameObject* pCameraObj = GetEntitySystem()->CreateGameObject("MainCamera");
		m_pMainCamera =  pCameraObj->CreateComponent<Camera>();

		m_pRenderQueue[0] = new RenderQueue();
		m_pRenderQueue[1] = new RenderQueue();

		LineRender* pLineRender = new LineRender();
		SetLineRender(pLineRender);
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

	void RenderSystem::Init()
	{
		if ( GetRenderSetting()->m_bThread )
		{
			m_pRenderThread = new RenderThread();
			SetRenderThread(m_pRenderThread);
			m_pRenderThread->Init();
		}

		GetRenderThread()->RC_Init();

		GetRenderSetting()->Init();
	}

	void RenderSystem::Update()
	{
		RenderQueueBuilder rqBuilder(m_pMainCamera);
		GetSceneSystem()->TravelScene(&rqBuilder);

		rqBuilder.AddToRenderQueue();

		m_pMainCamera->AdjustPlanes( rqBuilder.GetWorldAABB() );
	}

	void RenderSystem::RT_Init()
	{
		GetRenderDevice()->Init(Platform::GetInstance().GetWindId());

		GetRenderScheme()->Init();

		GetLineRender()->Init();

		ScreenQuad::Init();
		UnitSphere::Init();
	}

	void RenderSystem::ShoutDown()
	{
		GetRenderScheme()->ShoutDown();

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
		m_pRenderThread->RC_EndFrame();
	}

	void RenderSystem::OnFlushFrame()
	{
		GetRenderQueue()->Clear();

		GetLineRender()->OnFlushFrame();

		GetParticleSystem()->OnFlushFrame();
	}



	void RenderSystem::RT_BeginFrame()
	{
		GetRenderDevice()->BeginRender();	

		//GetRenderDevice()->ClearBuffer(true,true,true,m_cClearClor,1.0f,0);
	}

	void RenderSystem::RT_EndFrame()
	{
		GetRenderDevice()->EndRender();
	}

	void RenderSystem::Render()
	{
		m_pRenderThread->RC_Render();
	}

	void RenderSystem::RT_Render()
	{
		GetRenderContext()->SetCamera(m_pMainCamera);

		GetRenderScheme()->Render();
 		
 		GetLineRender()->Render();

		GetUISystem()->Render();
	}

	void RenderSystem::DrawRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		if (pRenderable == NULL)
			return;

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData;

		if (pSubMeshData && pSubMeshData->m_nVertexCount <= 0)
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
		GetRenderThread()->RC_CreateRenderTarget(pTarget);
		return pTarget;
	}

	ShaderProgram* RenderSystem::CreateShaderProgram(Technique* pTech,const char* pVSFile, const char* pPSFile,const char* pszDefine)
	{
		ShaderProgram* pShaderProgram = GetRenderDevice()->CreateShaderProgram(pTech,pVSFile,pPSFile,pszDefine);
		GetRenderThread()->RC_CreateShader(pShaderProgram);
		return pShaderProgram;
	}

	void RenderSystem::PushRenderTarget(Texture* pTexture,int index)
	{
		GetRenderThread()->RC_PushRenderTarget(pTexture,index);
	}

	void RenderSystem::PushDepthStencil(Texture* pTexture)
	{
		GetRenderThread()->RC_PushDepthStencil(pTexture);
	}

	void RenderSystem::PushViewPort(Rectangle& viewPort)
	{
		GetRenderThread()->RC_PushViewPort(viewPort);
	}

	void RenderSystem::PopRenderTargert(int index)
	{
		GetRenderThread()->RC_PopRenderTargert(index);
	}
	
	void RenderSystem::PopDepthStencil()
	{
		GetRenderThread()->RC_PopDepthStencil();
	}

	void RenderSystem::PopViewPort()
	{
		GetRenderThread()->RC_PopViewPort();
	}

	void RenderSystem::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s)
	{
		GetRenderThread()->RC_ClearBuffer(bColor,bDepth,bStencil,c,z,s);
	}

	void RenderSystem::TexStreamComplete(Texture* pTexture,DataStream* pDataStream)
	{
		GetRenderThread()->RC_TexStreamComplete(pTexture,pDataStream);
	}

}

