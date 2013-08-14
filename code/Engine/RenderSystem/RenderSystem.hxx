#include "RenderSystem.h"
#include "DeferredLight.h"
#include "Shadow.h"


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
		m_pCamera = NULL;
		m_pDefferLight = NULL;
		m_pShadow = NULL;
		m_bShadow = false;
		m_bDefferLight = true;
	}


	void RenderSystem::Init()
	{
		GetRenderDevice()->Init(Platform::GetInstance().GetWindId());

		ScreenQuad::Init();
		LineRender::Init();

		if (m_bShadow)
		{
			m_pShadow = new Shadow();
			m_pShadow->Init();
		}

		if (m_bDefferLight)
		{
			m_pDefferLight = new DeferredLight();
			m_pDefferLight->Init();
		}
	}

	void RenderSystem::ShoutDown()
	{
		ScreenQuad::ShoutDown();
		LineRender::ShutDown();
	}

	void RenderSystem::BeginFrame()
	{
		Color clearColor(0,45.0f / 255.0f,50.0f/255.0f,170.0f/255.0f);
		GetRenderDevice()->ClearBuffer(true,true,true,clearColor,1.0f,0);

		GetRenderDevice()->BeginRender();

		LineRender::Begin();
	}

	void RenderSystem::EndFrame()
	{
		GetRenderDevice()->EndRender();

		m_arrSolidEntry.clear();
		m_arrTransEntry.clear();
		m_arrLight.clear();
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

	void RenderSystem::Flush()
	{
		if (m_pDefferLight)
		{
			m_pDefferLight->DoRender();	
		}

		if (m_pShadow)
		{
			m_pShadow->DoRender();
		}

		// ShadingPass
		for (UINT i = 0; i < m_arrSolidEntry.size(); ++i)
		{
// 			if (m_pDefferLight)
// 			{
// 				Material* pMaterial = m_arrSolidEntry[i]->m_pMaterial;
// 				pMaterial->SetCurTechnqiue("default","DeferredLight");
// 			}
			

			GetRenderDevice()->DrawRenderable(m_arrSolidEntry[i]);
		}

		for (UINT i = 0; i < m_arrTransEntry.size(); ++i)
		{
			GetRenderDevice()->DrawRenderable(m_arrTransEntry[i]);
		}
	}

	void RenderSystem::AddRenderable(Renderable* pRenderable,bool bTrans)
	{
		if (bTrans)
		{
			m_arrTransEntry.push_back(pRenderable);
		}
		else
		{
			m_arrSolidEntry.push_back(pRenderable);
		}
	}

	void RenderSystem::AddLight(Light* pLight)
	{
		m_arrLight.push_back(pLight);
	}

}

