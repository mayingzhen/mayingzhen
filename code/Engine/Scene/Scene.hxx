#include "Scene.h"
#include "Octree.h"
#include "ParallelCull.h"

namespace ma
{
	Scene::Scene(const char* pszName)
	{
		m_pRootNode = new SceneNode(this);
		m_sName = pszName ? pszName : "";

		m_pCamera = CreateCamera();
		m_pRootNode->AddChild(m_pCamera.get());

		m_pCullTree = new ParallelCull();

		m_pRenderQueue[0] = new RenderQueue();
		m_pRenderQueue[1] = new RenderQueue();

		m_pRenderScheme = new RenderScheme(this);
		
		m_viwMinZ = 0.0f;
		m_viwMaxZ = 0.0f;
		m_pCallback = NULL;	

		SceneNode* pMainLightNode = CreateSceneNode();
		m_pRootNode->AddChild(pMainLightNode);
		m_pMainDirLight = CreateDirectonalLight();
		pMainLightNode->AddComponent(m_pMainDirLight.get());

		m_cAmbientColor = Vector3::ZERO;
	}

	Scene::~Scene()
	{
		SAFE_DELETE(m_pRenderQueue[0]);
		SAFE_DELETE(m_pRenderQueue[1]);
	}

	void Scene::Reset(uint32_t nWidth,uint32_t nHeight)
	{
		float fAspect = (float)nWidth / (float)nHeight;
		float fFOV = m_pCamera->GetFov();
		float fNearClip = m_pCamera->GetNearClip();
		float fFarClip = m_pCamera->GetFarClip();
		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);

		m_pRenderScheme->Reset();
	}

	SceneNode* Scene::CreateSceneNode()
	{
		RefPtr<SceneNode> pSceneNode = new SceneNode(NULL,NULL);
		m_pRootNode->AddChild( pSceneNode.get() );
		return pSceneNode.get();
	}

	SceneNode* Scene::CreateSceneNode(const char* pPath)
	{
		RefPtr<SceneNode> pSceneNode = new SceneNode(NULL,NULL);
		pSceneNode->LoadFromXML(pPath);
		m_pRootNode->AddChild( pSceneNode.get() );
		return pSceneNode.get();
	}

	void Scene::AddParallelUpdate(Component* pComponent)
	{
		m_vecParallelUpdate.push_back(pComponent);
	}

	void Scene::AddParallelShow(Component* pComponent)
	{
		m_vecParallelShow.push_back(pComponent);
	}

	void Scene::AddRenderLight(Light* pLight)
	{
		m_vecRenderLight.push_back(pLight);
	}

	void Scene::Update()
	{
		profile_code();

		if (m_pCamera == NULL || m_pCullTree == NULL)
			return;

		if (m_pCallback)
		{
			m_pCallback->OnPreUpdate(this);
		}

		m_pMainDirLight->UpdateShadowMap(m_pCamera.get());

		m_pRootNode->Update();
		
		if (GetJobScheduler()->GetNumThreads() > 0)
		{
			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(m_vecParallelUpdate.size());
			for (uint32_t i = 0; i < m_vecParallelUpdate.size(); ++i)
			{
				Component* pComp = m_vecParallelUpdate[i].get();

				GetJobScheduler()->SubmitJob(jobGroup,
					[pComp]() { pComp->ParallelUpdate(); }
				);
			}
			GetJobScheduler()->WaitForGroup(jobGroup);

			for (uint32_t i = 0; i < m_vecParallelUpdate.size(); ++i)
			{
				m_vecParallelUpdate[i]->EndParallelUpdate();
			}
			m_vecParallelUpdate.clear();
		}
		
		m_arrRenderComp.clear();

		typedef vector<RenderComponent*> VEC_OBJ;
		static VEC_OBJ vecObj;
		m_pCullTree->FindObjectsIn(&m_pCamera->GetFrustum(),-1,vecObj);

		uint32_t nNodeCount = vecObj.size();
		m_arrRenderComp.resize(nNodeCount);
		for (uint32_t mm = 0; mm < nNodeCount; ++mm)
		{
			m_arrRenderComp[mm] = vecObj[mm];
		}
		vecObj.clear();
		
		RenderQueue* pRenderQueue = GetRenderQueue();
		pRenderQueue->Clear();

		for (uint32_t i = 0; i < m_arrRenderComp.size(); ++i)
		{
			m_arrRenderComp[i]->GetSceneNode()->SetLastVisibleFrame(GetTimer()->GetFrameCount());

			m_arrRenderComp[i]->Render(pRenderQueue);
		}

		if (GetJobScheduler()->GetNumThreads() > 0)
		{
			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(m_vecParallelShow.size());
			for (uint32_t i = 0; i < m_vecParallelShow.size(); ++i)
			{
				Component* pComp = m_vecParallelUpdate[i].get();
				Camera* pCamera = m_pCamera.get();

				GetJobScheduler()->SubmitJob(jobGroup, 
					[pComp, pCamera]() { pComp->ParallelShow(pCamera); }
				);
			}
			GetJobScheduler()->WaitForGroup(jobGroup);
			m_vecParallelShow.clear();
		}

		if (m_pCallback)
		{
			m_pCallback->OnPostUpdate(this);
		}
	}

	RenderQueue* Scene::GetRenderQueue()
	{
		int index = GetRenderSystem()->GetThreadList();
		return m_pRenderQueue[index];
	}

	void Scene::Render()
	{
		if (m_pCamera == NULL)
			return;

		GetRenderContext()->SetCurScene(this);
	
		GetRenderSystem()->BegineRender();

		m_pMainDirLight->RenderShadowMap(m_pCamera.get());

		GetRenderContext()->SetCamera(m_pCamera.get());

		if (m_pCallback)
		{
			m_pCallback->OnPreRender(this);
		}

		m_pRenderScheme->Render();

		if (m_pCallback)
		{
			m_pCallback->OnPosRender(this);
		}

		GetRenderSystem()->EndRender();

		m_vecRenderLight.clear();
	}

	RefPtr<Scene> CreateScene()
	{
		return new Scene();
	}

}



