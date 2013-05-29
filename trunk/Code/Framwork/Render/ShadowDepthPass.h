#ifndef  _ShadowDepthPass__H__
#define  _ShadowDepthPass__H__

namespace ma
{
	class FRAMWORK_API ShadowDepthPass
	{

	public:
		ShadowDepthPass();

		~ShadowDepthPass();

		void Render();
	
	protected:
		Light*		m_pMainLigt;

		Camera*		m_pMainCamera;
		
		ShadowMap*	m_arrShadowMap[4];

		std::vector<IRenderItem*>	m_SolidEntry;

		std::vector<IRenderItem*>	m_TransEntry;

		Technique*					m_pShdowMapTech;

		Technique*					m_pSkinShadowMapTech;
	};
}


#endif