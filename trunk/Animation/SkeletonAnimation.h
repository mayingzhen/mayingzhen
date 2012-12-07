#ifndef  _SkeletonAnimation__H__
#define  _SkeletonAnimation__H__

namespace ma
{
	class ANIMATION_API SkeletonAnimation
	{
	public:
		SkeletonAnimation();

		~SkeletonAnimation();



	private:
		Skeleton* m_pSkeleton;
		Animation* m_pAnimation;
	};
}

#endif