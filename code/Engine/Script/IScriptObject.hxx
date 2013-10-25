#include "IScriptObject.h"

namespace ma
{
	IMPL_OBJECT(IScriptObject,Component);

	IScriptObject::IScriptObject(GameObject* pGameObj):
	  Component(pGameObj)
	{

	}
}

