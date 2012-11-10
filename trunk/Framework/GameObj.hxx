#include "Framework/GameObj.h"

#include "Framework/Component.h"

namespace ma
{

	GameObject::GameObject(Scene* pScene, const char* pName):
	SceneNode(pScene,pName)
	{

	}

	GameObject::~GameObject()
	{

	}

	void GameObject::Render()
	{
		for (UINT i = 0; i < m_vComponents.size(); ++i)
		{
			m_vComponents[i]->Render();
		}
	}

	void GameObject::Update()
	{
		for (UINT i = 0; i < m_vComponents.size(); ++i)
		{
			m_vComponents[i]->Update();
		}
	}


	void GameObject::ParalleUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_vComponents.size(); ++i)
		{
			m_vComponents[i]->ParalleUpdate(fTimeElapsed);
		}
	}

	void GameObject::FixedUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_vComponents.size(); ++i)
		{
			m_vComponents[i]->FixedUpdate(fTimeElapsed);
		}
	}

	void GameObject::LateUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_vComponents.size(); ++i)
		{
			m_vComponents[i]->LateUpdate(fTimeElapsed);
		}
	}

	void GameObject::Start()
	{
		for (UINT i = 0; i < m_vComponents.size(); ++i)
		{
			m_vComponents[i]->Start();
		}
	}

	void GameObject::Stop()
	{
		for (UINT i = 0; i < m_vComponents.size(); ++i)
		{
			m_vComponents[i]->Stop();
		}
	}

	void GameObject::AddComponent(Component* pComponent)
	{
		if (pComponent == NULL)
			return;

		std::vector<Component*>::iterator it = find(m_vComponents.begin(),m_vComponents.end(),pComponent);
		if (it != m_vComponents.end())
			return;

		m_vComponents.push_back(pComponent);

		pComponent->SetGameObject(this);
	}

}


