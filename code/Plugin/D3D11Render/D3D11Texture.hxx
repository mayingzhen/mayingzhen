#include "D3D11Texture.h"
#include "D3D11Mapping.h"
#include "D3D11RenderDevice.h"

namespace ma
{
	D3D11Texture::D3D11Texture()
		:Texture()
	{
		m_pD3D11Tex2D = NULL;
		m_pD3D11ShaderResourceView = NULL;
		m_pD3D11ShaderResourceViewSRGBNotEqual = NULL;
		m_pDepthStencilView = NULL;
		memset(m_pRenderTargetView,0,sizeof(m_pRenderTargetView));
	}

	D3D11Texture::D3D11Texture(int nWidth,int nHeight,uint32 nMipMap,PixelFormat format,bool bTypeLess,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType)
		:Texture(nWidth,nHeight,nMipMap,format,bTypeLess,bSRGB,eUsage,eType)
	{
		m_pD3D11Tex2D = NULL;
		m_pD3D11ShaderResourceView = NULL;
		m_pD3D11ShaderResourceViewSRGBNotEqual = NULL;
		m_pDepthStencilView = NULL;
		memset(m_pRenderTargetView,0,sizeof(m_pRenderTargetView));
	}

	D3D11Texture::~D3D11Texture()
	{
		SAFE_RELEASE(m_pD3D11Tex2D);
		SAFE_RELEASE(m_pD3D11ShaderResourceView);
		SAFE_RELEASE(m_pD3D11ShaderResourceViewSRGBNotEqual);
		SAFE_RELEASE(m_pDepthStencilView);
		for (uint32 i = 0; i < MAX_MIP; ++i)
		{
			for (uint32 j = 0; j < 6; ++j)
			{
				SAFE_RELEASE(m_pRenderTargetView[i][j]);
			}
		}
	}

	bool D3D11Texture::GenerateMipmaps()
	{	
		return true;
	}

	DXGI_FORMAT GetSRVFormat(DXGI_FORMAT format)
	{
		if (format == DXGI_FORMAT_R24G8_TYPELESS)
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		else if (format == DXGI_FORMAT_R16_TYPELESS)
			return DXGI_FORMAT_R16_UNORM;
		else if (format == DXGI_FORMAT_R32_TYPELESS)
			return DXGI_FORMAT_R32_FLOAT;
		else if (format == DXGI_FORMAT_R8G8B8A8_TYPELESS)
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		else
			return format;
	}

	DXGI_FORMAT GetDSVFormat(DXGI_FORMAT format)
	{
		if (format == DXGI_FORMAT_R24G8_TYPELESS)
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		else if (format == DXGI_FORMAT_R16_TYPELESS)
			return DXGI_FORMAT_D16_UNORM;
		else if (format == DXGI_FORMAT_R32_TYPELESS)
			return DXGI_FORMAT_D32_FLOAT;
		else
			return format;
	}

	DXGI_FORMAT GetSRGBFormat(DXGI_FORMAT format)
	{
		if (format == DXGI_FORMAT_R8G8B8A8_UNORM)
			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		else if (format == DXGI_FORMAT_BC1_UNORM)
			return DXGI_FORMAT_BC1_UNORM_SRGB;
		else if (format == DXGI_FORMAT_BC2_UNORM)
			return DXGI_FORMAT_BC2_UNORM_SRGB;
		else if (format == DXGI_FORMAT_BC3_UNORM)
			return DXGI_FORMAT_BC3_UNORM_SRGB;
		else
			return format;
	}

	DXGI_FORMAT GetTypelessFormat(DXGI_FORMAT format) {
		switch(format) {
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_SNORM:
		case DXGI_FORMAT_R8G8B8A8_SINT:
			return DXGI_FORMAT_R8G8B8A8_TYPELESS;

		case DXGI_FORMAT_BC1_UNORM_SRGB:
		case DXGI_FORMAT_BC1_UNORM:
			return DXGI_FORMAT_BC1_TYPELESS;
		case DXGI_FORMAT_BC2_UNORM_SRGB:
		case DXGI_FORMAT_BC2_UNORM:
			return DXGI_FORMAT_BC2_TYPELESS;
		case DXGI_FORMAT_BC3_UNORM_SRGB:
		case DXGI_FORMAT_BC3_UNORM:
			return DXGI_FORMAT_BC3_TYPELESS;

		// Depth
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			return DXGI_FORMAT_R24G8_TYPELESS;
		};
		
		ASSERT(false);
		return format;
	}

	bool IsCompressed(DXGI_FORMAT format)
	{
		return format == DXGI_FORMAT_BC1_UNORM || format == DXGI_FORMAT_BC2_UNORM || format == DXGI_FORMAT_BC3_UNORM;
	}

	bool D3D11Texture::RT_CreateCubeTexture()
	{
		m_eFormat = D3D11Mapping::_getClosestSupportedPF(m_eFormat);
		m_descFormat = D3D11Mapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			m_descFormat = GetSRGBFormat(m_descFormat);
		}
		if (m_bTypeLess)
		{
			m_descFormat = GetTypelessFormat(m_descFormat);
		}

		D3D11_TEXTURE2D_DESC textureDesc;
		memset(&textureDesc, 0, sizeof textureDesc);
		textureDesc.Width = (UINT)m_nWidth;
		textureDesc.Height = (UINT)m_nHeight;
		textureDesc.MipLevels = m_nMipLevels;
		textureDesc.ArraySize = 6;
		textureDesc.Format = m_descFormat;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = m_eUsage == USAGE_DYNAMIC ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (m_eUsage == USAGE_RENDERTARGET)
			textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		else if (m_eUsage == USAGE_DEPTHSTENCIL)
			textureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		textureDesc.CPUAccessFlags = m_eUsage == USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

		GetD3D11DxDevive()->CreateTexture2D(&textureDesc, 0, (ID3D11Texture2D**)&m_pD3D11Tex2D);
		ASSERT(m_pD3D11Tex2D);
		if (m_pD3D11Tex2D == NULL)
		{
			LogError("Failed to create DepthStencile");
			return false;
		}

		DXGI_FORMAT srvFormat = D3D11Mapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			srvFormat = GetSRGBFormat(srvFormat);
		}
		if (m_bTypeLess)
		{
			srvFormat = GetSRVFormat(m_descFormat);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
		memset(&resourceViewDesc, 0, sizeof resourceViewDesc);
		resourceViewDesc.Format = srvFormat;
		resourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
		resourceViewDesc.TextureCube.MipLevels = m_nMipLevels;

		GetD3D11DxDevive()->CreateShaderResourceView(m_pD3D11Tex2D, &resourceViewDesc,&m_pD3D11ShaderResourceView);
		if (!m_pD3D11ShaderResourceView)
		{
			LogError("Failed to create shader resource view for texture");
			return false;
		}

		return true;
	}

	bool D3D11Texture::RT_CreateTexture()
	{
		m_eFormat = D3D11Mapping::_getClosestSupportedPF(m_eFormat);
		m_descFormat = D3D11Mapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			m_descFormat = GetSRGBFormat(m_descFormat);
		}
		if (m_bTypeLess)
		{
			m_descFormat = GetTypelessFormat(m_descFormat);
		}

		D3D11_TEXTURE2D_DESC textureDesc;
		memset(&textureDesc, 0, sizeof textureDesc);
		textureDesc.Width = (UINT)m_nWidth;
		textureDesc.Height = (UINT)m_nHeight;
		textureDesc.MipLevels = m_nMipLevels;
		textureDesc.ArraySize = 1;
		textureDesc.Format = m_descFormat;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		//if (m_eUsage == USAGE_STATIC)
		//{
		//	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		//}
		//else 
		if (m_eUsage == USAGE_DYNAMIC)
		{
			textureDesc.Usage = D3D11_USAGE_DYNAMIC;
		}
		else 
		{
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
		}
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (m_eUsage == USAGE_RENDERTARGET)
			textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		else if (m_eUsage == USAGE_DEPTHSTENCIL)
			textureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		textureDesc.CPUAccessFlags = m_eUsage == USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;

		GetD3D11DxDevive()->CreateTexture2D(&textureDesc, 0, (ID3D11Texture2D**)&m_pD3D11Tex2D);
		ASSERT(m_pD3D11Tex2D);
		if (m_pD3D11Tex2D == NULL)
		{
			LogError("Failed to create DepthStencile");
			return false;
		}

		DXGI_FORMAT srvFormat = D3D11Mapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			srvFormat = GetSRGBFormat(srvFormat);
		}
		if (m_bTypeLess)
		{
			srvFormat = GetSRVFormat(m_descFormat);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
		memset(&resourceViewDesc, 0, sizeof resourceViewDesc);
		resourceViewDesc.Format = srvFormat;
		resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MipLevels = m_nMipLevels;

		GetD3D11DxDevive()->CreateShaderResourceView(m_pD3D11Tex2D, &resourceViewDesc,&m_pD3D11ShaderResourceView);
		if (!m_pD3D11ShaderResourceView)
		{
			LogError("Failed to create shader resource view for texture");
			return false;
		}

		return true;
	}

	ID3D11ShaderResourceView* D3D11Texture::GetShaderResourceViewSRGBNotEqual()
	{
		if (m_pD3D11ShaderResourceViewSRGBNotEqual)
			return m_pD3D11ShaderResourceViewSRGBNotEqual;

		ASSERT(m_bSRGB);

		DXGI_FORMAT srvFormat = D3D11Mapping::_getPF(m_eFormat);

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
		memset(&resourceViewDesc, 0, sizeof resourceViewDesc);
		resourceViewDesc.Format = srvFormat;
		resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MipLevels = m_nMipLevels;

		GetD3D11DxDevive()->CreateShaderResourceView(m_pD3D11Tex2D, &resourceViewDesc,&m_pD3D11ShaderResourceViewSRGBNotEqual);
		if (!m_pD3D11ShaderResourceView)
		{
			LogError("Failed to create shader resource view for texture");
			return NULL;
		}

		return m_pD3D11ShaderResourceViewSRGBNotEqual;
	}

	ID3D11DepthStencilView* D3D11Texture::GetDepthStencilView()
	{
		if (m_pDepthStencilView)
			return m_pDepthStencilView;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		memset(&depthStencilViewDesc, 0, sizeof depthStencilViewDesc);
		depthStencilViewDesc.Format = GetDSVFormat(m_descFormat);
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		GetD3D11DxDevive()->CreateDepthStencilView(m_pD3D11Tex2D,&depthStencilViewDesc,&m_pDepthStencilView);
		if (!m_pDepthStencilView)
		{
			LogError("Failed to create depth-stencil view for texture");
			return NULL;
		}

		return m_pDepthStencilView;
	}

	ID3D11RenderTargetView* D3D11Texture::GetRenderTargetView(int level, int array_index, int face)
	{
		ASSERT(array_index == 0 && level < MAX_MIP);
		if (array_index != 0 || level >= MAX_MIP)
			return NULL;

		if (m_pRenderTargetView[level][face])
			return m_pRenderTargetView[level][face];

		m_eFormat = D3D11Mapping::_getClosestSupportedPF(m_eFormat);
		m_descFormat = D3D11Mapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			m_descFormat = GetSRGBFormat(m_descFormat);
		}

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		memset(&renderTargetViewDesc, 0, sizeof renderTargetViewDesc);
		renderTargetViewDesc.Format = m_descFormat;
		if (m_eType == TEXTYPE_2D)
		{
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		}
		else if (m_eType == TEXTYPE_CUBE)
		{
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			renderTargetViewDesc.Texture2DArray.MipSlice = level;
			renderTargetViewDesc.Texture2DArray.FirstArraySlice = array_index * 6 + face;
			renderTargetViewDesc.Texture2DArray.ArraySize = 1;
		}

		ID3D11RenderTargetView*	pRenderTargetView = NULL;
		GetD3D11DxDevive()->CreateRenderTargetView((ID3D11Resource*)m_pD3D11Tex2D, &renderTargetViewDesc,&pRenderTargetView);
		if (!pRenderTargetView)
		{
			LogError("Failed to create renderTarget view for texture");
			return NULL;
		}
	
		m_pRenderTargetView[level][face] = pRenderTargetView;

		return pRenderTargetView;
	}

	void D3D11Texture::SetRenderTargetView(ID3D11RenderTargetView* pView,int level, int array_index, int face)
	{
		ASSERT(array_index == 0 && level < MAX_MIP);
		if (array_index != 0 || level >= MAX_MIP)
			return;

		m_pRenderTargetView[level][face] = pView;
	}


	D3D11_BOX OgreImageBoxToDx11Box(const Box &inBox)
	{
		D3D11_BOX res;
		res.left	= static_cast<UINT>(inBox.left);
		res.top		= static_cast<UINT>(inBox.top);
		res.front	= static_cast<UINT>(inBox.front);
		res.right	= static_cast<UINT>(inBox.right);
		res.bottom	= static_cast<UINT>(inBox.bottom);
		res.back	= static_cast<UINT>(inBox.back);

		return res;
	}

	bool D3D11Texture::SetLevelData(int nLevel, int nFace, const PixelBox& src)
	{
		// for scoped deletion of conversion buffer
		RefPtr<MemoryStream> buf;
		PixelBox converted = src;
		
		int width = m_nWidth >> nLevel;
		int height = m_nHeight >> nLevel;

		if (PixelUtil::isCompressed(converted.format))
		{
			width += 3;
			width &= 0xfffffffc;
			height += 3;
			height &= 0xfffffffc;
		}

		Box dstBox(0,0,width,height);

		D3D11_BOX dstBoxDx11 = OgreImageBoxToDx11Box(dstBox);
		dstBoxDx11.front = 0;
		dstBoxDx11.back = converted.getDepth();

		// convert to pixelbuffer's native format if necessary
		if (D3D11Mapping::_getPF(src.format) == DXGI_FORMAT_UNKNOWN)
		{
			buf = CreateMemoryStream(PixelUtil::getMemorySize(src.getWidth(), src.getHeight(), src.getDepth(),m_eFormat), false);
			converted = PixelBox(src.getWidth(), src.getHeight(), src.getDepth(), m_eFormat, buf->GetPtr());
			PixelUtil::bulkPixelConversion(src, converted);
		}

		uint32 rowWidth;
		if (PixelUtil::isCompressed(converted.format))
		{
			// D3D wants the width of one row of cells in bytes
			if (converted.format == PF_DXT1)
			{
				// 64 bits (8 bytes) per 4x4 block
				rowWidth = (converted.rowPitch / 4) * 8;
			}
			else
			{
				// 128 bits (16 bytes) per 4x4 block
				rowWidth = (converted.rowPitch / 4) * 16;
			}
		}
		else
		{
			rowWidth = converted.rowPitch * PixelUtil::getNumElemBytes(converted.format);
		}

		unsigned subResource = D3D11CalcSubresource(nLevel, nFace, m_nMipLevels);

		GetD3D11DxDeviveContext()->UpdateSubresource( 
			m_pD3D11Tex2D, 
			subResource,
			&dstBoxDx11,
			converted.data,
			rowWidth,
			0 );
		
		return true;
	}

	void D3D11Texture::CopyTo(Texture* pDes,int nOutFace, int nOutLevel, int nInFace,int nInlevel)
	{
		D3D11Texture* pDestD3D11 = (D3D11Texture*)(pDes);

		D3D11_BOX src_box;
		src_box.left = 0;
		src_box.top = 0;
		src_box.front = 0;
		src_box.right = GetWidth();
		src_box.bottom = GetHeight();
		src_box.back = 1;

		GetD3D11DxDeviveContext()->CopySubresourceRegion(
			pDestD3D11->GetTexture2D(), D3D11CalcSubresource(nOutLevel, nOutFace, pDestD3D11->GetMipMapNumber()), 0, 0, 0, 
			this->GetTexture2D(), D3D11CalcSubresource(nInlevel,nInFace, this->GetMipMapNumber()), &src_box);
	}

	bool D3D11Texture::SaveToFile(const char* pszPath)
	{
		HRESULT hr = D3DX11SaveTextureToFile(GetD3D11DxDeviveContext(),GetTexture2D(),D3DX11_IFF_DDS,pszPath);
		return hr == S_OK;
	}
}



