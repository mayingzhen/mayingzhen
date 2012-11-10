#include "DXRender/DxRendWindow.h"
#include "Common/IRender.h"
#include "DxRender/DxRender.h"

void DxRendWindow::Init()
{
	DxRender* pRender = (DxRender*)ma::GetRender();
	if (pRender == NULL)
		return;

	IDirect3DDevice9* pD3DDevice = pRender->GetDXDevive();
	
	D3DPRESENT_PARAMETERS d3dPP;
	memset(&d3dPP, 0, sizeof(d3dPP));
	d3dPP.BackBufferWidth = 800;
	d3dPP.BackBufferHeight = 600;
	d3dPP.Windowed = TRUE;
	d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dPP.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dPP.EnableAutoDepthStencil = TRUE;
	d3dPP.AutoDepthStencilFormat = D3DFMT_D16;

	pD3DDevice->CreateAdditionalSwapChain(&d3dPP,&m_pSwapChain);

}

void DxRendWindow::Shutdown()
{
	SAFE_RELEASE(m_pSwapChain);
}

void DxRendWindow::BeginRender()
{
	if (m_pSwapChain == NULL)
		return;

	IDirect3DDevice9* pD3DDevice = NULL;
	m_pSwapChain->GetDevice(&pD3DDevice);

	m_pSwapChain->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&m_pBackuffer);
	pD3DDevice->SetRenderTarget(0,m_pBackuffer);
	
	SAFE_RELEASE(pD3DDevice);

}

void DxRendWindow::EndRender()
{
	if (m_pSwapChain == NULL)
		return;

	m_pSwapChain->Present(NULL,NULL,m_hWnd,NULL,NULL);
	
	SAFE_RELEASE(m_pBackuffer);
}