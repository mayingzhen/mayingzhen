#ifndef C_SNOW_HH__
#define C_SNOW_HH__

#include <d3d9.h>
#include <d3dx9.h>

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }
struct POINTVERTEX//顶点格式 
{
	D3DXVECTOR3 p;
	D3DCOLOR    color;
};
#define D3DFVF_POINTVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)//灵活顶点格式
class CSnow
{
public:
	LPDIRECT3DDEVICE9 g_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9      m_pSnowVB;   //顶点缓冲
	LPDIRECT3DTEXTURE9      m_pTextureSnow;//纹理
	LONG		m_lSnowCount;//个数
	D3DXVECTOR3 m_vPosition;//位置
	DWORD        m_Width;
	DWORD        m_Lenth;
	DWORD        m_Hight;
	float        m_offset;

	CSnow(LPDIRECT3DDEVICE9 g_pMYd3dDevice);
    ~CSnow(void);

	HRESULT En_Snow_Delete();
	HRESULT En_Snow_Init(LPDIRECT3DDEVICE9 g_pd3dDevice);	//初始化
	HRESULT En_Snow_Move( float offset );
	HRESULT En_Snow_Draw( LPDIRECT3DDEVICE9 g_pd3dDevice);
	void setworld();
	
};

#endif