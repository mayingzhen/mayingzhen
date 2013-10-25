#ifndef  _IAnimationObject__H__
#define  _IAnimationObject__H__

namespace ma
{
	class ISkeleton;
	class IAnimationSet;

	class ENGINE_API IAnimationObject : public Component
	{
		DECL_OBJECT(IAnimationObject)
	
	public:
		IAnimationObject(GameObject* pGameObj);

		virtual void			Load(const char* pszAniSetPath, const char* pszSkeletonPath) = 0;

		virtual void			Serialize(Serializer& sl, const char* pszLable = "AnimationObject") = 0;

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
