#ifndef  _ActionUtil__H__
#define  _ActionUtil__H__

namespace ma
{
	class Action;
	class Skeleton;

	namespace AnimationUtil
	{
		Action* CreateAction(const char* pszAnimPath,Skeleton* pSkeleton,const char* pszActionName = NULL);	
	}
}

#endif
