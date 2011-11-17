#include "Text.h"
#include "public.h"
#include "GameApp.h"

Text::Text(char* s, int size, D3DCOLOR color)
{
	m_s	  = s;
	m_size  = size;
	m_color = color;

	m_font=NULL;

	m_rect.left	=0;
	m_rect.top	=0;
	m_rect.right	=0;
	m_rect.bottom	=0;

	D3DXCreateFont(g_pD3DDevice, m_size, 0, FW_NORMAL,
		1, false, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, m_s, &m_font);
}

Text::Text(char* s, int size)
{
	m_s     = s;
	m_size  = size;
	m_color = 0xffffffff;

	m_font=NULL;

	m_rect.left	=0;
	m_rect.top	=0;
	m_rect.right	=0;
	m_rect.bottom	=0;

	D3DXCreateFont(g_pD3DDevice, m_size, 0, FW_NORMAL,
		1, false, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, m_s, &m_font);
}

Text::~Text()
{
	SAFE_RELEASE (m_font);
}

void Text::Render()
{
	if(m_s)
		m_font->DrawText(NULL, m_s, -1, &m_rect, DT_LEFT|DT_NOCLIP, m_color);
}

void Text::SetText(char* s)
{
	m_s=s;
}

void Text::SetColor(D3DCOLOR color)
{
	m_color=color;
}

void Text::SetSize(int size)
{
	m_size=size;
}

void Text::SetPos(int x, int y)
{
	m_rect.left=x;
	m_rect.top=y;
}