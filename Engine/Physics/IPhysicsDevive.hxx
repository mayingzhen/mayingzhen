#include "IPhysicsDevive.h"

namespace ma
{
	IPhysicsDevice* g_pPhysicsDevice = NULL;

	void SetPhysicsDevice(IPhysicsDevice* pPhysicsDevice)
	{
		g_pPhysicsDevice = pPhysicsDevice;
	}

	IPhysicsDevice* GetPhysicsDevice()
	{
		return g_pPhysicsDevice;
	}

}
