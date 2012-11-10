#include "DXRender/DxRendTexture.h"

namespace ma
{
	bool DxRendTexture::Load(const char* resPath)
	{
		DxRender* pRender = (DxRender*)GetRender();
		HRESULT hr = D3DXCreateTextureFromFile(pRender->GetDXDevive(), resPath, &m_pD3DTex);

		return hr == D3D_OK;
	}
}