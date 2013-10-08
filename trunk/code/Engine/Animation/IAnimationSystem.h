#ifndef  _IAnimationSystem__H__
#define  _IAnimationSystem__H__

namespace ma
{
	class ISkeleton;
	class IAnimationObject;
	class IAnimationSet;

	class ENGINE_API IAnimationSystem 
	{
	public:
		virtual IAnimationObject*	CreateAnimationObject(const char* pszSkePath,const char* pszAniSetPath) = 0;

		virtual	void				Update() = 0;
	};

	ENGINE_API void SetAnimationSystem(IAnimationSystem* pAnimationSystem);

	ENGINE_API IAnimationSystem* GetAnimationSystem();
}


#endif // _IAnimationSystem__H__