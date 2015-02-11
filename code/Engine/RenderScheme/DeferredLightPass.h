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

		//Texture*	GetTextureLightDiffuse() {return m_pDiffuse.get();}

		//Texture*	GetTextureightSpecular() {return m_pSpecular.get();}

	private:
		//RefPtr<Texture>		m_pDiffuse;

		//RefPtr<Texture>		m_pSpecular;

		RefPtr<Technique>	m_pAmbientLight;

		RefPtr<Technique>	m_pDirLight;

		RefPtr<Technique>	m_pPointLight;
	};

}


#endif
