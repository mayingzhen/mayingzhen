#ifndef  _DeferredLightPass__H__
#define  _DeferredLightPass__H__

namespace ma
{
	class Texture;
	class Technique;

	class DeferredLightPass : public RenderPass
	{

	public:
		DeferredLightPass(Scene* pScene);

		void		Init();

		void		Render();

		void		ShoutDown();

	private:
		RefPtr<Technique>	m_pAmbientLight;

		RefPtr<Technique>	m_pDirLight;

		RefPtr<Technique>	m_pPointLight;
	};

}


#endif
