#include "DXRender/DxRendTexture.h"

namespace ma
{
	DxRendTexture::DxRendTexture()
	{
		m_pD3DTex = NULL;
	}

	DxRendTexture::~DxRendTexture()
	{

	}

	bool DxRendTexture::Load(const char* resPath)
	{
		DxRenderDevice* pDxRenderDevice = (DxRenderDevice*)GetRenderDevice();
		HRESULT hr = D3DXCreateTextureFromFile(pDxRenderDevice->GetDXDevive(), resPath, &m_pD3DTex);

		return hr == D3D_OK;
	}
}