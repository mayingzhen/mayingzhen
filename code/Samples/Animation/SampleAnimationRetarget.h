#ifndef _Sample_Animation_H_
#define _Sample_Animation_H_

namespace ma
{
	class AnimationComponent;
	class MeshComponent;

	class SampleAnimationRetarget : public Sample
	{
	public:
		SampleAnimationRetarget();

		virtual void	Load();

		virtual void	UnLoad();

		virtual void	Update();

		virtual	void	Render();

	private:
		RefPtr<AnimationComponent>		m_pAnimtionObjectA;
		
		RefPtr<AnimationComponent>		m_pAnimtionObjectB;
	};
}

#endif

