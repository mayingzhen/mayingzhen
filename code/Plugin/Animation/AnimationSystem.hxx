#include "AnimationSystem.h"

namespace ma
{
	static AnimationSystem* gPAnimationSystem = NULL;

	void SetAnimationSystem(AnimationSystem* pAniSystem)
	{
		gPAnimationSystem = pAniSystem;
	}

	AnimationSystem* GetAnimationSystem()
	{
		return gPAnimationSystem;
	}


	void AnimationSystem::Update()
	{
		profile_code();
	}

}
