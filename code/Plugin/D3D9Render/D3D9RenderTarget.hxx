#include "D3D9RenderTarget.h"
#include "D3D9Texture.h"

namespace ma
{

	D3D9RenderTarget::D3D9RenderTarget()
	{
		m_pTexture = NULL;
		m_pD3D9Surface = NULL;
	}


	D3D9RenderTarget::~D3D9RenderTarget()
	{

	}

	void D3D9RenderTarget::Create(int nWidth,int nHeight,FORMAT format)
	{
		m_pTexture = new D3D9Texture();
		m_pTexture->CreateRT(nWidth,nHeight,format);

		IDirect3DTexture9* pD3D9Texture = ((D3D9Texture*)m_pTexture)->GetD3DTexture();
		pD3D9Texture->GetSurfaceLevel(0,&m_pD3D9Surface);
	}

	Texture* D3D9RenderTarget::GetTexture()
	{
		return m_pTexture;
	}

}


