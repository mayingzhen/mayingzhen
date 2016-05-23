#pragma once

namespace ma
{
	class WwiseComponent;

	class SampleWwise : public Sample
	{
	public:
		SampleWwise();

		virtual void Load();

		virtual void UnLoad();

		virtual void Update();

		virtual void Render();

		virtual void keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key);

	private:
		RefPtr<WwiseComponent> m_pWwiseComp;
	};
}



