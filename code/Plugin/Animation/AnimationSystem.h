#ifndef  _AnimationSystem__H__
#define  _AnimationSystem__H__

namespace ma
{
	class AnimationComponent;

	class ANIMATION_API AnimationSystem 
	{
	public:

		virtual AnimationComponent*	CreateAnimationObject();

		virtual	void				Update();

	private:
		typedef std::vector< RefPtr<AnimationComponent> > VEC_ANIMCOMP;
		VEC_ANIMCOMP				m_arrAnimObject;
	};

	ANIMATION_API void SetAnimationSystem(AnimationSystem* pAniSystem);
	ANIMATION_API AnimationSystem* GetAnimationSystem();
}

#endif