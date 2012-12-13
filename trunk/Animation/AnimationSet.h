#ifndef  _AnimationSet__H__
#define  _AnimationSet__H__

namespace ma
{
	class AnimationAction;
	class Skeleton;

	class ANIMATION_API AnimationSet
	{
	public:
		AnimationAction* GetAnimationActionByName(const char* pszName);

		AnimationInst* LoadAnimation(const char* pszAnimPath,const char* pszAnimName,
			Skeleton* pSkeleton,Skeleton* pOriSkeleton);

		void AddAnimationAction(AnimationAction* pSkelAnim);

	private:
		std::vector<AnimationAction*> m_arrAnimation;
	};
}


#endif
