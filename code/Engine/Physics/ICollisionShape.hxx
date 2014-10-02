#include "ICollisionShape.h"

namespace ma
{
	IMPL_OBJECT(ICollisionShape,Component);

	ICollisionShape::ICollisionShape(SceneNode* pGameObj)
		:Component(pGameObj)
	{

	}

	IMPL_OBJECT(IBoxCollisionShape,ICollisionShape);

	IBoxCollisionShape::IBoxCollisionShape(SceneNode* pGameObj):
		ICollisionShape(pGameObj)
	{

	}

	IMPL_OBJECT(ISphereCollisionShape,ICollisionShape);

	ISphereCollisionShape::ISphereCollisionShape(SceneNode* pGameObj)
		:ICollisionShape(pGameObj)
	{

	}

	IMPL_OBJECT(ICapsuleCollisionShape,ICollisionShape);

	ICapsuleCollisionShape::ICapsuleCollisionShape(SceneNode* pGameObj)
		:ICollisionShape(pGameObj)
	{

	}

	IMPL_OBJECT(ICollisionMaterial,Component);

	ICollisionMaterial::ICollisionMaterial(SceneNode* pGameObj)
		:Component(pGameObj)
	{

	}
	
}


