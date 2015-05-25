#include "Scene.h"
#include "Octree.h"
#include "FrustumCullQuery.h"

namespace ma
{
	Scene::Scene(const char* pszName)
	{
		m_pRootNode = new SceneNode(this);
		m_sName = pszName ? pszName : "";

		m_pCameraNode = new SceneNode(this, "defaultCamera");
		m_pCamera = m_pCameraNode->CreateComponent<Camera>();

		m_pSunNode = new SceneNode(this,"SunNode");
		m_pSunShadow = new RenderShadowCSM();
		m_pSunNode->AddComponent(m_pSunShadow.get());
		m_pSunNode->LookAt(Vector3(1.f, 1.f, 0.f),Vector3::ZERO); 
		
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

	SceneNode* Scene::CreateNode(const char* pPath)
	{
		RefPtr<SceneNode> pSceneNode = CreateSceneNode(pPath);
		m_pRootNode->AddChild( pSceneNode.get() );
		return pSceneNode.get();
	}
	
	void ParallelUpdateWork(const WorkItem* item, unsigned threadIndex)
	{
		Component** start = reinterpret_cast<Component**>(item->start_);
		Component** end = reinterpret_cast<Component**>(item->end_);

		do
		{
			Component* drawable = *start;
			if (drawable)
				drawable->ParallelUpdate();
		}
		while (start++ != end);
	}

	void Scene::AddParallelUpdate(Component* drawable)
	{
		m_vecParallelUpdateNode.push_back(drawable);
	}

	void Scene::ParallelUpdate()
	{
		if (m_vecParallelUpdateNode.empty())
			return;

		WorkQueue* queue = GetWorkQueue();

		int numWorkItems = queue->GetNumThreads() + 1; // Worker threads + main thread
		int drawablesPerItem = Math::Max((int)(m_vecParallelUpdateNode.size() / numWorkItems), 1);

		int start = 0;
		// Create a work item for each thread
		for (int i = 0; i < numWorkItems; ++i)
		{
			RefPtr<WorkItem> item = queue->GetFreeItem();
			item->priority_ = -1/*M_MAX_UNSIGNED*/;
			item->workFunction_ = ParallelUpdateWork;
			item->aux_ = NULL;

			int end = m_vecParallelUpdateNode.size();
			if (i < numWorkItems - 1 && end - start > drawablesPerItem)
				end = start + drawablesPerItem;

			item->start_ = &(m_vecParallelUpdateNode[start]);
			item->end_ = &(m_vecParallelUpdateNode[end - 1]);
			queue->AddWorkItem(item);

			start = end;

			if (start >= (int)m_vecParallelUpdateNode.size())
				break;
		}

		queue->Complete(-1/*M_MAX_UNSIGNED*/);

		for (UINT i = 0; i < m_vecParallelUpdateNode.size(); ++i)
		{
			m_vecParallelUpdateNode[i]->EndParallelUpdate();
		}
		m_vecParallelUpdateNode.clear();
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

		m_pCameraNode->Update();
		
		m_pRootNode->Update();
			
		ParallelUpdate();

		m_arrRenderComp.clear();
		FrustumCullQuery frustumQuery(m_pCamera->GetFrustum(),m_arrRenderComp);
		m_pCullTree->FindObjectsIn(frustumQuery);

		UpdateViewMinMaxZ();

		m_pSunShadow->Update(m_pCamera.get());
		
		if (GetRenderQueue())
			GetRenderQueue()->Clear();

		for (UINT i = 0; i < m_arrRenderComp.size(); ++i)
		{
			m_arrRenderComp[i]->Show(m_pCamera.get());
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

	void Scene::UpdateViewMinMaxZ()
	{
		if(m_arrRenderComp.size() == 0) 
			return;

		// find the nearest and farthest points of given
		// scene objects in camera's view space
		float fMaxZ = 0;
		float fMinZ = FLT_MAX;

		// for each object
		for(unsigned int i = 0; i < m_arrRenderComp.size(); i++)
		{
			AABB aabb = m_arrRenderComp[i]->GetAABBWS();
			aabb.transform(m_pCamera->GetMatView());

			float aabbMinZ = -aabb.getMaximum().z;
			float aabbMaxZ = -aabb.getMinimum().z;

			m_arrRenderComp[i]->SetViewMinMaxZ(aabbMinZ, aabbMaxZ);

			fMaxZ = max(aabbMaxZ,fMaxZ);
			fMinZ = min(aabbMinZ,fMinZ);
		}

		// use smallest distance as new near plane
		// and make sure it is not too small
		m_viwMinZ = max(fMinZ, m_pCamera->GetNearClip());

		// use largest distance as new far plane
		// and make sure it is larger than nearPlane
		m_viwMaxZ = max(fMaxZ, m_viwMinZ + 1.0f);
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



