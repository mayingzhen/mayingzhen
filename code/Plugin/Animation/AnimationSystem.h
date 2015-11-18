#ifndef  _AnimationSystem__H__
#define  _AnimationSystem__H__

namespace ma
{
	class AnimationComponent;

	class AnimationSystem 
	{
	public:

		virtual	void	Update();
	};

	void SetAnimationSystem(AnimationSystem* pAniSystem);
	AnimationSystem* GetAnimationSystem();
}

#endif