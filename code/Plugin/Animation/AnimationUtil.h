#ifndef  _ActionUtil__H__
#define  _ActionUtil__H__

namespace ma
{
	class SkelAnimtion;
	class Skeleton;

	namespace AnimationUtil
	{
		SkelAnimtion* CreateAction(const char* pszAnimPath,Skeleton* pSkeleton,const char* pszActionName = NULL);	
	}
}

#endif
