#include "D3D9RenderTarget.h"
#include "D3D9Texture.h"

namespace ma
{

	D3D9RenderTarget::D3D9RenderTarget()
	{
		mRenderTarget = NULL;
	}


	D3D9RenderTarget::~D3D9RenderTarget()
	{

	}

	bool D3D9RenderTarget::Create(int nWidth,int nHeight,FORMAT format)
	{
		m_pTexture = new D3D9Texture();
		m_pTexture->CreateRT(nWidth,nHeight,format);

		IDirect3DTexture9* pD3D9Texture = ((D3D9Texture*)m_pTexture)->GetD3DTexture();
		pD3D9Texture->GetSurfaceLevel(0,&mRenderTarget);
		
		return true;

// 		if (nWidth == -1 && nHeight == -1)
// 		{
// 			GetRenderDevice()->GetRenderWndSize(nWidth,nHeight);
// 		}
// 
// 		ASSERT(nWidth && nHeight);
// 		//d_assert (FindTexture(sName).IsNull());
// 
// 		//if (!RenderSystem::Instance()->CheckRenderTargetFormat(Format))
// 		//{
// 		//	EXCEPTION("Your device can't support texture format '" + D3D9Mapping::GetFormatString(Format) + 
// 		//		"' for render target");
// 		//}
// 
// 		HRESULT hr = D3D_OK;
// 		DWORD D3DUsage = D3DUSAGE_RENDERTARGET;
// 		D3DPOOL D3DPool = D3DPOOL_DEFAULT;
// 		D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(format);
// 		IDirect3DTexture9 * pD3D9Texture = NULL;
// 
// 		D3D9RenderDevice* pDxRenderDevice = (D3D9RenderDevice*)GetRenderDevice();
// 
// 		hr = GetD3D9DxDevive()->CreateTexture(
// 			nWidth, nHeight, 1,
// 			D3DUsage, D3DFormat, D3DPool,
// 			&pD3D9Texture, NULL);
// 
// 		// 		if (FAILED(hr))
// 		// 		{
// 		// 			EXCEPTION("D3D Error: CreateTexture failed, desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
// 		// 		}
// 
// 		m_pD3DTex = pD3D9Texture;
// 		mWidth = nWidth;
// 		mHeight = nHeight;
// 		mFormat = format;
// 
// 		hr = pD3D9Texture->GetSurfaceLevel(0,&mRenderTarget);
// 
// 		return true;
	}

}


