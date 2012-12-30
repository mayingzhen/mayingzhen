#ifndef  _DXRenderDevice__H__
#define  _DXRenderDevice__H__

#include "Common/IRender/IRenderDevice.h"

namespace ma
{
	class DXRENDER_API DxRenderDevice : public IRenderDevice
	{
	public:
		DxRenderDevice();

		~DxRenderDevice();

		void BeginRender();
	
		void EndRender();

		IRendMesh* CreateRendMesh();

		IRendTexture* CreateRendTexture();

		LPDIRECT3DDEVICE9 GetDXDevive() {return m_pd3dDevice;}

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

