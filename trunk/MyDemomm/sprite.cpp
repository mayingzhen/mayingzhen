#include "sprite.h"
#include "public.h"
#include "GameApp.h"


CSprite::CSprite()
{
	memset(&m_rect,0,sizeof(RECT));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_texture = NULL;

	D3DXCreateSprite(g_pD3DDevice, &m_sprite);
}

CSprite::CSprite(const D3DXVECTOR3 &pos,const RECT &rect,const char* fileName,D3DCOLOR colorkey)
{
	m_pos = pos;
	m_rect = rect;

	SetTexture(fileName,colorkey);
}

CSprite::~CSprite()
{
	SAFE_RELEASE(m_sprite);
	SAFE_RELEASE(m_texture);
}

void CSprite::Render()
{
	if (!m_texture)
		return;

	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_sprite->Draw(m_texture, NULL,NULL, &m_pos, 0xffffffff);
	m_sprite->End();
}

void CSprite::SetPos(int x,int y)
{
	m_pos.x = (float)x;
	m_pos.y = (float)y;
	m_pos.z = 0;
}

void CSprite::SetRect(const RECT &rect)
{
	m_rect = rect;
}


void CSprite::SetTexture(const char* fileName,D3DCOLOR colorkey /*要去掉的背景色*/)
{
	//HRESULT res = D3DXCreateTextureFromFile(g_pD3DDevice,fileName,&m_texture);
	HRESULT res = D3DXCreateTextureFromFileEx(g_pD3DDevice,fileName,
		D3DX_DEFAULT_NONPOW2,D3DX_DEFAULT_NONPOW2,D3DX_FROM_FILE,0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		colorkey, NULL, NULL, &m_texture);

	if( FAILED(res) )
	{
		MessageBox(NULL, "Could not SetTexture()",fileName, MB_OK);
	}
	
}

