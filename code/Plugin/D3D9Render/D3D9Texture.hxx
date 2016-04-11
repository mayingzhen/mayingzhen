#include "D3D9Texture.h"
#include "D3D9Mapping.h"
#include "D3D9RenderDevice.h"

namespace ma
{
	D3D9Texture::D3D9Texture()
		:Texture()
	{
		m_pD3DTex = NULL;
		m_pD3D9Surface = NULL;
		m_D3DPool = D3DPOOL_MANAGED;
	}

	D3D9Texture::D3D9Texture(int nWidth,int nHeight,PixelFormat format,TEXTURE_USAGE eUsage)
		:Texture(nWidth,nHeight,format,eUsage)
	{
		m_pD3DTex = NULL;
		m_pD3D9Surface = NULL; 
		
		if (eUsage == USAGE_DEPTHSTENCIL || eUsage == USAGE_RENDERTARGET)
		{
			m_D3DPool = D3DPOOL_DEFAULT;
		}
	}

	D3D9Texture::~D3D9Texture()
	{
		SAFE_RELEASE(m_pD3D9Surface);
		SAFE_RELEASE(m_pD3DTex);
	}

	bool D3D9Texture::GenerateMipmaps()
	{	
		DWORD eFilter = D3DX_DEFAULT;
		if (m_bSRGB)
		{
			eFilter = D3DX_FILTER_BOX | D3DX_FILTER_DITHER | D3DX_FILTER_SRGB;
		}

		if( D3DXFilterTexture( m_pD3DTex, NULL, D3DX_DEFAULT, eFilter ) != D3D_OK )
		{
			LogError("Failed to filter texture (generate mipmaps) CD3D9HardwarePixelBuffer::_genMipmaps" );
			return false;
		}

		return true;
	}

	bool D3D9Texture::RT_CreateTexture(bool bMinMap)
	{
		ASSERT(m_pD3DTex == NULL);
		if (m_pD3DTex)
			return false;

		D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(m_eFormat);
		DWORD D3DUsage = D3D9Mapping::GetD3DTextureUsage(m_eUsage);

		HRESULT hr = D3DXCheckTextureRequirements(GetD3D9DxDevive(),NULL,NULL,NULL,0,&D3DFormat,m_D3DPool);
		if (FAILED(hr))
		{
			LogError("Failed to D3DXCheckTextureRequirements, d3dPF:%d, mD3DPool:%d", D3DFormat, m_D3DPool);
			return false;
		}

		hr = GetD3D9DxDevive()->CreateTexture(
			m_nWidth,
			m_nHeight,
			bMinMap ? 0 : m_nMipLevels,
			D3DUsage,
			D3DFormat,
			m_D3DPool,
			&m_pD3DTex,
			NULL
			);

		if (FAILED(hr))
		{
			LogError("Error creating texture: %s, D3D9Texture::_createNormTex", DXGetErrorDescription(hr));
			return false;
		}

		m_pD3DTex->GetSurfaceLevel(0,&m_pD3D9Surface);
		m_nMipLevels = m_pD3DTex->GetLevelCount();

		return true;
	}

	bool D3D9Texture::RT_CreateDepthStencil()
	{
		D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(m_eFormat);

		HRESULT hr = GetD3D9DxDevive()->CreateDepthStencilSurface(
			m_nWidth,
			m_nHeight,
			D3DFormat,
			D3DMULTISAMPLE_NONE,
			0,
			true,
			&m_pD3D9Surface,
			NULL
			);

		if (FAILED(hr))
		{
			LogError("Error creating DepthStencil: %s, D3D9DepthStencil::RT_Create", DXGetErrorDescription(hr));
			return false;
		}

		return true;
	}

	bool D3D9Texture::SetLevelData(int nLevel, const PixelBox& src)
	{
		// for scoped deletion of conversion buffer
		RefPtr<MemoryStream> buf;
		PixelBox converted = src;
		
		int width = m_nWidth >> nLevel;
		int height = m_nHeight >> nLevel;

		Box dstBox(0,0,width,height);

		// convert to pixelbuffer's native format if necessary
		if (D3D9Mapping::GetD3DFormat(src.format) == D3DFMT_UNKNOWN)
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

		IDirect3DSurface9* pD3D9Surface = NULL;
		m_pD3DTex->GetSurfaceLevel(nLevel,&pD3D9Surface);

		if(D3DXLoadSurfaceFromMemory(pD3D9Surface, NULL, &destRect, 
			converted.data, D3D9Mapping::GetD3DFormat(converted.format),
			static_cast<UINT>(rowWidth),
			NULL, &srcRect, D3DX_DEFAULT, 0) != D3D_OK)
		{
			LogError("D3DXLoadSurfaceFromMemory failed CD3D9HardwarePixelBuffer::blitFromMemory");
			return false;
		}

		SAFE_RELEASE(pD3D9Surface);

		return true;
	}
}



