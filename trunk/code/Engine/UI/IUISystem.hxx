#include "IUISystem.h"

namespace ma
{
	IUISystem* gpUISystem = NULL;

	void SetUISystem(IUISystem* pUISystem)
	{
		gpUISystem = pUISystem;
	}

	IUISystem* GetUISystem()
	{
		return gpUISystem;
	}
}

