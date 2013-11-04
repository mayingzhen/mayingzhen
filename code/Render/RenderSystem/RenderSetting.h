#ifndef  _RenderSetting__H__
#define  _RenderSetting__H__


namespace ma
{
	struct RENDER_API RenderSetting
	{
	public:
		RenderSetting();

		void	Init();
	
	public:

		bool	m_bShadow;
		
		bool	m_bDefferLight;
		
		bool	m_bThread;

		Color	m_cClearClor;

		float	m_fShadowMapSize;

		bool	m_bHWShadowMap;	
	};

	RENDER_API RenderSetting*	GetRenderSetting();

	RENDER_API void				SetRenderSetting(RenderSetting* pRenderSetting);
}

#endif