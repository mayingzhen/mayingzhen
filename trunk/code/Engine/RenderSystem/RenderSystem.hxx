#include "RenderSystem.h"
#include "DeferredLight.h"
#include "Shadow.h"
#include "RenderThread.h"
#include "RenderQueue.h"


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
		m_pDefferLight = NULL;
		m_pShadow = NULL;
		m_pRenderThread = NULL;
		m_pRenderQueue = NULL;

		m_bShadow = false; 
		m_bDefferLight = false;
		m_bThread = true;

		m_cAmbientColor = Vector4(1,1,1,1);

		m_cClearClor = Color(0,45.0f / 255.0f,50.0f/255.0f,170.0f/255.0f);
	}


	void RenderSystem::Init()
	{
		if (m_bThread)
		{
			m_pRenderThread = new RenderThread();
			SetRenderThread(m_pRenderThread);
			m_pRenderThread->Init();
		}

		m_pRenderQueue = new RenderQueue();
		SetRenderQueue(m_pRenderQueue);

		GetRenderThread()->RC_Init();
	}

	void RenderSystem::RT_Init()
	{
		GetRenderDevice()->Init(Platform::GetInstance().GetWindId());

		if (m_bShadow)
		{
			m_pShadow = new Shadow();
			m_pShadow->Init();
		}

		if (m_bDefferLight)
		{
			m_pDefferLight = new DeferredLight();
			SetDeferredLight(m_pDefferLight);
			m_pDefferLight->Init();
		}

		LineRender* pLineRender = new LineRender();
		SetLineRender(pLineRender);
		pLineRender->Init();

		ScreenQuad::Init();
		UnitSphere::Init();
	}

	void RenderSystem::ShoutDown()
	{
		LineRender* pLineRender = GetLineRender();
		GetLineRender()->ShutDown();
		SetLineRender(NULL);
		SAFE_DELETE(pLineRender);

		ScreenQuad::ShoutDown();
		UnitSphere::ShutDown();
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
		m_pRenderQueue->Clear();

		GetLineRender()->OnFlushFrame();

		GetParticleSystem()->OnFlushFrame();
	}

	void RenderSystem::RT_BeginFrame()
	{
		GetRenderDevice()->BeginRender();	

		GetRenderDevice()->ClearBuffer(true,true,true,m_cClearClor,1.0f,0);
	}

	void RenderSystem::RT_EndFrame()
	{
		GetRenderDevice()->EndRender();
	}

	void RenderSystem::SetCamera(Camera* pCamera)
	{
		m_matView = pCamera->GetViewMatrix();
		m_matProj = pCamera->GetProjMatrix();
		m_fFar = pCamera->GetFarClip();
		m_fNear = pCamera->GetNearClip();
	}


	void RenderSystem::Render()
	{
		m_pRenderThread->RC_Render();
	}

	void RenderSystem::RT_Render()
	{
		if (m_pShadow)
		{
			m_pShadow->ShadowDepthPass();
		}

		GetMaterialManager()->SetCurViewPojectMat(m_matView,m_matProj);

		if (m_pDefferLight)
		{
			m_pDefferLight->GBufferPass();
		}

		if (m_pShadow)
		{
			m_pShadow->DeferredShadow();
		}

		if (m_pDefferLight)
		{
			m_pDefferLight->DefferedLighting();
		}

		ShadingPass();

		GetLineRender()->Render();

		GetUISystem()->Render();
	}

	void RenderSystem::ShadingPass()
	{
		RENDER_PROFILE(ShadingPass);

		GetRenderDevice()->ClearBuffer(true,true,true,m_cClearClor, 1.0f, 0);

		UINT nSolid = m_pRenderQueue->GetRenderObjNumber(RL_Solid);
		for (UINT i = 0; i < nSolid; ++i)
		{
			RenderObject* pRenderObj = m_pRenderQueue->GetRenderObjByIndex(RL_Solid,i);
			if (pRenderObj == NULL)
				continue;

			Material* pMaterial = pRenderObj->GetMaterial();

			Technique* pTech = pMaterial->GetTechnqiue("Shading");

			pRenderObj->Render(pTech);
		}

		UINT nTrans = m_pRenderQueue->GetRenderObjNumber(RL_Trans);
		for (UINT i = 0; i < nTrans; ++i)
		{
			RenderObject* pRenderObj = m_pRenderQueue->GetRenderObjByIndex(RL_Trans,i);
			if (pRenderObj == NULL)
				continue;

			Material* pMaterial = pRenderObj->GetMaterial();

			Technique* pTech = pMaterial->GetTechnqiue("Shading");

			pRenderObj->Render(pTech);
		}
	}

	void RenderSystem::AddLight(Light* pLight)
	{
		m_arrLight.push_back(pLight);
	}


	void RenderSystem::DrawRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		if (pRenderable == NULL)
			return;

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData;

		if (pSubMeshData && pSubMeshData->m_nVertexCount <= 0)
			return;

		GetMaterialManager()->SetCurRenderable(pRenderable);

		pTechnique->Bind();

		GetRenderDevice()->DrawRenderable(pRenderable,pTechnique);

		pTechnique->UnBind();
	}


	void RenderSystem::DrawDyRenderable(Renderable* pRenderable,Technique* pTechnique)
	{
		if (pRenderable == NULL)
			return;

		GetMaterialManager()->SetCurRenderable(pRenderable);

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

	ShaderProgram* RenderSystem::CreateShaderProgram(Technique* pTech,const char* pszName,const char* pszDefine)
	{
		ShaderProgram* pShaderProgram = GetRenderDevice()->CreateShaderProgram(pTech,pszName,pszDefine);
		GetRenderThread()->RC_CreateShader(pShaderProgram);
		return pShaderProgram;
	}

	void RenderSystem::PushRenderTarget(Texture* pTexture)
	{
		GetRenderThread()->RC_PushRenderTarget(pTexture);
	}

	void RenderSystem::PushDepthStencil(Texture* pTexture)
	{
		GetRenderThread()->RC_PushDepthStencil(pTexture);
	}

	void RenderSystem::PushViewPort(Rectangle& viewPort)
	{
		GetRenderThread()->RC_PushViewPort(viewPort);
	}

	void RenderSystem::PopRenderTargert()
	{
		GetRenderThread()->RC_PopRenderTargert();
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

