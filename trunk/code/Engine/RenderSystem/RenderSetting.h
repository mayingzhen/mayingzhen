#ifndef  _RenderSetting__H__
#define  _RenderSetting__H__


namespace ma
{
	struct RENDER_API RenderSetting
	{
	public:
		RenderSetting();
	
	public:

		//bool			m_bShadow;
		
		//bool			m_bDefferLight;
		
		//bool			m_bRenderThread;

		//bool			m_bPartcleThread;

		//ColourValue		m_cClearClor;

		//bool			m_bIsHDRRending;
	};

	RENDER_API RenderSetting*	GetRenderSetting();

	RENDER_API void				SetRenderSetting(RenderSetting* pRenderSetting);
}

#endif