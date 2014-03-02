#ifndef  _DeferredLightPass__H__
#define  _DeferredLightPass__H__

namespace ma
{
	class DeferredLightPass : public RenderPass
	{
		DECL_OBJECT(DeferredLightPass)

	public:

		void		Init();

		void		Render();

		void		ShoutDown();

		Texture*	GetTextureLightDiffuse() {return m_pDiffuse;}

		Texture*	GetTextureightSpecular() {return m_pSpecular;}

	private:
		Texture*	m_pDiffuse;

		Texture*	m_pSpecular;

		Technique*	m_pAmbientLight;

		Technique*	m_pDirLight;

		Technique*	m_pPointLight;
	};

	DeferredLightPass*	GetDeferredLightPass();

}


#endif
