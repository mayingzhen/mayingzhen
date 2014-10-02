#ifndef  _StringRender__H__
#define  _StringRender__H__

namespace ma
{

	class UI_API StringRender
	{

	public:
		void Init();

		void ShutDown();

		void OnFlushFrame();

		void Render();

		void DrawScreenString(const char* pszText,int x,int y,ColourValue cClor);

	private:
		struct TextInfo
		{
			std::string		m_sString;
			int				m_nPosX;
			int				m_nPosY;
			ColourValue		m_cColor;
		};

		std::vector<TextInfo> m_arrTextInfo[2];

		Font*					m_pFont;

	};

	UI_API StringRender*	GetStringRender();

	UI_API void				SetStringRender(StringRender* pStringRender);
}

#endif
