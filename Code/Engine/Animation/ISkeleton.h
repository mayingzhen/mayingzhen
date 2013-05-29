#ifndef  _ISkeleton__H__
#define  _ISkeleton__H__

namespace ma
{
	struct SkeletonData;

	class ENGINE_API ISkeleton
	{
	public:
		virtual bool				InitWithData(const SkeletonData* skelData) = 0;

		virtual const SkeletonData*	GetSkeletonData() = 0;
	};
}



#endif