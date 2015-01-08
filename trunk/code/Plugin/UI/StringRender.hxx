#include "StringRender.h"
#include "Engine/RenderSystem/RenderThread.h"

namespace ma
{
	static StringRender* gpStringRender = NULL;

	StringRender*	GetStringRender()
	{
		return gpStringRender;
	}

	void SetStringRender(StringRender* pStringRender)
	{
		gpStringRender = pStringRender;
	}

	void StringRender::Init()
	{
		m_pFont =  Font::create("font/arial18.gpb");
	}

	void StringRender::ShutDown()
	{
		//SAFE_DELETE(m_pFont);
	}

	void StringRender::DrawScreenString(const char* pszText,int x,int y,ColourValue cClor)
	{
		TextInfo textInfo;
		textInfo.m_sString = pszText;
		textInfo.m_cColor = cClor;
		textInfo.m_nPosX = x;
		textInfo.m_nPosY = y;

		int index = GetRenderSystem()->CurThreadFill();
		std::vector<TextInfo>& arrTextInfo = m_arrTextInfo[index];

		arrTextInfo.push_back(textInfo);
	}

	void StringRender::OnFlushFrame()
	{
		int index = GetRenderSystem()->CurThreadFill();
		m_arrTextInfo[index].clear();
	}

	void StringRender::Render()
	{
		m_pFont->start();

		int index = GetRenderSystem()->CurThreadProcess();
		std::vector<TextInfo>& arrTextInfo = m_arrTextInfo[index];

		for (UINT i = 0; i < arrTextInfo.size(); i += 2)
		{
			TextInfo& textInfo = arrTextInfo[i];

			m_pFont->drawText(textInfo.m_sString.c_str(),textInfo.m_nPosX,
				textInfo.m_nPosY,textInfo.m_cColor,m_pFont->getSize());
		}

		m_pFont->finish();	
	}

}

