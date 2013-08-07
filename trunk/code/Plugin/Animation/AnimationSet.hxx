#include "Animation/AnimationSet.h"

namespace ma
{
	AnimationSet::AnimationSet(AnimationPlay* pAniPlay)
	{
		m_pAnimPlay = pAniPlay;
	}

	Action*	AnimationSet::CreateAction(const char* actionName)
	{
		Action* pAction = new Action(actionName);
		m_arrAnimation.push_back(pAction);
		pAction->SetSkeleton(m_pAnimPlay->GetSkeleton());
		return pAction;
	}

	void AnimationSet::AddAnimClip(const char* pszSkaPath, const char* actionName)
	{
		Skeleton* pSkelton = m_pAnimPlay->GetSkeleton();
		Action* pAction = AnimationUtil::CreateAction(pszSkaPath,pSkelton,actionName);
		m_arrAnimation.push_back(pAction);
	}

	Action* AnimationSet::GetActionByName(const char* pszName)
	{
		if (pszName == NULL)
			return NULL;

		for (UINT i = 0; i < m_arrAnimation.size(); ++i)
		{
			if (m_arrAnimation[i] == NULL)
				continue;

			if (_stricmp(pszName,m_arrAnimation[i]->GetAnimName()) == 0)
				return m_arrAnimation[i];
		}

		return NULL;
	}

	Action* AnimationSet::GetActionByIndex(UINT index)
	{
		if (index < 0 || index >= m_arrAnimation.size())
			return NULL;

		return m_arrAnimation[index];
	}

// 	void	AnimationSet::AddAction(Action* pAction) 
// 	{
// 		if (pAction == NULL)
// 			return;
// 
// 		m_arrAnimation.push_back(pAction);
// 	}

	void	AnimationSet::RemoveAction(Action* pAction)
	{
		if (pAction == NULL)
			return;

		std::vector<Action*>::iterator it = std::find(m_arrAnimation.begin(),m_arrAnimation.end(),pAction);
		if (it == m_arrAnimation.end())
			return;

		m_arrAnimation.erase(it);
	}

	void	AnimationSet::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_arrBoneSet);

		//sl.SerializeObjectArray(m_arrPoseModifier);

		sl.Serialize(m_arrAnimation);

		sl.EndSection();
	}

}

