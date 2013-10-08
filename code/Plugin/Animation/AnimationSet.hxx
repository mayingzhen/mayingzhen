#include "AnimationSet.h"
#include "Action.h"
#include "AnimationObject.h"
#include "AnimationUtil.h"
#include "PoseModifier/PoseModifier.h"

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

		if (sl.IsReading())
		{
			UINT nBoneSetSize;
			sl.Serialize(nBoneSetSize,"arrBoneSetSize");
			for (UINT nCnt = 0;nCnt < nBoneSetSize; ++nCnt)
			{
				BoneSet* pBoneSet = new BoneSet();
				pBoneSet->Serialize(sl);
				m_arrBoneSet.push_back(pBoneSet);
			}

			UINT nPMSize;
			sl.Serialize(nPMSize,"arrPoseModifier");
			for (UINT nCnt = 0; nCnt < nPMSize; ++nCnt)
			{
				std::string strTypeName;
				sl.Serialize(strTypeName,"PoseModifierType");

				ObjectFactoryManager& objFac = ObjectFactoryManager::GetInstance();
				PoseModifier* pPoseModifier = SafeCast<PoseModifier>(objFac.CreateObject(strTypeName.c_str()));
				m_arrPoseModifier.push_back(pPoseModifier);

				pPoseModifier->Serialize(sl);
			}

			UINT nAnimationSize;
			sl.Serialize(nAnimationSize,"arrAnimationSize");
			for (UINT nCnt = 0;nCnt < nAnimationSize; ++nCnt)
			{
				IAction* pAction = CreateAction("");
				pAction->Serialize(sl);
			}
		}
		else
		{

			UINT nBoneSetSize = m_arrBoneSet.size();
			sl.Serialize(nBoneSetSize,"arrBoneSetSize");
			for (UINT nCnt = 0;nCnt < nBoneSetSize; ++nCnt)
			{
				BoneSet* pBoneSet = m_arrBoneSet[nCnt];
				pBoneSet->Serialize(sl);
			}

			UINT nPMSize = m_arrPoseModifier.size();
			sl.Serialize(nPMSize,"arrPoseModifier");
			for (UINT nCnt = 0; nCnt < nPMSize; ++nCnt)
			{
				PoseModifier* pPoseModifier = m_arrPoseModifier[nCnt];

				std::string strTypeName = pPoseModifier->GetClass()->GetName();
				sl.Serialize(strTypeName,"PoseModifierType");

				pPoseModifier->Serialize(sl);
			}

			UINT nAnimationSize = m_arrAnimation.size();
			sl.Serialize(nAnimationSize,"arrAnimationSize");
			for (UINT nCnt = 0;nCnt < nAnimationSize; ++nCnt)
			{
				Action* pAction = m_arrAnimation[nCnt];
				pAction->Serialize(sl);
			}
		}

		sl.EndSection();
	}

}

