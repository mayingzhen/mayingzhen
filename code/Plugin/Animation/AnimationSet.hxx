#include "AnimationSet.h"
#include "Action.h"
#include "AnimationObject.h"
#include "AnimationUtil.h"
#include "PoseModifier/PoseModifier.h"

namespace ma
{
	AnimationSet::AnimationSet(AnimationObject* pAniPlay,ref_ptr<AnimationSetData> pAniSetData)
	{
		m_pAnimObject = pAniPlay;
		m_pAniSetData = pAniSetData;

		for (UINT i = 0; i < pAniSetData->GetActionDataNumber(); ++i)
		{
			ActionData* pActionData = pAniSetData->GetActionDataByIndex(i);
			
			Action* pAction = new Action(pActionData,m_pAnimObject->GetSkeleton());
			m_arrAction.push_back(pAction);
		}
	}

	IAction*	AnimationSet::CreateAction(const char* actionName)
	{
		Action* pAction = new Action(actionName,m_pAnimObject->GetSkeleton());
		m_arrAction.push_back(pAction);
		return pAction;
	}

	void AnimationSet::AddAnimClip(const char* pszSkaPath, const char* actionName)
	{
		Skeleton* pSkelton = m_pAnimObject->GetSkeleton();
		Action* pAction = AnimationUtil::CreateAction(pszSkaPath,pSkelton,actionName);
		m_arrAction.push_back(pAction);
	}

	IAction* AnimationSet::GetActionByName(const char* pszName)
	{
		if (pszName == NULL)
			return NULL;

		for (UINT i = 0; i < m_arrAction.size(); ++i)
		{
			if (m_arrAction[i] == NULL)
				continue;

			if (_stricmp(pszName,m_arrAction[i]->GetAnimName()) == 0)
				return m_arrAction[i];
		}

		return NULL;
	}

	IAction* AnimationSet::GetActionByIndex(UINT index)
	{
		if (index < 0 || index >= m_arrAction.size())
			return NULL;

		return m_arrAction[index];
	}

	void AnimationSet::RemoveAction(IAction* pAction)
	{
		if (pAction == NULL)
			return;

		std::vector<Action*>::iterator it = std::find(m_arrAction.begin(),m_arrAction.end(),pAction);
		if (it == m_arrAction.end())
			return;

		m_arrAction.erase(it);
	}

}

