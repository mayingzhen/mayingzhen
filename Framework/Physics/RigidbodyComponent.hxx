#include "Framework/Physics/RigidbodyComponent.h"
#include "Framework/Physics/IRigidBody.h"


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

	void RigidBodyComponent::SetUseGravity(bool bUseGravity)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->SetUseGravity(bUseGravity);
		}
	}

	bool RigidBodyComponent::IsUseGravity()
	{
		if (m_pRigidBody)
		{
			return m_pRigidBody->IsUseGravity();
		}
		return false;
	}

	void RigidBodyComponent::SetKinematic(bool bKinematic)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->SetKinematic(bKinematic);
		}
	}

	bool RigidBodyComponent::IsKinematic()
	{
		if (m_pRigidBody)
		{
			return m_pRigidBody->IsKinematic();
		}
		return false;
	}

	void	RigidBodyComponent::SetMass(float fMass)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->SetMass(fMass);
		}
	}

	float	RigidBodyComponent::GetMass()
	{
		if (m_pRigidBody)
		{
			return m_pRigidBody->GetMass();
		}
		return 0;
	}

	void	RigidBodyComponent::SetLinearDamping(float fLinearDamping)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->SetAngularDamping(fLinearDamping);
		}
	}

	float	RigidBodyComponent::GetLinearDamping()
	{
		if (m_pRigidBody)
		{
			return m_pRigidBody->GetLinearDamping();
		}
		return 0;
	}

	void	RigidBodyComponent::SetAngularDamping(float fAngularDamping)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->SetAngularDamping(fAngularDamping);
		}
	}

	float	RigidBodyComponent::GetAngularDamping()
	{
		if (m_pRigidBody)
		{
			return m_pRigidBody->GetAngularDamping();
		}
		return 0;
	}

	void RigidBodyComponent::SetGameObject(GameObject* pGameObj)
	{
		__super::SetGameObject(pGameObj);

		if (pGameObj == NULL)
			return;

		m_pPhysicsObject = pGameObj->GetPhyscisObject();
		if (m_pPhysicsObject == NULL)
		{
			m_pPhysicsObject = GetPhysicsDevice()->CreatePhysicsObject();
			pGameObj->SetPhyscisObject(m_pPhysicsObject);
		}
		assert(m_pPhysicsObject);

		if (m_pPhysicsObject)
		{	
			m_pPhysicsObject->SetRigidBody(m_pRigidBody);
		}
	}
}

