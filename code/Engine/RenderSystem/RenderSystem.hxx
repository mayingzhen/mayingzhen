#include "RenderSystem.h"
#include "DeferredLight.h"
#include "Shadow.h"
#include "RenderThread.h"


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
		m_pRenderThread = NULL;

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
		int index = GetRenderThread()->CurThreadFill();
		m_arrSolidEntry[index].clear();
		m_arrTransEntry[index].clear();

		LineRender::OnFlushFrame();

		GetParticleManager()->OnFlushFrame();
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

	UINT		RenderSystem::GetSolidEntryNumber()
	{
		int index = m_pRenderThread->GetThreadList();
		return m_arrSolidEntry[index].size();
	}

	Renderable*	RenderSystem::GetSolidEntryByIndex(UINT i) 
	{
		int index = m_pRenderThread->GetThreadList();
		return m_arrSolidEntry[index][i];
	}

	void RenderSystem::DoRender()
	{
		m_pRenderThread->RC_Flush();
	}

	void RenderSystem::RT_Flush()
	{
		if (m_pDefferLight)
		{
			m_pDefferLight->DoRender();	
		}

		if (m_pShadow)
		{
			m_pShadow->DoRender();
		}

		ShadingPass();

		GetParticleManager()->Render();

		LineRender::Flush();

		GetUISystem()->Render();
	}

	void RenderSystem::ShadingPass()
	{
		int index = m_pRenderThread->m_nCurThreadProcess;
		std::vector<Renderable*>& arrEntry = m_arrSolidEntry[index];
		for (UINT i = 0; i < arrEntry.size(); ++i)
		{
			if (m_pDefferLight)
			{
				Material* pMaterial = arrEntry[i]->m_pMaterial;
				pMaterial->SetCurTechnqiue("default","DeferredLight");
			}

			DrawRenderable(arrEntry[i]);
		}

		arrEntry = m_arrTransEntry[index];
		for (UINT i = 0; i < arrEntry.size(); ++i)
		{
			DrawRenderable(arrEntry[i]);
		}
	}

	void RenderSystem::AddRenderable(Renderable* pRenderable,bool bTrans)
	{
		int index = m_pRenderThread->m_nCurThreadFill;
		if (bTrans)
		{
			m_arrTransEntry[index].push_back(pRenderable);
		}
		else
		{
			m_arrSolidEntry[index].push_back(pRenderable);
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

		IndexMesh indexMesh;
		indexMesh.m_eMeshType = pRenderable->m_ePrimitiveType;
		indexMesh.m_nIndexCount = pSubMeshData->m_nIndexCount;
		indexMesh.m_nVertexCount = pSubMeshData->m_nVertexCount;
		indexMesh.m_nVertexStart = pSubMeshData->m_nVertexStart;
		indexMesh.m_nIndexStart = pSubMeshData->m_nIndexStart;
		indexMesh.m_pDecl = pRenderable->m_pDeclaration;
		indexMesh.m_pIndBuf = pRenderable->m_pIndexBuffer;
		indexMesh.m_pVerBuf = pRenderable->m_pVertexBuffers;
		indexMesh.m_pTech = pRenderable->m_pMaterial->GetCurTechnqiue();

		GetRenderDevice()->DrawIndexMesh(indexMesh);

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

		IndexMesh indexMesh;
		indexMesh.m_eMeshType = pRenderable->m_ePrimitiveType;
		indexMesh.m_nIndexCount = pSubMeshData->m_nIndexCount;
		indexMesh.m_nVertexCount = pSubMeshData->m_nVertexCount;
		indexMesh.m_nVertexStart = pSubMeshData->m_nVertexStart;
		indexMesh.m_nIndexStart = pSubMeshData->m_nIndexStart;
		indexMesh.m_pDecl = pRenderable->m_pDeclaration;
		indexMesh.m_pIndBuf = pRenderable->m_pIndexBuffer;
		indexMesh.m_pVerBuf = pRenderable->m_pVertexBuffers;
		indexMesh.m_pTech = pRenderable->m_pMaterial->GetCurTechnqiue();

		GetRenderDevice()->DrawDyIndexMesh(indexMesh);

		pMaterial->UnBind();
	}

}

