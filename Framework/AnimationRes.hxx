#include "Framework/AnimationRes.h"

namespace ma
{
	AnimationRes::AnimationRes(const char* pszPath)
		:Resource(pszPath)
	{
		m_pAnimation  = NULL;
	}

	AnimationRes::~AnimationRes()
	{

	}

	bool AnimationRes::Load()
	{	
		if (m_pAnimation == NULL)
		{
			m_pAnimation = new Animation();
		}
			
		AnimationData* pAniData = LoadAnimationFromBinaryFile(m_sResPath.c_str());
		
		m_pAnimation->InitWithData(pAniData);
		return true;
	}	

	bool AnimationRes::IsLoad()
	{
		return false;
	}
}