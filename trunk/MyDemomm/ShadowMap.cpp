#include "ShadowMap.h"
#include "GameApp.h"

ShadowMap::ShadowMap()
{
	m_pTexture = NULL;
	m_pDSTexture = NULL;
	m_pSurface = NULL;
	m_pDSSurface = NULL;
	m_pOldDSSurface = NULL;
	m_pOldRenderTarget = NULL;
	m_iBytesPerTexel = 0;
}

ShadowMap::~ShadowMap()
{
	Destroy();
}

bool ShadowMap::Create(int iSizeX,int iSizeY)
{
	HRESULT hr;

	// Create a renderable texture
	//
	hr = g_pD3DDevice->CreateTexture( iSizeX, iSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pTexture, NULL );
	assert( SUCCEEDED(hr) );

	hr = m_pTexture->GetSurfaceLevel(0, &m_pSurface);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("GetSurfaceLevel failed!"), TEXT("Error!"), MB_OK);
		return false;
	}

#ifdef PIX_DEBUG
	hr = g_pD3DDevice->CreateDepthStencilSurface(iSizeX, iSizeY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pDSSurface, NULL);
#else
	hr = g_pD3DDevice->CreateTexture( iSizeX, iSizeY, 1, D3DUSAGE_DEPTHSTENCIL, D3DFMT_D24S8, 
		D3DPOOL_DEFAULT, &m_pDSTexture, NULL);
	assert( SUCCEEDED(hr) );

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Creating render texture failed!"), TEXT("Error!"), MB_OK);
		return false;
	}

	hr = m_pDSTexture->GetSurfaceLevel(0, &m_pDSSurface);
	if (FAILED(hr))
	{
 		MessageBox(NULL, TEXT("GetSurfaceLevel failed!"), TEXT("Error!"), MB_OK);
	 	return false;
	 }
#endif

	// Create a new viewport
	m_Viewport.X = 0;
	m_Viewport.Y = 0;
	m_Viewport.Width  = iSizeX;
	m_Viewport.Height = iSizeY;
	m_Viewport.MinZ = 0.0f;
	m_Viewport.MaxZ = 1.0f;

	m_nSizeX = iSizeX;
	m_nSizeY = iSizeY;

	// TexScaleBiasMat
	float fOffsetX = 0.5f + (0.5f / (float)iSizeX);
	float fOffsetY = 0.5f + (0.5f / (float)iSizeY);
	unsigned int range = 1;            //note different scale in DX9!
	float fBias    = 0.0f;
	m_TexScaleBiasMat = D3DXMATRIX( 0.5f,     0.0f,     0.0f,         0.0f,
									0.0f,    -0.5f,     0.0f,         0.0f,
									0.0f,     0.0f,     (float)range, 0.0f,
									fOffsetX, fOffsetY, fBias,        1.0f );

	return true;
}

void ShadowMap::Destroy(void)
{
	SAFE_RELEASE(m_pDSSurface);
	SAFE_RELEASE(m_pSurface);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pDSTexture);

	SAFE_RELEASE(m_pOldDSSurface);
	SAFE_RELEASE(m_pOldRenderTarget);
}

void ShadowMap::EnableRendering(void)
{
	// Store original values
	g_pD3DDevice->GetViewport(&m_OldViewport);
	g_pD3DDevice->GetRenderTarget(0, &m_pOldRenderTarget);
	g_pD3DDevice->GetDepthStencilSurface(&m_pOldDSSurface);

	// Set new values
	g_pD3DDevice->SetViewport(&m_Viewport);
	g_pD3DDevice->SetRenderTarget(0, m_pSurface);
	g_pD3DDevice->SetDepthStencilSurface(m_pDSSurface);
}

void ShadowMap::DisableRendering(void)
{
	g_pD3DDevice->SetDepthStencilSurface(m_pOldDSSurface);
	SAFE_RELEASE(m_pOldDSSurface)

	g_pD3DDevice->SetRenderTarget(0, m_pOldRenderTarget);
	SAFE_RELEASE(m_pOldRenderTarget);

	g_pD3DDevice->SetViewport(&m_OldViewport);
}


