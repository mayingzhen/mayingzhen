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

		Vector4 m_cColor = Vector4(1.0);

		RefPtr<SamplerState> m_pDiffuseTexture;

		float m_fExporse = 0.5f;
		float m_fParam = 2.0f;
	};
}



