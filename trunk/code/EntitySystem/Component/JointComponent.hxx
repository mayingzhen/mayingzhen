#include "JointComponent.h"

namespace ma
{
	IMPL_OBJECT(GenericJointComponent,Component)

	GenericJointComponent::GenericJointComponent(GameObject* pGameObject)
		:Component(pGameObject)
	{
		ASSERT(pGameObject && pGameObject->GetPhyscisObject());
		if (pGameObject && pGameObject->GetPhyscisObject())
		{
			m_pPhysicsGenericJoint = GetPhysicsSystem()->CreatePhysicsGenericJoint(pGameObject->GetPhyscisObject(),NULL);
		}
	}

	IMPL_OBJECT(FixJointComponent,Component)

	FixJointComponent::FixJointComponent(GameObject* pGameObject)
		:Component(pGameObject)
	{
		ASSERT(pGameObject && pGameObject->GetPhyscisObject());
		if (pGameObject && pGameObject->GetPhyscisObject())
		{
			IPhysicsGenericJoint* pGenericJoint = GetPhysicsSystem()->CreatePhysicsGenericJoint(pGameObject->GetPhyscisObject(),NULL);
			pGenericJoint->SetAngularLowerLimit(Vector3(0.0f, 0.0f, 0.0f));
			pGenericJoint->SetAngularUpperLimit(Vector3(0.0f, 0.0f, 0.0f));
			pGenericJoint->SetLinearLowerLimit(Vector3(0.0f, 0.0f, 0.0f));
			pGenericJoint->SetLinearUpperLimit(Vector3(0.0f, 0.0f, 0.0f));
			m_pPhysicsFixJoint = pGenericJoint;
		}
	}

	void FixJointComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		ASSERT(false);
	}
	
	IMPL_OBJECT(HingJointComponent,Component)

	HingJointComponent::HingJointComponent(GameObject* pGameObject)
		:Component(pGameObject)
	{
		ASSERT(pGameObject && pGameObject->GetPhyscisObject());
		if (pGameObject && pGameObject->GetPhyscisObject())
		{
			m_pPhysicsHingeJoint = GetPhysicsSystem()->CreatePhysicsHingeJoint(pGameObject->GetPhyscisObject(),NULL);
		}
	}


}