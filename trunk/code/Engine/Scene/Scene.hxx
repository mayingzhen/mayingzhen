#include "Scene.h"
#include "Octree.h"
#include "FrustumCullQuery.h"

namespace ma
{
	Scene::Scene(const char* pszName)
		:SceneNode(NULL,pszName)
	{
		m_pScene = this;
		m_sName = pszName ? pszName : "";

		m_pCameraNode = new SceneNode(this, "defaultCamera");
		m_pCamera = m_pCameraNode->CreateComponent<Camera>();
		
		SetRenderScheme(RenderScheme::Forward);

		m_pCullTree = new Octree();

		m_pRenderQueue[0] = new RenderQueue();
		m_pRenderQueue[1] = new RenderQueue();
		
		m_viwMinZ = 0.0f;
		m_viwMaxZ = 0.0f;
		m_cDirLight = ColourValue::White;
		m_vDirLight = Vector3::ZERO;
		m_pCallback = NULL;
	}

	Scene::~Scene()
	{
		SAFE_DELETE(m_pRenderQueue[0]);
		SAFE_DELETE(m_pRenderQueue[1]);
	}

	void Scene::Reset()
	{
		m_eResState = ResUnLoad;

		this->RemoveAllChild();

		m_pCullTree = new Octree();

		m_arrRenderComp.clear();
	}

	void Scene::SetRenderScheme(RenderScheme::Type eType)
	{
		if (eType == RenderScheme::DeferredLighting)
		{
			GetRenderSystem()->AddMacro("DeferredLight", "1");
		}

		m_pRenderScheme = CreateRenderScheme(eType,this);
	}

	SceneNode* Scene::CreateNode(const char* pName)
	{
		SceneNode* pSceneNode = new SceneNode(this,pName);	
		this->AddChild(pSceneNode);
		return pSceneNode;
	}

	void Scene::Init()
	{
		ASSERT(m_pRenderScheme);
		if (m_pRenderScheme)
			m_pRenderScheme->Init();
	}

	void Scene::ShutDown()
	{
		ASSERT(m_pRenderScheme);
		if (m_pRenderScheme)
			m_pRenderScheme->ShoutDown();

		m_pRenderQueue[0]->Clear();
		m_pRenderQueue[1]->Clear();
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

		for (uint32 i = 0; i < m_arrChild.size(); ++i)
		{
			m_arrChild[i]->Update();
		}

		m_arrRenderComp.clear();
		FrustumCullQuery frustumQuery(m_pCamera->GetFrustum(),m_arrRenderComp);
		m_pCullTree->FindObjectsIn(frustumQuery);

		UpdateViewMinMaxZ();

		GetRenderShadowCSM()->Update(m_pCamera.get());
		
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
	
		GetRenderSystem()->BeginFrame();

		GetRenderShadowCSM()->Render(m_pCamera.get());

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

		GetRenderSystem()->EndFrame();
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

	void Scene::GetDirectionalLight(OUT ColourValue& color, OUT Vector3& vDir) const
	{
		color = m_cDirLight;
		vDir = m_vDirLight;
	}

	void Scene::SetDirectionalLight(const ColourValue& color, const Vector3& vDir)
	{
		m_cDirLight = color;
		m_vDirLight = vDir;
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



