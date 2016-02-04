#include "Scene.h"
#include "Octree.h"
#include "FrustumCullQuery.h"

namespace ma
{
	Scene::Scene(const char* pszName)
	{
		m_pRootNode = new SceneNode(this);
		m_sName = pszName ? pszName : "";

		m_pCamera = CreateCamera();
		m_pRootNode->AddChild(m_pCamera.get());

		m_pSunShadow = new RenderShadowCSM();
		m_pSunShadow->LookAt(Vector3(1.f, 1.f, 0.f),Vector3::ZERO); 
		
		m_viewport = GetRenderSystem()->GetViewPort();
		SetRenderScheme(RenderScheme::Forward);

		m_pCullTree = new Octree();

		m_pRenderQueue[0] = new RenderQueue();
		m_pRenderQueue[1] = new RenderQueue();
		
		m_viwMinZ = 0.0f;
		m_viwMaxZ = 0.0f;
		m_pCallback = NULL;	
	}

	Scene::~Scene()
	{
		SAFE_DELETE(m_pRenderQueue[0]);
		SAFE_DELETE(m_pRenderQueue[1]);
	}

	void Scene::Reset()
	{
		m_pRootNode->RemoveAllChild();

		m_pCullTree = new Octree();

		m_arrRenderComp.clear();
	}

	void Scene::SetRenderScheme(RenderScheme::Type eType)
	{
		if (eType == RenderScheme::DeferredLighting)
		{
			GetRenderSystem()->AddShaderGlobaMacro("DEFERREDSHADING", "1");
		}

		m_pRenderScheme = CreateRenderScheme(eType,this);
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
			
		if (GetWorkQueue()->GetNumThreads() > 0)
		{
			WorkQueue::JobGroupID jobGroup = GetWorkQueue()->BeginGroup(m_vecParallelUpdate.size());
			for (UINT32 i = 0; i < m_vecParallelUpdate.size(); ++i)
			{
				GetWorkQueue()->SubmitJob(jobGroup,ParallelUpdate,m_vecParallelUpdate[i].get(),NULL,NULL);
			}
			GetWorkQueue()->WaitForGroup(jobGroup);

			for (UINT i = 0; i < m_vecParallelUpdate.size(); ++i)
			{
				m_vecParallelUpdate[i]->EndParallelUpdate();
			}
			m_vecParallelUpdate.clear();
		}

		m_arrRenderComp.clear();
		FrustumCullQuery frustumQuery(m_pCamera->GetFrustum(),m_arrRenderComp);
		m_pCullTree->FindObjectsIn(frustumQuery);

		m_pSunShadow->Update(m_pCamera.get());
		
		if (GetRenderQueue())
			GetRenderQueue()->Clear();

		for (UINT i = 0; i < m_arrRenderComp.size(); ++i)
		{
			m_arrRenderComp[i]->Show(m_pCamera.get());
		}

		if (GetWorkQueue()->GetNumThreads() > 0)
		{
			WorkQueue::JobGroupID jobGroup = GetWorkQueue()->BeginGroup(m_vecParallelShow.size());
			for (UINT32 i = 0; i < m_vecParallelShow.size(); ++i)
			{
				GetWorkQueue()->SubmitJob(jobGroup,ParallelShow,m_vecParallelShow[i].get(),m_pCamera.get(),NULL);
			}
			GetWorkQueue()->WaitForGroup(jobGroup);

			for (UINT i = 0; i < m_vecParallelUpdate.size(); ++i)
			{
				m_vecParallelUpdate[i]->EndParallelUpdate();
			}
			m_vecParallelUpdate.clear();
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

		m_pSunShadow->Render(m_pCamera.get());

		if (m_pRenderTarget)
		{
			GetRenderSystem()->SetRenderTarget(m_pRenderTarget.get());
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

		if (GetLineRender())
			GetLineRender()->Render();

		GetRenderSystem()->EndRender();
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



