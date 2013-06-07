#include "RigidbodyComponent.h"


namespace ma
{
	IMPL_OBJECT(RigidBodyComponent,Component)

	RigidBodyComponent::RigidBodyComponent()
	{
		m_pPhysicsObject = NULL;

		if (GetPhysicsDevice() == NULL)
			return;

		m_pRigidBody = GetPhysicsDevice()->CreateRigidBody();
	}

	RigidBodyComponent::~RigidBodyComponent()
	{

	}

	void RigidBodyComponent::SetGameObject(GameObject* pGameObj)
	{
		Component::SetGameObject(pGameObj);

		if (pGameObj == NULL)
			return;

		m_pPhysicsObject = pGameObj->GetPhyscisObject();
		if (m_pPhysicsObject == NULL)
		{
			m_pPhysicsObject = GetPhysicsDevice()->CreatePhysicsObject();
			pGameObj->SetPhyscisObject(m_pPhysicsObject);
		}
		ASSERT(m_pPhysicsObject);

		if (m_pPhysicsObject)
		{	
			m_pPhysicsObject->SetRigidBody(m_pRigidBody);
		}
	}
}

