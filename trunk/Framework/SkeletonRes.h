#ifndef  _SkeletionRes__H__
#define  _SkeletionRes__H__

#include "Resource.h"


namespace ma
{
	class ISkeleton;

	class FRAMEWORK_API SkeletonRes : public Resource
	{
	public:
		SkeletonRes(const char* pszPath = NULL);

		~SkeletonRes();

		void Load();

		bool IsLoad();

	private:
		Skeleton* m_skeleton;
	};
}




#endif