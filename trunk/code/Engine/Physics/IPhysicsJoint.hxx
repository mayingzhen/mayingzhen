#include "IPhysicsJoint.h"

namespace ma
{
	
	IMPL_OBJECT(IPhysicsJoint,Component);

	IPhysicsJoint::IPhysicsJoint(GameObject* pGameObj)
		:Component(pGameObj)
	{

	}

	IMPL_OBJECT(IPhysicsGenericJoint,IPhysicsJoint);

	IPhysicsGenericJoint::IPhysicsGenericJoint(GameObject* pGameObj)
		:IPhysicsJoint(pGameObj)
	{

	}

	IMPL_OBJECT(IPhysicsHingeJoint,IPhysicsJoint);

	IPhysicsHingeJoint::IPhysicsHingeJoint(GameObject* pGameObj)
		:IPhysicsJoint(pGameObj)
	{

	}

}


