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
	void SetTexture(const char* fileName,D3DCOLOR colorkey /*Ҫȥ���ı���ɫ*/);

private:
	RECT m_rect;		    //��Ҫ���Ƶ���ͼ�ϵ�һ����������,����������ͼ����ָ��ΪNULL
	D3DXVECTOR3	m_pos;      //���Ƶ�λ������, Ҳ����ָ��NULL��ʾ(0,0,0)
	LPD3DXSPRITE m_sprite;
	LPDIRECT3DTEXTURE9	m_texture;
};


#endif