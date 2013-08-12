#include "IAnimationSystem.h"

namespace ma
{
	IAnimationSystem* gpAnimationSystem = NULL;

	void SetAnimationSystem(IAnimationSystem * pAnimationSystem)
	{
		gpAnimationSystem = pAnimationSystem;
	}

	IAnimationSystem * GetAnimationSystem()
	{
		return gpAnimationSystem;
	}

}