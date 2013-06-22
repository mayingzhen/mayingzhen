#ifndef  _DeferredLighting__H__
#define  _DeferredLighting__H__


namespace ma
{

	class DeferredLighting 
	{
	public:
		void						Init();

		void						GBufferPass();	

		void						DefferdLigtPass();

		void						ShadingPass();

		void						SetCamera(Camera* pCamera);

		void						ClearLightList();

		void						AddLight(Light* pLight);

	private:
		// GBuffer
		//ShaderProgram*				m_pGBufferTech;
		//ShaderProgram*				m_pGBufferSkinTech;
		RenderTarget*				m_pDepthTex;
		RenderTarget*				m_pNormalTex;

		// Light
	 	//ShaderProgram*				m_DeferredLightTech;
		RenderTarget*				m_pDiffuseTex;
		RenderTarget*				m_pSpeculaTex;

		//ShaderProgram*				m_pShadingTech;
		
		std::vector<IRenderItem*>	m_SolidEntry;
		std::vector<IRenderItem*>	m_TransEntry;

		Camera*						m_pMainCamera;

		std::vector<Light*>			m_Ligts;

		Light*						m_mainLigt;

		ScreenQuad*					m_pScreenQuad;
		
		UnitSphere*					m_pUnitSphere;
	};

}

#endif


