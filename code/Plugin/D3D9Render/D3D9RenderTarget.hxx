#include "D3D9RenderTarget.h"
#include "D3D9Texture.h"

namespace ma
{

	D3D9RenderTarget::D3D9RenderTarget(int nWidth,int nHeight,FORMAT format)
	{
		m_pTexture = NULL;
		m_pTexture = new D3D9Texture(nWidth,nHeight,format);
	}


	D3D9RenderTarget::~D3D9RenderTarget()
	{

	}

	void D3D9RenderTarget::Create()
	{
		m_pTexture->CreateRT();

		IDirect3DTexture9* pD3D9Texture = ((D3D9Texture*)m_pTexture)->GetD3DTexture();
		pD3D9Texture->GetSurfaceLevel(0,&m_pD3D9Surface);
	}

	Texture* D3D9RenderTarget::GetTexture()
	{
		return m_pTexture;
	}

}


