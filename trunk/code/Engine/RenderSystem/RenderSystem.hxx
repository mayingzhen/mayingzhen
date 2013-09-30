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

		ScreenQuad::Init();
		LineRender::Init();
		UnitSphere::Init();
	}

	void RenderSystem::ShoutDown()
	{
		ScreenQuad::ShoutDown();
		LineRender::ShutDown();
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

		LineRender::OnFlushFrame();

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

	RenderMesh*	RenderSystem::CreatRenderMesh(const char* pMeshPath,const char* pDiffueTexture)
	{
		RenderMesh* pRenderMesh = new RenderMesh();
		pRenderMesh->Load(pMeshPath,pDiffueTexture);
		m_arrRenderMesh.push_back(pRenderMesh);

		return pRenderMesh;
	}

	void RenderSystem::DeleteRenderMesh(RenderMesh* pRenderMesh)
	{
		std::vector<RenderMesh*>::iterator it;
		it = std::find(m_arrRenderMesh.begin(),m_arrRenderMesh.end(),pRenderMesh);
		ASSERT(it != m_arrRenderMesh.end());
		if (it == m_arrRenderMesh.end())
		{
			SAFE_DELETE(pRenderMesh);
			return;
		}

		SAFE_DELETE(pRenderMesh);
		m_arrRenderMesh.erase(it);	
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

		if (m_pDefferLight)
		{
			m_pDefferLight->GBufferPass();

			m_pDefferLight->DefferedLighting();
		}

		if (m_pShadow)
		{
			m_pShadow->DeferredShadow();
		}

		ShadingPass();

		LineRender::Render();

		GetUISystem()->Render();
	}

	void RenderSystem::ShadingPass()
	{
		UINT nSolid = m_pRenderQueue->GetRenderObjNumber(RL_Solid);
		for (UINT i = 0; i < nSolid; ++i)
		{
			RenderObject* pRenderObj = m_pRenderQueue->GetRenderObjByIndex(RL_Solid,i);
			if (pRenderObj == NULL)
				continue;

			if (m_pDefferLight)
			{
				Material* pMaterial = pRenderObj->GetMaterial();
				pMaterial->SetCurTechnqiue("default","DeferredLight");
			}

			pRenderObj->Render();	
		}

		UINT nTrans = m_pRenderQueue->GetRenderObjNumber(RL_Trans);
		for (UINT i = 0; i < nTrans; ++i)
		{
			RenderObject* pRenderObj = m_pRenderQueue->GetRenderObjByIndex(RL_Trans,i);
			if (pRenderObj == NULL)
				continue;
			
			pRenderObj->Render();	
		}
	}

	void RenderSystem::AddLight(Light* pLight)
	{
		m_arrLight.push_back(pLight);
	}


	void RenderSystem::DrawRenderable(Renderable* pRenderable)
	{
		if (pRenderable == NULL)
			return;

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData;

		if (pSubMeshData && pSubMeshData->m_nVertexCount <= 0)
			return;

		GetMaterialManager()->SetCurRenderable(pRenderable);

		Material* pMaterial = pRenderable->m_pMaterial;
		ASSERT(pMaterial);
		pMaterial->Bind();

		GetRenderDevice()->DrawRenderable(pRenderable);

		pMaterial->UnBind();

	}


	void RenderSystem::DrawDyRenderable(Renderable* pRenderable)
	{
		if (pRenderable == NULL)
			return;

		GetMaterialManager()->SetCurRenderable(pRenderable);

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData;

		if (pSubMeshData && pSubMeshData->m_nVertexCount <= 0)
			return;

		Material* pMaterial = pRenderable->m_pMaterial;
		ASSERT(pMaterial);
		pMaterial->Bind();

		GetRenderDevice()->DrawDyRenderable(pRenderable);

		pMaterial->UnBind();
	}

}

