#include "ICollisionShape.h"

namespace ma
{
	IMPL_OBJECT(ICollisionShape,Component);

	ICollisionShape::ICollisionShape(GameObject* pGameObj)
		:Component(pGameObj)
	{

	}

	IMPL_OBJECT(IBoxCollisionShape,ICollisionShape);

	IBoxCollisionShape::IBoxCollisionShape(GameObject* pGameObj):
		ICollisionShape(pGameObj)
	{

	}

	IMPL_OBJECT(ISphereCollisionShape,ICollisionShape);

	ISphereCollisionShape::ISphereCollisionShape(GameObject* pGameObj)
		:ICollisionShape(pGameObj)
	{

	}

	IMPL_OBJECT(ICapsuleCollisionShape,ICollisionShape);

	ICapsuleCollisionShape::ICapsuleCollisionShape(GameObject* pGameObj)
		:ICollisionShape(pGameObj)
	{

	}
	
}


