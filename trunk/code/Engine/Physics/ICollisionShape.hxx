#include "ICollisionShape.h"

namespace ma
{
	IMPL_OBJECT(ICollisionShape,Component);

	IMPL_OBJECT(IBoxCollisionShape,ICollisionShape);

	IMPL_OBJECT(ISphereCollisionShape,ICollisionShape);

	IMPL_OBJECT(ICapsuleCollisionShape,ICollisionShape);

	IMPL_OBJECT(ICollisionMaterial,Component);
}


