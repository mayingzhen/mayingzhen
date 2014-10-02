#include "IScriptObject.h"

namespace ma
{
	IMPL_OBJECT(IScriptObject,Component);

	IScriptObject::IScriptObject(SceneNode* pGameObj):
	  Component(pGameObj)
	{

	}
}

