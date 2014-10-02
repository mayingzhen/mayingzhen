#include "IPhysicsJoint.h"

namespace ma
{
	
	IMPL_OBJECT(IPhysicsJoint,Component);

	IPhysicsJoint::IPhysicsJoint(SceneNode* pGameObj)
		:Component(pGameObj)
	{

	}

	IMPL_OBJECT(IPhysicsGenericJoint,IPhysicsJoint);

	IPhysicsGenericJoint::IPhysicsGenericJoint(SceneNode* pGameObj)
		:IPhysicsJoint(pGameObj)
	{

	}

	IMPL_OBJECT(IPhysicsHingeJoint,IPhysicsJoint);

	IPhysicsHingeJoint::IPhysicsHingeJoint(SceneNode* pGameObj)
		:IPhysicsJoint(pGameObj)
	{

	}

}


