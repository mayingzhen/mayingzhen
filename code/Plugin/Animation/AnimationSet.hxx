#include "AnimationSet.h"
#include "AnimationComponent.h"
#include "PoseModifier/PoseModifier.h"

namespace ma
{
	IMPL_OBJECT(AnimationSet,Serializable)

	AnimationSet::AnimationSet()
	{
	}

	AnimationSet::~AnimationSet()
	{
	}

	AnimTreeNode* AnimationSet::GetSkelAnimByName(const char* pszName)
	{
		if (pszName == NULL)
			return NULL;

		for (UINT i = 0; i < m_arrSkelAnim.size(); ++i)
		{
			if (m_arrSkelAnim[i] == NULL)
				continue;

			if (_stricmp(pszName,m_arrSkelAnim[i]->GetName()) == 0)
				return m_arrSkelAnim[i].get();
		}

		return NULL;
	}

	AnimTreeNode* AnimationSet::GetSkelAnimByIndex(UINT index)
	{
		if (index < 0 || index >= m_arrSkelAnim.size())
			return NULL;

		return m_arrSkelAnim[index].get();
	}

	void AnimationSet::AddSkelAnim(AnimTreeNode* pAction)
	{
		if (pAction == NULL)
			return;

		VEC_SkELANIM::iterator it = std::find(m_arrSkelAnim.begin(),m_arrSkelAnim.end(),pAction);
		if (it != m_arrSkelAnim.end())
			return;

		m_arrSkelAnim.push_back(pAction);
	}

	void AnimationSet::RemoveSkelAnim(AnimTreeNode* pAction)
	{
		if (pAction == NULL)
			return;

		VEC_SkELANIM::iterator it = std::find(m_arrSkelAnim.begin(),m_arrSkelAnim.end(),pAction);
		if (it == m_arrSkelAnim.end())
			return;

		m_arrSkelAnim.erase(it);
	}

	void AnimationSet::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		SerializeArrObj<AnimTreeNode>(sl,m_arrSkelAnim,"arrAnimNode");

		sl.EndSection();
	}

	void AnimationSet::SetSkeleton(Skeleton* pSkeleton)
	{
		m_pSkeleton = pSkeleton;
	
		for (UINT i = 0; i < m_arrSkelAnim.size(); ++i)
		{
			m_arrSkelAnim[i]->SetSkeletion(pSkeleton);
		}
	}

	RefPtr<AnimationSet> CreateAnimationSet()
	{
		return new AnimationSet();
	}

	RefPtr<AnimationSet> CreateAnimationSet(const char* pszFile)
	{
		RefPtr<AnimationSet> pAnimSet = new AnimationSet();
		pAnimSet->LoadFromXML(pszFile);
		return pAnimSet;
	}
}

