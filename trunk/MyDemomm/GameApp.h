#ifndef C_GAMEAPP__HH__
#define C_GAMEAPP__HH__

#include <windows.h>
#include <d3dx9.h>
#include <mmsystem.h>

#define  g_nWndWidth   g_GameApp.GetWndWith()
#define  g_nWndHeigh   g_GameApp.GetWndHeigh()
#define  g_pD3DDevice  g_GameApp.GetD3dDevice()
#define  g_ElapsedTime g_GameApp.GetElapsedTime()

namespace ma
{
	class Scene;
	class Camera;
}


class CGameApp
{
public:
	CGameApp();
	~CGameApp();

	HRESULT Create(HINSTANCE hInstance);
	void Run();

	LPDIRECT3DDEVICE9 GetD3dDevice() {return m_pd3dDevice;}
	int GetWndWith() {return m_nWndWidth;}
	int GetWndHeigh() {return m_nWndHeigh;}
	float GetElapsedTime() {return m_fElapsedTime;}

private:
	HRESULT InitDeviceObjects(); 
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT Render3DEnvironment();
	HRESULT Resize3DEnvironment();

	void InitGame();

	void Render();
	void Update();

private:

	ma::Scene* m_pScene;
	ma::Camera* m_pCamera;

	HWND m_hMainWnd;
	LPDIRECT3D9  m_pD3D;				   // The main D3D object
	D3DPRESENT_PARAMETERS m_d3dpp;         // Parameters for CreateDevice/Reset
	LPDIRECT3DDEVICE9 m_pd3dDevice;        // The D3D rendering device
	int m_nWndWidth;
	int m_nWndHeigh;
	float m_fElapsedTime;

	/// Font
	ID3DXFont* m_pFont;
};

extern CGameApp g_GameApp;

#endif