#include "IAnimationObject.h"

namespace ma
{
	IMPL_OBJECT(IAnimationObject,Component)

	IAnimationObject::IAnimationObject(GameObject* pGameObj)
			:Component(pGameObj)
	{

	}


}
