#ifndef  _D3D9RENDERDevice__H__
#define  _D3D9RENDERDevice__H__

#include "Engine/RenderSystem/IRenderDevice.h"

namespace ma
{
	class D3D9RENDER_API D3D9RenderDevice : public IRenderDevice
	{
	public:
		D3D9RenderDevice();

		~D3D9RenderDevice();

		virtual void BeginRender();
	
		virtual void EndRender();

		virtual IRendMesh* CreateRendMesh();

		virtual IRendTexture* CreateRendTexture();

		LPDIRECT3DDEVICE9 GetDXDevive() {return m_pd3dDevice;}

		D3DPRESENT_PARAMETERS* GetD3dPP() {return &m_d3dpp;}

		bool Init(HWND hWnd);

	private:
		LPDIRECT3D9  m_pD3D;				   // The main D3D object
		D3DPRESENT_PARAMETERS m_d3dpp;         // Parameters for CreateDevice/Reset
		LPDIRECT3DDEVICE9 m_pd3dDevice;        // The D3D rendering device

		//IDirect3DSwapChain9* m_pSwapChain;
		//IDirect3DSurface9*	m_pBackBuffer;

	};
}

#endif


