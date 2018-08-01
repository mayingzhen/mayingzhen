#pragma once

namespace ma
{
	class SampleIBL : public Sample
	{
	public:
		SampleIBL();

		virtual void Load();

		virtual void UnLoad();

		virtual void Update();

		virtual void PreRender();

		virtual void PostRender();

		virtual void keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key);

	private:
		float m_fMetalness = 0.0;

		float m_fGlossiness = 0.0;
	};
}



