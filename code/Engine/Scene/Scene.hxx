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

		SceneNode* pSun = CreateSceneNode();
		m_pRootNode->AddChild(pSun);
		m_pDirLight = CreateDirectonalLight();
		pSun->AddComponent(m_pDirLight.get());

		m_cAmbientColor = Vector3::ZERO;
	}

	Scene::~Scene()
	{
		SAFE_DELETE(m_pRenderQueue[0]);
		SAFE_DELETE(m_pRenderQueue[1]);
	}

	void Scene::Reset(uint32 nWidth,uint32 nHeight)
	{
		m_viewport = Rectangle(0,0,(float)nWidth,(float)nHeight);

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
	
	void* ParallelUpdate (void* rawData,void* rawData1)
	{
		Component* pComponent = reinterpret_cast<Component*>(rawData);
		pComponent->ParallelUpdate();
		return NULL;
	}

	struct ParallelUpdateTask
	{
		MT_DECLARE_TASK(ParallelUpdateTask, MT::StackRequirements::STANDARD, MT::TaskPriority::NORMAL, MT::Color::Blue);

		Component* m_pComponent;

		void Do(MT::FiberContext&)
		{
			m_pComponent->ParallelUpdate();
		}
	};

	struct ParallelShowTask
	{
		MT_DECLARE_TASK(ParallelShowTask, MT::StackRequirements::STANDARD, MT::TaskPriority::NORMAL, MT::Color::Blue);

		Component* m_pComponent;
		Camera* m_pCamera;

		void Do(MT::FiberContext&)
		{
			m_pComponent->ParallelShow(m_pCamera);
		}
	};

	void* ParallelShow(void* rawData,void* rawData1)
	{
		Component* pComponent = reinterpret_cast<Component*>(rawData);
		Camera* pCamera = reinterpret_cast<Camera*>(rawData1);
		pComponent->ParallelShow(pCamera);
		return NULL;
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

		m_pRootNode->Update();
		
		/*
		if (GetJobScheduler()->GetNumThreads() > 0)
		{
			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(m_vecParallelUpdate.size());
			for (UINT32 i = 0; i < m_vecParallelUpdate.size(); ++i)
			{
				GetJobScheduler()->SubmitJob(jobGroup,ParallelUpdate,m_vecParallelUpdate[i].get(),NULL,NULL);
			}
			GetJobScheduler()->WaitForGroup(jobGroup);

			for (UINT i = 0; i < m_vecParallelUpdate.size(); ++i)
			{
				m_vecParallelUpdate[i]->EndParallelUpdate();
			}
			m_vecParallelUpdate.clear();
		}
		*/
		if (!m_vecParallelUpdate.empty())
		{
			static vector<ParallelUpdateTask> vecJobData;
			vecJobData.resize(m_vecParallelUpdate.size());
			for (UINT32 iJob = 0; iJob < m_vecParallelUpdate.size(); ++iJob)
			{
				vecJobData[iJob].m_pComponent = m_vecParallelUpdate[iJob].get();
			}
			MT::g_pTaskScheduler->RunAsync(MT::TaskGroup::Default(), &vecJobData[0], vecJobData.size());
			MT::g_pTaskScheduler->WaitGroup(MT::TaskGroup::Default(), -1);
		}

		m_arrRenderComp.clear();

		typedef vector<RenderComponent*> VEC_OBJ;
		static VEC_OBJ vecObj;
		m_pCullTree->FindObjectsIn(&m_pCamera->GetFrustum(),-1,vecObj);

		uint32 nNodeCount = vecObj.size();
		m_arrRenderComp.resize(nNodeCount);
		for (uint32 mm = 0;mm< nNodeCount;++mm)
		{
			m_arrRenderComp[mm] = vecObj[mm];
		}
		vecObj.clear();
		
		if (GetRenderQueue())
			GetRenderQueue()->Clear();

		for (UINT i = 0; i < m_arrRenderComp.size(); ++i)
		{
			m_arrRenderComp[i]->Show(m_pCamera.get());
		}

// 		if (GetJobScheduler()->GetNumThreads() > 0)
// 		{
// 			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(m_vecParallelShow.size());
// 			for (UINT32 i = 0; i < m_vecParallelShow.size(); ++i)
// 			{
// 				GetJobScheduler()->SubmitJob(jobGroup,ParallelShow,m_vecParallelShow[i].get(),m_pCamera.get(),NULL);
// 			}
// 			GetJobScheduler()->WaitForGroup(jobGroup);
// 			m_vecParallelShow.clear();
// 		}
		if (!m_vecParallelShow.empty())
		{
			static vector<ParallelShowTask> vecJobData;
			vecJobData.resize(m_vecParallelShow.size());
			for (UINT32 iJob = 0; iJob < m_vecParallelShow.size(); ++iJob)
			{
				vecJobData[iJob].m_pCamera = m_pCamera.get();
				vecJobData[iJob].m_pComponent = m_vecParallelShow[iJob].get();
			}
			MT::g_pTaskScheduler->RunAsync(MT::TaskGroup::Default(), &vecJobData[0], vecJobData.size());
			MT::g_pTaskScheduler->WaitGroup(MT::TaskGroup::Default(), -1);
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

		for (uint32 i = 0; i < m_vecRenderLight.size();++i)
		{
			m_vecRenderLight[i]->RenderShadowMap(m_pCamera.get());
		}

		GetRenderSystem()->SetViewPort(m_viewport);

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

		LineRender::Render();

		GetRenderSystem()->EndRender();

		m_vecRenderLight.clear();
	}

	void Scene::OnFlushFrame()
	{
		//if ( GetRenderQueue() )
		//	GetRenderQueue()->Clear()
	}

	RefPtr<Scene> CreateScene()
	{
		return new Scene();
	}

}



