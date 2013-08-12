#include "IPhysicsSystem.h"

namespace ma
{
	static IPhysicsSystem* gpPhysicsSystem = NULL;

	void SetPhysicsSystem(IPhysicsSystem* pPhysicsSystem)
	{
		gpPhysicsSystem = pPhysicsSystem;
	}

	IPhysicsSystem* GetPhysicsSystem()
	{
		return gpPhysicsSystem;
	}

}
