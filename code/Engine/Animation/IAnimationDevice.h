#ifndef  _IAnimationDevice__H__
#define  _IAnimationDevice__H__

namespace ma
{
	class ISkeleton;
	class IAnimationPlay;
	class IAnimationSet;

	class ENGINE_API IAnimationDevice /*: public IDevice*/
	{
	public:

		virtual ISkeleton*		CreateSkeleton() = 0;

		virtual IAnimationPlay*	CreateAnimationPlay() = 0;

		virtual	IAnimationSet*	CreateAnimationSet(IAnimationPlay* pAnimPlay) = 0;
	};

	ENGINE_API void SetAnimationDevice(IAnimationDevice* pAnimationDevice);

	ENGINE_API IAnimationDevice* GetAnimationDevice();
}


#endif