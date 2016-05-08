#pragma once

namespace ma
{
	class Texture;
	class Technique;

	class DeferredShadingPass : public RenderPass
	{

	public:
		DeferredShadingPass(Scene* pScene);

		void		Init();
		
		void		Reset();

		void		Render();

		void		Shoutdown();

	private:
		RefPtr<Technique>	m_pAmbientLight;

		RefPtr<Technique>	m_pDirLight;

		RefPtr<Technique>	m_pPointLight;
	};

}

