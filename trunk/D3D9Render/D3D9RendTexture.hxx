#include "D3D9Render/D3D9RendTexture.h"

namespace ma
{
	D3D9RendTexture::D3D9RendTexture()
	{
		m_pD3DTex = NULL;
	}

	D3D9RendTexture::~D3D9RendTexture()
	{

	}

	bool D3D9RendTexture::Load(const char* pszPath)
	{
		if (pszPath)
		{
			m_sResPath = pszPath;
		}

		D3D9RenderDevice* pDxRenderDevice = (D3D9RenderDevice*)GetRenderDevice();
		HRESULT hr = D3DXCreateTextureFromFile(pDxRenderDevice->GetDXDevive(), m_sResPath.c_str(), &m_pD3DTex);

		return hr == D3D_OK;
	}
}
