#ifndef  _AnimationSystem__H__
#define  _AnimationSystem__H__

namespace ma
{
	class AnimationPlay;

	class ANIMATION_API AnimationSystem 
	{
	public:

		virtual AnimationPlay*	CreateAnimationPlay(const char* pszSkePath);

		virtual	void			Update();

	private:
		std::vector<AnimationPlay*>	m_arrAnimPlay;
	};

	ANIMATION_API void SetAnimationSystem(AnimationSystem* pAnimationSystem);

	ANIMATION_API AnimationSystem* GetAnimationSystem();
}

#endif
