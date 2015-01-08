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

	AnimationComponent*	AnimationSystem::CreateAnimationObject()
	{
		AnimationComponent* pAnimObject = new AnimationComponent();
		m_arrAnimObject.push_back(pAnimObject);
		return pAnimObject;
	}

	void AnimationSystem::Update()
	{
		profile_code();

// 		for (UINT i = 0; i < m_arrAnimObject.size(); ++i)
// 		{
// 			AnimationComponent* pAnimObj = m_arrAnimObject[i].get();
// 			if (pAnimObj == NULL)
// 				continue;
// 			
// 			pAnimObj->AdvanceTime( GetTimer()->GetFrameDeltaTime() );
// 
// 			if (pAnimObj->GetSceneNode()->GetLastVisibleFrame() == GetTimer()->GetFrameCount())
// 			{
// 				pAnimObj->EvaluateAnimation();
// 			}
// 		}
	}

}
