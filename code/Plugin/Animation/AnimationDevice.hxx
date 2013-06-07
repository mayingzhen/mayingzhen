#include "AnimationDevice.h"

namespace ma
{
	ISkeleton*		AnimationDevice::CreateSkeleton()
	{
		return new Skeleton();
	}

	IAnimationPlay*	AnimationDevice::CreateAnimationPlay()
	{
		return new AnimationPlay();
	}

	IAnimationSet*	AnimationDevice::CreateAnimationSet()
	{
		return new AnimationSet();
	}

}