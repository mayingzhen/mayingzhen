#ifndef _Sample_Animation_IK_H_
#define _Sample_Animation_IK_H_

namespace ma
{
	class AnimationComponent;
	class MeshComponent;

	class SampleAnimationIK : public Sample
	{
	public:
		SampleAnimationIK();

		virtual void	Load();

		virtual void	UnLoad();

		virtual void	Update();

		virtual	void	Render();

		virtual void	keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key);

	private:
		RefPtr<AnimationComponent> m_pAnimComp;
	};
}

#endif

