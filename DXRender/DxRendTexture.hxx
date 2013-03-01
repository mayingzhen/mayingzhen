#include "DXRender/DxRendTexture.h"

namespace ma
{
	DxRendTexture::DxRendTexture(const char* resPath):
	IRendTexture(resPath)
	{
		m_pD3DTex = NULL;
	}

	DxRendTexture::~DxRendTexture()
	{

	}

	bool DxRendTexture::Load()
	{
		DxRenderDevice* pDxRenderDevice = (DxRenderDevice*)GetRenderDevice();
		HRESULT hr = D3DXCreateTextureFromFile(pDxRenderDevice->GetDXDevive(), m_sResPath.c_str(), &m_pD3DTex);

		return hr == D3D_OK;
	}
}