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

		void DrawScreenString(const char* pszText,int x,int y,Vector4 cClor);

	private:
		struct TextInfo
		{
			std::string m_sString;
			int			m_nPosX;
			int			m_nPosY;
			Vector4		m_cColor;
		};

		std::vector<TextInfo> m_arrTextInfo[2];

		Font*					m_pFont;

	};

	UI_API StringRender*	GetStringRender();

	UI_API void			SetStringRender(StringRender* pStringRender);
}

#endif
