#include "AnimationSystem.h"

namespace ma
{
	IAnimationObject*	AnimationSystem::CreateAnimationObject(const char* pszSkePath)
	{
		AnimationObject* pAnimObject = new AnimationObject(pszSkePath);
		m_arrAnimObject.push_back(pAnimObject);
		return pAnimObject;
	}


	void AnimationSystem::Update()
	{
		for (UINT i = 0; i < m_arrAnimObject.size(); ++i)
		{
			m_arrAnimObject[i]->AdvanceTime(GetTimer()->GetFrameDeltaTime());
			m_arrAnimObject[i]->EvaluateAnimation(1.0f);
		}
	}

}