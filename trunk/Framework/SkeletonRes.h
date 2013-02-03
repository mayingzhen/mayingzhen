#ifndef  _SkeletionRes__H__
#define  _SkeletionRes__H__

#include "Resource.h"

namespace ma
{
	class Skeleton;

	class FRAMEWORK_API SkeletonRes : public Resource
	{
	public:
		SkeletonRes(const char* pszPath = NULL);

		~SkeletonRes();

		bool Load();

		bool IsLoad();

		Skeleton* GetSkeleton() {return m_pSkeleton;}

	private:
		Skeleton* m_pSkeleton;
	};
}




#endif