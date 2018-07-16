#pragma once

namespace ma
{
	class Texture;
	class Technique;

	class DeferredShading : public Referenced
	{

	public:
		DeferredShading();

		void		Init();
		
		void		Reset();

		void		Render(RenderCommand* pCommond);

		void		Shoutdown();

	private:
		RefPtr<Technique>	m_pAmbientLight;

		RefPtr<Technique>	m_pDirLight;

		RefPtr<Technique>	m_pPointLight;
	};

}


