#ifndef _Sample_Animation_Retarget_H_
#define _Sample_Animation_Retarget_H_

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

		virtual	void	Render();

		virtual void	keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key);

	private:
		RefPtr<AnimationComponent>		m_pAnimtionObjectA;
		
		RefPtr<AnimationComponent>		m_pAnimtionObjectB;
	};
}

#endif

