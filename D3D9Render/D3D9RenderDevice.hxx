#include "D3D9RenderDevice.h"
#include "D3D9RendMesh.h"
#include "D3D9RendTexture.h"

namespace ma
{

	D3D9RenderDevice::D3D9RenderDevice()
	{

	}

	D3D9RenderDevice::~D3D9RenderDevice()
	{

	}

	IRendMesh* D3D9RenderDevice::CreateRendMesh()
	{
		return new D3D9RendMesh();
	}

	IRendTexture* D3D9RenderDevice::CreateRendTexture()
	{
		return new D3D9RendTexture();
	}

	bool D3D9RenderDevice::Init(HWND hWnd)
	{
		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if(NULL == m_pD3D)
		{
			assert(FALSE && "Direct3DCreate9(D3D_SDK_VERSION)");
			return false;
		}

		RECT rect;
		GetWindowRect(hWnd,&rect);

		ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
		m_d3dpp.Windowed = TRUE;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		m_d3dpp.EnableAutoDepthStencil = TRUE;
		m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Disable vertical synchronization
		m_d3dpp.BackBufferWidth = rect.right - rect.left;
		m_d3dpp.BackBufferHeight = rect.bottom - rect.top;

		UINT AdapterToUse = D3DADAPTER_DEFAULT;
		D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

#if SHIPPING_VERSION
		// When building a shipping version, disable NVPerfHUD (opt-out)
#else
		// Look for 'NVIDIA NVPerfHUD' adapter
		// If it is present, override default settings
		for (UINT Adapter = 0; Adapter < m_pD3D->GetAdapterCount(); Adapter++)
		{
			D3DADAPTER_IDENTIFIER9 Identifier;
			HRESULT Res;
			Res = m_pD3D->GetAdapterIdentifier(Adapter, 0, &Identifier);
			if ( strcmp(Identifier.Description, "NVIDIA NVPerfHUD") == 0 )
			{
				AdapterToUse = Adapter;
				DeviceType = D3DDEVTYPE_REF;
				//Logger::info(String("Using ") + Identifier.Description + " adapter for debug purposes.");
				break;
			}
		}
#endif

		HRESULT hr = S_OK;
		hr = m_pD3D->CreateDevice(AdapterToUse,DeviceType,hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&m_d3dpp,&m_pd3dDevice);

		if( FAILED(hr) )
		{
			assert(FALSE && "m_pD3D->CreateDevice()");
			return false;
		}


		
// 		D3DPRESENT_PARAMETERS d3dpp;
// 		ZeroMemory( &d3dpp, sizeof( d3dpp ) );
// 
// 		d3dpp.BackBufferWidth = rect.right - rect.left;
// 		d3dpp.BackBufferHeight = rect.bottom - rect.top;
// 
// 		d3dpp.Windowed = TRUE;
// 		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
// 		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
// 		d3dpp.EnableAutoDepthStencil = TRUE;
// 		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
// 
// 		m_pd3dDevice->CreateAdditionalSwapChain(&d3dpp,&m_pSwapChain);


		return true;
	}


	void D3D9RenderDevice::BeginRender()
	{
		m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,45,50,170), 1.0f, 0);
		m_pd3dDevice->BeginScene();

		//m_pd3dDevice->GetBackBuffer();

// 		if (NULL == m_pSwapChain)
// 		{
// 			return;
// 		}
// 
// 		IDirect3DDevice9* pd3dDevice;
// 		m_pSwapChain->GetDevice(&pd3dDevice);
// 
// 		m_pSwapChain->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer));
// 		pd3dDevice->SetRenderTarget(0,m_pBackBuffer);
// 
// 		SAFE_RELEASE(pd3dDevice);

	}

	void D3D9RenderDevice::EndRender()
	{
		m_pd3dDevice->EndScene();
		m_pd3dDevice->Present(NULL,NULL,NULL,NULL);
	}

}



