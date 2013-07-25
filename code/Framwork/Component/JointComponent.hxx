#include "JointComponent.h"

namespace ma
{

	GenericJointComponent::GenericJointComponent(GameObject* pGameObject)
	{
		ASSERT(pGameObject && pGameObject->GetPhyscisObject());
		if (pGameObject && pGameObject->GetPhyscisObject())
		{
			m_pPhysicsGenericJoint = pGameObject->GetPhyscisObject()->CreatePhysicsGenericJoint(NULL);
		}
	}


	FixJointComponent::FixJointComponent(GameObject* pGameObject)
	{
		ASSERT(pGameObject && pGameObject->GetPhyscisObject());
		if (pGameObject && pGameObject->GetPhyscisObject())
		{
			IPhysicsGenericJoint* pGenericJoint = pGameObject->GetPhyscisObject()->CreatePhysicsGenericJoint(NULL);
			pGenericJoint->SetAngularLowerLimit(Vector3(0.0f, 0.0f, 0.0f));
			pGenericJoint->SetAngularUpperLimit(Vector3(0.0f, 0.0f, 0.0f));
			pGenericJoint->SetLinearLowerLimit(Vector3(0.0f, 0.0f, 0.0f));
			pGenericJoint->SetLinearUpperLimit(Vector3(0.0f, 0.0f, 0.0f));
			m_pPhysicsFixJoint = pGenericJoint;
		}
	}
	
	HingJointComponent::HingJointComponent(GameObject* pGameObject)
	{
		ASSERT(pGameObject && pGameObject->GetPhyscisObject());
		if (pGameObject && pGameObject->GetPhyscisObject())
		{
			m_pPhysicsHingeJoint = m_pGameObject->GetPhyscisObject()->CreatePhysicsHingeJoint();
		}
	}


}