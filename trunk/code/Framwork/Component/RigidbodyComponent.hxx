#include "RigidbodyComponent.h"


namespace ma
{
	IMPL_OBJECT(RigidBodyComponent,Component)

	RigidBodyComponent::RigidBodyComponent(GameObject* pGameObj)
	:Component(pGameObj)
	{
		m_pRigidBody = NULL;

		ASSERT(pGameObj);
		m_pPhysicsObject = pGameObj ? m_pGameObject->GetPhyscisObject() : NULL;
		ASSERT(m_pPhysicsObject);	
		m_pRigidBody = m_pPhysicsObject ? m_pPhysicsObject->CreateRigidBody() : NULL;
		
	}

	RigidBodyComponent::~RigidBodyComponent()
	{

	}

	void RigidBodyComponent::Serialize(Serializer& sl, const char* pszLable)
	{

	}
}

