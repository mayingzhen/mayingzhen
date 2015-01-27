#ifndef  _AnimationSystem__H__
#define  _AnimationSystem__H__

namespace ma
{
	class AnimationComponent;

	class AnimationSystem 
	{
	public:

		virtual AnimationComponent*	CreateAnimationObject();

		virtual	void				Update();

	private:
		typedef std::vector< RefPtr<AnimationComponent> > VEC_ANIMCOMP;
		VEC_ANIMCOMP				m_arrAnimObject;
	};

	void SetAnimationSystem(AnimationSystem* pAniSystem);
	AnimationSystem* GetAnimationSystem();
}

#endif