#include "DXRender/DxRender.h"
#include "DXRender/DxRenderDevice.h"

namespace ma
{
	DxRender::DxRender()
	{
// 		DxRenderDevice* pRenderDevice = new DxRenderDevice();
// 		SetRenderDevice(pRenderDevice);
	}

	DxRender::~DxRender()
	{
// 		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
// 		SAFE_DELETE(pRenderDevice);
	}

	void DxRender::BeginRender()
	{
		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
		pRenderDevice->BeginRender();
		//m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,45,50,170), 1.0f, 0);
		//m_pd3dDevice->BeginScene();
	}

	void DxRender::EndRender()
	{
		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
		pRenderDevice->EndRender();
		//m_pd3dDevice->EndScene();
		//m_pd3dDevice->Present(NULL,NULL,NULL,NULL);
	}

	void DxRender::RenderSkelMesh(IRendMesh* pSkelMesh)
	{

	}
	
	void DxRender::SetViewMatrix(const D3DXMATRIX* viewMatrix)
	{
		
	}

	void DxRender::SetProjMatrix(const D3DXMATRIX* projMatrix)
	{
		
	}

	void DxRender::RenderMesh(D3DXMATRIX* pWordMat,IRendMesh* pMesh,IRendTexture* pTexture)
	{
		DxRendMesh* pDxMesh = (DxRendMesh*)pMesh;
		DxRendTexture* pDxTexure = (DxRendTexture*)(pTexture);

		//m_pd3dDevice->setvi

	}


// 	bool DxRender::Init(HWND hWnd)
// 	{
// 		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
// 		if(NULL == m_pD3D)
// 		{
// 			assert(FALSE && "Direct3DCreate9(D3D_SDK_VERSION)");
// 			return false;
// 		}
// 
// 		RECT rect;
// 		GetWindowRect(hWnd,&rect);
// 
// 		ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
// 		m_d3dpp.Windowed = TRUE;
// 		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
// 		m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
// 		m_d3dpp.EnableAutoDepthStencil = TRUE;
// 		m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
// 		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
// 		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Disable vertical synchronization
// 		m_d3dpp.BackBufferWidth = rect.right - rect.left;
// 		m_d3dpp.BackBufferHeight = rect.bottom - rect.top;
// 
// 		UINT AdapterToUse = D3DADAPTER_DEFAULT;
// 		D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;
// 
// #if SHIPPING_VERSION
// 		// When building a shipping version, disable NVPerfHUD (opt-out)
// #else
// 		// Look for 'NVIDIA NVPerfHUD' adapter
// 		// If it is present, override default settings
// 		for (UINT Adapter = 0; Adapter < m_pD3D->GetAdapterCount(); Adapter++)
// 		{
// 			D3DADAPTER_IDENTIFIER9 Identifier;
// 			HRESULT Res;
// 			Res = m_pD3D->GetAdapterIdentifier(Adapter, 0, &Identifier);
// 			if ( strcmp(Identifier.Description, "NVIDIA NVPerfHUD") == 0 )
// 			{
// 				AdapterToUse = Adapter;
// 				DeviceType = D3DDEVTYPE_REF;
// 				//Logger::info(String("Using ") + Identifier.Description + " adapter for debug purposes.");
// 				break;
// 			}
// 		}
// #endif
// 
// 		HRESULT hr = S_OK;
// 		hr = m_pD3D->CreateDevice(AdapterToUse,DeviceType,hWnd,
// 			D3DCREATE_HARDWARE_VERTEXPROCESSING,
// 			&m_d3dpp,&m_pd3dDevice);
// 
// 		if( FAILED(hr) )
// 		{
// 			assert(FALSE && "m_pD3D->CreateDevice()");
// 			return false;
// 		}
// 
// 		return true;
// 	}
}


