#pragma once

namespace ma
{
	class SampleIBL : public Sample
	{
	public:
		SampleIBL();

		virtual void Load();

		virtual void UnLoad();

		virtual void keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key);

	private:

	};
}



