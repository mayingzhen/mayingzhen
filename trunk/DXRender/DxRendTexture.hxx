#include "DXRender/DxRendTexture.h"

namespace ma
{
	bool DxRendTexture::Load(const char* resPath)
	{
		DxRenderDevice* pDxRenderDevice = (DxRenderDevice*)GetRenderDevice();
		HRESULT hr = D3DXCreateTextureFromFile(pDxRenderDevice->GetDXDevive(), resPath, &m_pD3DTex);

		return hr == D3D_OK;
	}
}