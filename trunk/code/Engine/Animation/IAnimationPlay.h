#ifndef  _IAnimationPlay__H__
#define  _IAnimationPlay__H__

namespace ma
{
	class ISkeleton;
	class IAnimationSet;

	class ENGINE_API IAnimationPlay
	{
	public:
		virtual	ISkeleton*		GetSkeleton() = 0;

 		virtual IAnimationSet*	GetAnimationSet() = 0;

		virtual ISkeleton*		CreateSkeleton(const char* pSkePath) = 0;

		virtual	IAnimationSet*	CreateAnimSet(const char* pAnimSetPath) = 0;

		virtual void			PlayAnimation(const char* pszAnimName) = 0;

		virtual void			AdvanceTime(float fTimeElepse) = 0;

		virtual void			EvaluateAnimation(float fWeight) = 0;

		virtual UINT			GetSkinMatrixNumber() = 0;

		virtual Matrix4x4*		GetSkinMatrixArray() = 0;
	};
}

#endif
