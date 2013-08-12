#include "IScriptSystem.h"


namespace ma
{
	static IScriptSystem* gpScriptSystem = NULL;

	IScriptSystem* GetScriptSystem()
	{
		return gpScriptSystem;
	}

	void SetScriptSystem(IScriptSystem* pScriptSystem)
	{
		gpScriptSystem = pScriptSystem;
	}



}