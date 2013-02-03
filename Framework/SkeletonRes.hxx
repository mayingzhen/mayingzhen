#include "Framework/SkeletonRes.h"

namespace ma
{
	SkeletonRes::SkeletonRes(const char* pszPath)
	:Resource(pszPath)
	{
		m_pSkeleton  = NULL;
	}

	SkeletonRes::~SkeletonRes()
	{

	}

	bool SkeletonRes::Load()
	{	
		if (m_pSkeleton == NULL)
		{
			m_pSkeleton = new Skeleton();
		}

		SkeletonData* pSkelData = ma::LoadSkeletonFromBinaryFile(m_sResPath.c_str());

		m_pSkeleton->InitWithData(*pSkelData);

		//m_pSkeleton->Load( m_sResPath.c_str() );
		return true;
	}	

	bool SkeletonRes::IsLoad()
	{
		return false;
	}
}