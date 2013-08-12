#include "AnimationSet.h"
#include "Action.h"
#include "AnimationObject.h"
#include "AnimationUtil.h"

namespace ma
{
	AnimationSet::AnimationSet(AnimationObject* pAniPlay)
	{
		m_pAnimObject = pAniPlay;
	}

	IAction*	AnimationSet::CreateAction(const char* actionName)
	{
		Action* pAction = new Action(actionName);
		m_arrAnimation.push_back(pAction);
		pAction->SetSkeleton(m_pAnimObject->GetSkeleton());
		return pAction;
	}

	void AnimationSet::AddAnimClip(const char* pszSkaPath, const char* actionName)
	{
		Skeleton* pSkelton = m_pAnimObject->GetSkeleton();
		Action* pAction = AnimationUtil::CreateAction(pszSkaPath,pSkelton,actionName);
		m_arrAnimation.push_back(pAction);
	}

	IAction* AnimationSet::GetActionByName(const char* pszName)
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

	IAction* AnimationSet::GetActionByIndex(UINT index)
	{
		if (index < 0 || index >= m_arrAnimation.size())
			return NULL;

		return m_arrAnimation[index];
	}

	void AnimationSet::RemoveAction(IAction* pAction)
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

