#include "AnimationSystem.h"

namespace ma
{
	IAnimationObject*	AnimationSystem::CreateAnimationObject(GameObject* pGameObj)
	{
		AnimationObject* pAnimObject = new AnimationObject(pGameObj);
		m_arrAnimObject.push_back(pAnimObject);
		return pAnimObject;
	}

	void AnimationSystem::DeleteAnimationObject(IAnimationObject* pAnimObj)
	{
		std::vector<AnimationObject*>::iterator it = 
			std::find(m_arrAnimObject.begin(),m_arrAnimObject.end(),pAnimObj);
		if (it != m_arrAnimObject.end())
		{
			m_arrAnimObject.erase(it);
		}

		//SAFE_DELETE(pAnimObj);
	}


	void AnimationSystem::Update()
	{
		profile_code();

		for (UINT i = 0; i < m_arrAnimObject.size(); ++i)
		{
			AnimationObject* pAnimObj = m_arrAnimObject[i];
			if (pAnimObj == NULL)
				continue;

			pAnimObj->AdvanceTime(GetTimer()->GetFrameDeltaTime());

			//if ( !GetRenderSystem()->GetMainCamera()->IsCull( pAnimObj->GetSceneNode()->GetAABBWS() ) )
			{
				pAnimObj->EvaluateAnimation(1.0f);
			}
		}
	}

}
