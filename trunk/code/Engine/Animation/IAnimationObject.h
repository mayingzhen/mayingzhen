#ifndef  _IAnimationObject__H__
#define  _IAnimationObject__H__

namespace ma
{
	class ISkeleton;
	class IAnimationSet;

	class ENGINE_API IAnimationObject
	{
	public:
 		virtual IAnimationSet*	GetAnimationSet() = 0;

		virtual void			PlayAnimation(const char* pszAnimName) = 0;

		virtual void			PlayAnimation(UINT actionID) = 0;

		virtual void			AdvanceTime(float fTimeElepse) = 0;

		virtual void			EvaluateAnimation(float fWeight) = 0;

		virtual	void			SetFrame(float fFrame) = 0;

		virtual UINT			GetSkinMatrixNumber() = 0;

		virtual Matrix4x4*		GetSkinMatrixArray() = 0;
	};
}

#endif
