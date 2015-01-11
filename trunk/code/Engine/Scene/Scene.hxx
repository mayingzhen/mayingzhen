#include "Scene.h"
#include "Octree.h"
#include "FrustumCullQuery.h"

namespace ma
{
	Scene::Scene(const char* pszName)
		:SceneNode(NULL,pszName)
	{
		m_sName = pszName ? pszName : "";

		SceneNode* pCamera = this->CreateNode("defaultCamera");
		m_pCamera = pCamera->CreateComponent<Camera>();
		
		m_pRenderScheme = CreateRenderScheme(RenderScheme::Forward);

		m_pCullTree = new Octree();
		m_pScene = this;
	}

	SceneNode* Scene::CreateNode(const char* pName)
	{
		SceneNode* pSceneNode = new SceneNode(this,pName);	
		this->AddChild(pSceneNode);
		return pSceneNode;
	}

	void Scene::Reset()
	{
		m_eResState = ResUnLoad;

		this->RemoveAllChild();

		m_pCullTree = new Octree();
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

		for (uint32 i = 0; i < m_arrChild.size(); ++i)
		{
			m_arrChild[i]->Update();
		}

		m_arrRenderComp.clear();
		FrustumCullQuery frustumQuery(m_pCamera->GetFrustum(),m_arrRenderComp);
		m_pCullTree->FindObjectsIn(frustumQuery);

		for (UINT i = 0; i < m_arrRenderComp.size(); ++i)
		{
			RenderComponent* pRenderComp = m_arrRenderComp[i];

			pRenderComp->Show(m_pCamera.get());
		}

		if (m_pCallback)
		{
			m_pCallback->OnPostUpdate(this);
		}
	}

	void Scene::Render()
	{
		if (m_pCamera == NULL)
			return;
	
		GetRenderSystem()->BeginFrame();

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

		if (m_pRenderScheme)
			m_pRenderScheme->Render();

		if (m_pCallback)
		{
			m_pCallback->OnPosRender(this);
		}

		if (GetLineRender())
			GetLineRender()->Render();

		GetRenderSystem()->EndFrame();
	}

}



