#include "D3D9Texture.h"
#include "D3D9Mapping.h"
#include "D3D9RenderDevice.h"

namespace ma
{
	D3D9Texture::D3D9Texture()
	{
		m_pD3DTex = NULL;
	}

	D3D9Texture::~D3D9Texture()
	{

	}

	bool D3D9Texture::CreateRT(int nWidth,int nHeight,FORMAT Format)
	{
		if (nWidth == -1 && nHeight == -1)
		{
			Platform::GetInstance().GetWindowSize(nWidth,nHeight);
		}

		ASSERT(nWidth && nHeight);

		HRESULT hr = D3D_OK;
		DWORD D3DUsage = D3DUSAGE_RENDERTARGET;
		D3DPOOL D3DPool = D3DPOOL_DEFAULT;
		D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(Format);
		IDirect3DTexture9 * pD3D9Texture = NULL;

		if (D3DFormat == D3DFMT_D24S8)
		{
			D3DUsage = D3DUSAGE_DEPTHSTENCIL;
		}
		
		D3D9RenderDevice* pDxRenderDevice = (D3D9RenderDevice*)GetRenderDevice();

		hr = GetD3D9DxDevive()->CreateTexture(
			nWidth, 
			nHeight, 
			1,
			D3DUsage, 
			D3DFormat, 
			D3DPool,
			&pD3D9Texture, NULL);

		ASSERT(hr == D3D_OK);

		//pTexture->mName = sName;
		m_pD3DTex = pD3D9Texture;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_eUsage = USAGE_DYNAMIC;
		m_eFormat = Format;
		m_eType = TEXTYPE_RENDERTARGET;
		m_nMipLevels = 1;

		return true;
	}

	bool D3D9Texture::LoadFromData(FORMAT format,UINT width,UINT height,Uint8* data, UINT size, bool generateMipmaps )
	{	
		m_nWidth = width;
		m_nHeight = height;
		m_eFormat = format;

		HRESULT hr = D3D_OK;
		hr = GetD3D9DxDevive()->CreateTexture(
			m_nWidth,
			m_nHeight,
			generateMipmaps ? 0 : 1,
			D3DUSAGE_DYNAMIC,
			D3D9Mapping::GetD3DFormat(format),
			D3DPOOL_DEFAULT,
			&m_pD3DTex,
			NULL
			);
		ASSERT(hr == D3D_OK);
		
		D3DLOCKED_RECT lock;
		hr = m_pD3DTex->LockRect(0,&lock,NULL,0);
		memcpy(lock.pBits,data,size);
		hr = m_pD3DTex->UnlockRect(0);

		return true;
	}

	bool D3D9Texture::Load(DataStream* pDataStream, bool generateMipmaps)
	{
		ASSERT(pDataStream);
		if (pDataStream == NULL)
			return false;

		D3DXIMAGE_INFO ImgInfo;

		IDirect3DTexture9 * pD3D9Texture = NULL;
		HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(GetD3D9DxDevive(),
			pDataStream->GetData(),
			pDataStream->GetSize(),
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			generateMipmaps ? D3DX_DEFAULT : D3DX_FROM_FILE,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			&ImgInfo,
			NULL,
			&pD3D9Texture);
		ASSERT(hr == D3D_OK);


		D3DSURFACE_DESC desc;

		pD3D9Texture->GetLevelDesc(0, &desc);

		m_pD3DTex = pD3D9Texture;
		m_nWidth = desc.Width;
		m_nHeight = desc.Height;
		//m_eFormat = D3D9Mapping::GetFormat(desc.Format);
		m_nMipLevels = pD3D9Texture->GetLevelCount();

		//mLoadState = Resource::LOADED;

		return true;
	}

	bool D3D9Texture::Load(const char* pszPath,bool generateMipmaps)
	{
		ASSERT(pszPath);
		if (pszPath == NULL)
			return false;

		SAFE_DELETE(m_pDataStream);
		m_pDataStream =  FileSystem::readAll(pszPath);

		return Load(m_pDataStream,generateMipmaps);
	}
}



