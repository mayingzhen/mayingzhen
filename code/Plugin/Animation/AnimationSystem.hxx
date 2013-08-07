#include "AnimationSystem.h"

namespace ma
{
	static AnimationSystem* gpAnimSystem = NULL;

	void SetAnimationSystem(AnimationSystem* pAnimationSystem)
	{
		gpAnimSystem = pAnimationSystem;
	}

	AnimationSystem* GetAnimationSystem()
	{
		return gpAnimSystem;
	}

	AnimationPlay*	AnimationSystem::CreateAnimationPlay(const char* pszSkePath)
	{
		AnimationPlay* pAnimPlay = new AnimationPlay(pszSkePath);
		m_arrAnimPlay.push_back(pAnimPlay);
		return pAnimPlay;
	}


	void AnimationSystem::Update()
	{
// 		for (UINT i = 0; i < m_arrAnimPlay.size(); ++i)
// 		{
// 			m_arrAnimPlay[i].EvaluateAnimation(1.0f);
// 		}
	}

}