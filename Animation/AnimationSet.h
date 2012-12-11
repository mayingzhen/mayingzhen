#ifndef  _AnimationSet__H__
#define  _AnimationSet__H__

namespace ma
{
	class SkeletonAnimation;
	class Skeleton;

	class ANIMATION_API AnimationSet
	{
	public:
		SkeletonAnimation* GetSkeletonAnimationByName(const char* pszName);

		AnimationInst* LoadAnimation(const char* pszAnimPath,const char* pszAnimName,
			Skeleton* pSkeleton,Skeleton* pOriSkeleton);

		void AddSkeletonAnimation(SkeletonAnimation* pSkelAnim);

	private:
		std::vector<SkeletonAnimation*> m_arrAnimation;
	};
}


#endif
