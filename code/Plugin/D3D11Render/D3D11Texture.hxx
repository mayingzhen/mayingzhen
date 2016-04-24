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
		m_pDepthStencilView = NULL;
		m_pRenderTargetView = NULL;
	}

	D3D11Texture::D3D11Texture(int nWidth,int nHeight,PixelFormat format,TEXTURE_USAGE eUsage)
		:Texture(nWidth,nHeight,format,eUsage)
	{
		m_pD3D11Tex2D = NULL;
		m_pD3D11ShaderResourceView = NULL;
		m_pDepthStencilView = NULL;
		m_pRenderTargetView = NULL;
		
// 		if (eUsage == USAGE_DEPTHSTENCIL || eUsage == USAGE_RENDERTARGET)
// 		{
// 			m_D3DPool = D3DPOOL_DEFAULT;
// 		}
	}

	D3D11Texture::~D3D11Texture()
	{
		SAFE_RELEASE(m_pD3D11Tex2D);
		SAFE_RELEASE(m_pD3D11ShaderResourceView);
		SAFE_RELEASE(m_pDepthStencilView);
		SAFE_RELEASE(m_pRenderTargetView);
	}

	bool D3D11Texture::GenerateMipmaps()
	{	
// 		DWORD eFilter = D3DX_DEFAULT;
// 		if (m_bSRGB)
// 		{
// 			eFilter = D3DX_FILTER_BOX | D3DX_FILTER_DITHER | D3DX_FILTER_SRGB;
// 		}
// 
// 		if( D3DXFilterTexture( m_pD3DTex, NULL, D3DX_DEFAULT, eFilter ) != D3D_OK )
// 		{
// 			LogError("Failed to filter texture (generate mipmaps) CD3D11HardwarePixelBuffer::_genMipmaps" );
// 			return false;
// 		}

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

	unsigned GetRowDataSize(DXGI_FORMAT format,int width) 
	{
		switch (format)
		{
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_A8_UNORM:
			return (unsigned)width;

		case DXGI_FORMAT_R8G8_UNORM:
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_R16_TYPELESS:
			return (unsigned)(width * 2);

		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R16G16_UNORM:
		case DXGI_FORMAT_R16G16_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_R32_TYPELESS:
			return (unsigned)(width * 4);

		case DXGI_FORMAT_R16G16B16A16_UNORM:
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return (unsigned)(width * 8);

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return (unsigned)(width * 16);

		case DXGI_FORMAT_BC1_UNORM:
		case DXGI_FORMAT_BC1_UNORM_SRGB:
			return (unsigned)(((width + 3) >> 2) * 8);

		case DXGI_FORMAT_BC2_UNORM:
		case DXGI_FORMAT_BC2_UNORM_SRGB:
		case DXGI_FORMAT_BC3_UNORM:
		case DXGI_FORMAT_BC3_UNORM_SRGB:
			return (unsigned)(((width + 3) >> 2) * 16);

		default:
			ASSERT(false);
			return 0;
		}
	}

	bool IsCompressed(DXGI_FORMAT format)
	{
		return format == DXGI_FORMAT_BC1_UNORM || format == DXGI_FORMAT_BC2_UNORM || format == DXGI_FORMAT_BC3_UNORM;
	}


	bool D3D11Texture::RT_CreateTexture()
	{
		m_eFormat = D3D11Mapping::_getClosestSupportedPF(m_eFormat);
		m_descFormat = D3D11Mapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			m_descFormat = GetSRGBFormat(m_descFormat);
		}

		//DWORD D3DUsage = D3D11Mapping::GetD3DTextureUsage(m_eUsage);

		D3D11_TEXTURE2D_DESC textureDesc;
		memset(&textureDesc, 0, sizeof textureDesc);
		textureDesc.Width = (UINT)m_nWidth;
		textureDesc.Height = (UINT)m_nHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
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

		GetD3D11DxDevive()->CreateTexture2D(&textureDesc, 0, (ID3D11Texture2D**)&m_pD3D11Tex2D);
		ASSERT(m_pD3D11Tex2D);
		if (m_pD3D11Tex2D == NULL)
		{
			LogError("Failed to create DepthStencile");
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
		memset(&resourceViewDesc, 0, sizeof resourceViewDesc);
		resourceViewDesc.Format = (DXGI_FORMAT)GetSRVFormat(textureDesc.Format);
		resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MipLevels = (UINT)1/*levels_*/;

		GetD3D11DxDevive()->CreateShaderResourceView(m_pD3D11Tex2D, &resourceViewDesc,&m_pD3D11ShaderResourceView);
		if (!m_pD3D11ShaderResourceView)
		{
			LogError("Failed to create shader resource view for texture");
			return false;
		}

		if (m_eUsage == USAGE_RENDERTARGET)
		{
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			memset(&renderTargetViewDesc, 0, sizeof renderTargetViewDesc);
			renderTargetViewDesc.Format = textureDesc.Format;
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

			GetD3D11DxDevive()->CreateRenderTargetView((ID3D11Resource*)m_pD3D11Tex2D, &renderTargetViewDesc,&m_pRenderTargetView);
			if (!m_pRenderTargetView)
			{
				LogError("Failed to create renderTarget view for texture");
				return false;
			}
		}

		return true;
	}


	bool D3D11Texture::RT_CreateDepthStencil()
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		memset(&depthStencilViewDesc, 0, sizeof depthStencilViewDesc);
		depthStencilViewDesc.Format = GetDSVFormat(m_descFormat);
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		GetD3D11DxDevive()->CreateDepthStencilView(m_pD3D11Tex2D,&depthStencilViewDesc,&m_pDepthStencilView);
		if (!m_pDepthStencilView)
		{
			LogError("Failed to create depth-stencil view for texture");
			return false;
		}

		return true;
	}

	bool D3D11Texture::SetLevelData(int nLevel, const PixelBox& src)
	{
		// for scoped deletion of conversion buffer
		RefPtr<MemoryStream> buf;
		PixelBox converted = src;
		
		int width = m_nWidth >> nLevel;
		int height = m_nHeight >> nLevel;

		Box dstBox(0,0,width,height);

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

		RECT destRect, srcRect;
		srcRect = toD3DRECT(converted);
		destRect = toD3DRECT(dstBox);

		unsigned char* srcData = (unsigned char*)converted.data;
		unsigned rowSize = GetRowDataSize(m_descFormat,width);
		unsigned rowStart = GetRowDataSize(m_descFormat,0);
		unsigned subResource = D3D11CalcSubresource(nLevel, 0, nLevel);

		if (m_eUsage == USAGE_DYNAMIC)
		{
			if (IsCompressed(m_descFormat))
			{
				height = (height + 3) >> 2;
			}

			D3D11_MAPPED_SUBRESOURCE mappedData;
			mappedData.pData = 0;

			GetD3D11DxDeviveContext()->Map(m_pD3D11Tex2D, subResource, D3D11_MAP_WRITE_DISCARD, 0,
				&mappedData);
			if (mappedData.pData)
			{
				for (int row = 0; row < height; ++row)
				{
					//memcpy((unsigned char*)mappedData.pData + row * mappedData.RowPitch + rowStart, src + row * rowSize, rowSize);
				}
				GetD3D11DxDeviveContext()->Unmap(m_pD3D11Tex2D, subResource);
			}
			else
			{
				LogError("Failed to map texture for update");
				return false;
			}
		}
		else
		{
			D3D11_BOX destBox;
			destBox.left = 0;
			destBox.right = (UINT)(width);
			destBox.top = 0;
			destBox.bottom = (UINT)(height);
			destBox.front = 0;
			destBox.back = 1;

			GetD3D11DxDeviveContext()->UpdateSubresource(m_pD3D11Tex2D, subResource, &destBox, srcData, rowSize, 0);
		}

		return true;
	}
}



