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

	void SkeletonRes::Load()
	{	
		if (m_pSkeleton == NULL)
		{
			m_pSkeleton = new Skeleton();
		}

		SkeletonData* pSkelData = ma::LoadSkeletonFromBinaryFile(m_sResPath.c_str());

		m_pSkeleton->InitWithData(*pSkelData);

		//m_pSkeleton->Load( m_sResPath.c_str() );
	}	

	bool SkeletonRes::IsLoad()
	{
		return false;
	}
}