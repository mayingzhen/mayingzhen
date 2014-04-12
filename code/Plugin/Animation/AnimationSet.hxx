#include "AnimationSet.h"
#include "SkelAnimtion.h"
#include "AnimationComponent.h"
#include "AnimationUtil.h"
#include "PoseModifier/PoseModifier.h"

namespace ma
{
	AnimationSet::AnimationSet(ref_ptr<Skeleton> pSkeleton,ref_ptr<AnimationSetData> pAniSetData)
	{
		m_pSkeleton = pSkeleton;
		m_pAniSetData = pAniSetData;

		for (UINT i = 0; i < pAniSetData->GetActionDataNumber(); ++i)
		{
			SkelAnimData* pActionData = pAniSetData->GetActionDataByIndex(i);
			
			SkelAnimtion* pAction = new SkelAnimtion(pActionData,m_pSkeleton.get());
			m_arrSkelAnim.push_back(pAction);
		}
	}

	AnimationSet::~AnimationSet()
	{
		for (UINT i = 0; i < m_arrSkelAnim.size(); ++i)
		{
			SAFE_DELETE(m_arrSkelAnim[i]);
		}
		m_arrSkelAnim.clear();	
	}

	SkelAnimtion*	AnimationSet::CreateSkelAnim(const char* actionName)
	{
		SkelAnimtion* pAction = new SkelAnimtion(actionName,m_pSkeleton.get());
		m_arrSkelAnim.push_back(pAction);
		return pAction;
	}

	void AnimationSet::AddAnimClip(const char* pszSkaPath, const char* actionName)
	{
		SkelAnimtion* pAction = AnimationUtil::CreateAction(pszSkaPath,m_pSkeleton.get(),actionName);
		m_arrSkelAnim.push_back(pAction);
	}

	SkelAnimtion* AnimationSet::GetSkelAnimByName(const char* pszName)
	{
		if (pszName == NULL)
			return NULL;

		for (UINT i = 0; i < m_arrSkelAnim.size(); ++i)
		{
			if (m_arrSkelAnim[i] == NULL)
				continue;

			if (_stricmp(pszName,m_arrSkelAnim[i]->GetAnimName()) == 0)
				return m_arrSkelAnim[i];
		}

		return NULL;
	}

	SkelAnimtion* AnimationSet::GetSkelAnimByIndex(UINT index)
	{
		if (index < 0 || index >= m_arrSkelAnim.size())
			return NULL;

		return m_arrSkelAnim[index];
	}

	void AnimationSet::RemoveSkelAnim(SkelAnimtion* pAction)
	{
		if (pAction == NULL)
			return;

		std::vector<SkelAnimtion*>::iterator it = std::find(m_arrSkelAnim.begin(),m_arrSkelAnim.end(),pAction);
		if (it == m_arrSkelAnim.end())
			return;

		m_arrSkelAnim.erase(it);
	}

}

