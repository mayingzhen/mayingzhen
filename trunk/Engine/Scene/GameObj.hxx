#include "GameObj.h"
#include "Component.h"

namespace ma
{
	IMPL_OBJECT(GameObject,SceneNode)
	GameObject::GameObject(Scene* pScene, const char* pName):
	SceneNode(pScene,pName)
	{
		m_pPhyscisObject = NULL;
// 		IPhysicsDevice* pPhysicsDevice = GetPhysicsDevice();
// 		if (pPhysicsDevice)
// 		{
// 			m_pPhyscisObject = pPhysicsDevice->CreatePhysicsObject();
// 		}
	}

	GameObject::~GameObject()
	{
		//SAFE_DELETE(m_pPhyscisObject);
	}

	void GameObject::Render()
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->Render();
		}
	}

	void GameObject::Update()
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->Update();
		}
	}


	void GameObject::ParalleUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->ParalleUpdate(fTimeElapsed);
		}
	}

	void GameObject::FixedUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->FixedUpdate(fTimeElapsed);
		}
	}

	void GameObject::LateUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->LateUpdate(fTimeElapsed);
		}
	}

	void GameObject::Start()
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->Start();
		}

		if (m_pPhyscisObject)
		{
			m_pPhyscisObject->Start(this);
		}
	}

	void GameObject::Stop()
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->Stop();
		}

		if (m_pPhyscisObject)
		{
			m_pPhyscisObject->Stop(this);
		}
	}

	void GameObject::AddComponent(Component* pComponent)
	{
		if (pComponent == NULL)
			return;

		std::vector<Component*>::iterator it = find(m_arrComp.begin(),m_arrComp.end(),pComponent);
		if (it != m_arrComp.end())
			return;

		m_arrComp.push_back(pComponent);

		pComponent->SetGameObject(this);
	}

	void GameObject::SyncToPhysics()
	{
		if (m_pPhyscisObject == NULL)
			return;
	
		//if ( m_pPhyscisObject->IsKinematic() )
			m_pPhyscisObject->SetTransformWS( this->GetTransformWS() );
	}

	void GameObject::SyncFromPhysics()
	{
		if (m_pPhyscisObject == NULL)
			return;

		//if ( !m_pPhyscisObject->IsKinematic() )
			this->SetTransformWS( m_pPhyscisObject->GetTransformWS() );
	}

	void GameObject::TravelProperty(PropertyVisitor* pVisitor)
	{
		SceneNode::TravelProperty(pVisitor);

		pVisitor->VisitObjectArrayProperty(m_arrComp,"arrComp",RefMode_Composite);
	}

	void GameObject::Serialize(SerializeListener& sl, const char* pszLable)
	{
		__super::Serialize(sl,pszLable);

		sl.BeginSection(pszLable);

		sl.SerializeObjectArray(m_arrComp);

		sl.EndSection();

		if ( sl.IsReading() )
		{
			for(UINT i = 0; i < m_arrComp.size(); ++i)
			{
				m_arrComp[i]->SetGameObject(this);
			}
		}
	}

	void GameObject::UpdateAABB()
	{

	}
}


