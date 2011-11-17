#ifndef C_SPRITE_HH_ 
#define C_SPRITE_HH_ 

#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>

class CSprite
{
public:
	CSprite();
	CSprite(const D3DXVECTOR3 &pos,const RECT &rect,const char* fileName,D3DCOLOR colorkey);
	~CSprite();

	void Render();

	void SetPos(int x,int y);
	void SetRect(const RECT &rect);
	void SetTexture(const char* fileName,D3DCOLOR colorkey /*要去掉的背景色*/);

private:
	RECT m_rect;		    //需要绘制的贴图上的一个矩形区域,绘制整过贴图可以指定为NULL
	D3DXVECTOR3	m_pos;      //绘制的位置座标, 也可以指定NULL表示(0,0,0)
	LPD3DXSPRITE m_sprite;
	LPDIRECT3DTEXTURE9	m_texture;
};


#endif