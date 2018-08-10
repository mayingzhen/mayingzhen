#pragma once

namespace ma
{
	class MeshComponent;

	class SampleAnimationTree : public Sample
	{
	public:
		SampleAnimationTree();

		virtual void Load();

		virtual void UnLoad();

		virtual void keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key);

	private:

		RefPtr<AnimClipNode>		m_pClip602;
		RefPtr<AnimClipNode>		m_pClip120;

		AnimationComponent* m_pAnimator;
	};
}



