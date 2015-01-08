#include "IPhysicsJoint.h"

namespace ma
{
	
	IMPL_OBJECT(IPhysicsJoint,Component);

	IMPL_OBJECT(IPhysicsGenericJoint,IPhysicsJoint);

	IMPL_OBJECT(IPhysicsHingeJoint,IPhysicsJoint);

}


