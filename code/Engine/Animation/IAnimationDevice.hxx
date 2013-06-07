#include "IAnimationDevice.h"

namespace ma
{
	IAnimationDevice* g_pAnimationDevice = NULL;

	void SetAnimationDevice(IAnimationDevice* pAnimationDevice)
	{
		g_pAnimationDevice = pAnimationDevice;
	}

	IAnimationDevice* GetAnimationDevice()
	{
		return g_pAnimationDevice;
	}

}