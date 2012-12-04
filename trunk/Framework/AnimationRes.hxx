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

	void AnimationRes::Load()
	{	
		if (m_pAnimation == NULL)
		{
			m_pAnimation = new Animation();
		}
		m_pAnimation->Load( m_sResPath.c_str() );
	}	

	bool AnimationRes::IsLoad()
	{
		return false;
	}
}