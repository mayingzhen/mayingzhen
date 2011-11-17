#ifndef C_SNOW_HH__
#define C_SNOW_HH__

#include <d3d9.h>
#include <d3dx9.h>

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }
struct POINTVERTEX//�����ʽ 
{
	D3DXVECTOR3 p;
	D3DCOLOR    color;
};
#define D3DFVF_POINTVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)//�����ʽ
class CSnow
{
public:
	LPDIRECT3DDEVICE9 g_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9      m_pSnowVB;   //���㻺��
	LPDIRECT3DTEXTURE9      m_pTextureSnow;//����
	LONG		m_lSnowCount;//����
	D3DXVECTOR3 m_vPosition;//λ��
	DWORD        m_Width;
	DWORD        m_Lenth;
	DWORD        m_Hight;
	float        m_offset;

	CSnow(LPDIRECT3DDEVICE9 g_pMYd3dDevice);
    ~CSnow(void);

	HRESULT En_Snow_Delete();
	HRESULT En_Snow_Init(LPDIRECT3DDEVICE9 g_pd3dDevice);	//��ʼ��
	HRESULT En_Snow_Move( float offset );
	HRESULT En_Snow_Draw( LPDIRECT3DDEVICE9 g_pd3dDevice);
	void setworld();
	
};

#endif