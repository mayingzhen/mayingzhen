#include "AnimationDevice.h"

namespace ma
{
	ISkeleton*		AnimationDevice::CreateSkeleton()
	{
		return new Skeleton();
	}

	IAnimationPlay*	AnimationDevice::CreateAnimationPlay()
	{
		AnimationPlay* pAnimPlay = new AnimationPlay();
		m_arrAnimPlay.push_back(pAnimPlay);
		return pAnimPlay;
	}

	IAnimationSet*	AnimationDevice::CreateAnimationSet(IAnimationPlay* pAnimPlay)
	{
		return new AnimationSet((AnimationPlay*)pAnimPlay);
	}

	void AnimationDevice::Update()
	{
// 		for (UINT i = 0; i < m_arrAnimPlay.size(); ++i)
// 		{
// 			m_arrAnimPlay[i].EvaluateAnimation(1.0f);
// 		}
	}

}