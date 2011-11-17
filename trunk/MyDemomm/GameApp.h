#ifndef C_GAMEAPP__HH__
#define C_GAMEAPP__HH__

#include <windows.h>
#include <d3dx9.h>
#include <mmsystem.h>


class CGameApp
{
public:
	CGameApp();
	~CGameApp();

	HRESULT Create(HINSTANCE hInstance);
	void Run();

private:
	HRESULT InitDeviceObjects(); 
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT Initialize3DEnvironment();
	HRESULT Render3DEnvironment();
	HRESULT Resize3DEnvironment();

	void InitGame();

	void Render();
	void Update();

private:

	HWND m_hMainWnd;
	LPDIRECT3D9  m_pD3D;				   // The main D3D object
	D3DPRESENT_PARAMETERS m_d3dpp;         // Parameters for CreateDevice/Reset
	LPDIRECT3DDEVICE9 m_pd3dDevice;        // The D3D rendering device

	/// Font
	ID3DXFont* m_pFont;

	BOOL m_bActive;
};

extern LPDIRECT3DDEVICE9 g_pD3DDevice ;   //DIRECT3D …Ë±∏
extern int g_nWndWidth;
extern int g_nWndHeigh;

#endif