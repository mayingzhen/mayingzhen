#ifndef  _RenderScheme__H__
#define  _RenderScheme__H__


namespace ma
{
	
	class ENGINE_API GBufferPass
	{
		void DoRender();

	private:
		Technique*					m_pGBufferTech;
		Technique*					m_pGBufferSkinTech;

		RenderTarget*				m_pDepthTex;
		RenderTarget*				m_pNormalTex;

		std::vector<IRenderItem*>	m_SolidEntry;
		std::vector<IRenderItem*>	m_TransEntry;

		Camera*						m_pMainCamera;

	};
	


	class ENGINE_API RenderScheme
	{
		
	};


}






#endif