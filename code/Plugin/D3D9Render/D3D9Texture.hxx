#include "D3D9Texture.h"
#include "D3D9Mapping.h"
#include "D3D9RenderDevice.h"

namespace ma
{
	D3D9Texture::D3D9Texture(const char* pszPath)
		:Texture(pszPath)
	{
		m_pD3DTex = NULL;
		m_pD3D9Surface = NULL;
	}

	D3D9Texture::D3D9Texture(int nWidth,int nHeight,FORMAT format,bool bDepthStencil)
		:Texture(nWidth,nHeight,format,bDepthStencil)
	{
		m_pD3DTex = NULL;
		m_pD3D9Surface = NULL; 
	}

	D3D9Texture::~D3D9Texture()
	{

	}

	bool D3D9Texture::CreateRT()
	{
		if (m_nWidth == -1 && m_nHeight == -1)
		{
			Platform::GetInstance().GetWindowSize(m_nWidth,m_nHeight);
		}

		ASSERT(m_nWidth && m_nHeight);

		HRESULT hr = D3D_OK;
		D3DPOOL D3DPool = D3DPOOL_DEFAULT;
		D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(m_eFormat);

		if ( m_eUsage == USAGE_RENDERTARGET )
		{
			DWORD D3DUsage = D3DUSAGE_RENDERTARGET;
			if (D3DFormat == D3DFMT_D24S8)
			{
				D3DUsage = D3DUSAGE_DEPTHSTENCIL;
			}

			hr = GetD3D9DxDevive()->CreateTexture(
				m_nWidth, 
				m_nHeight, 
				1,
				D3DUsage, 
				D3DFormat, 
				D3DPool,
				&m_pD3DTex, NULL);

			ASSERT(hr == D3D_OK);

			m_pD3DTex->GetSurfaceLevel(0,&m_pD3D9Surface);
		}
		else if(m_eUsage == USAGE_DEPTHSTENCIL)
		{
			hr = GetD3D9DxDevive()->CreateDepthStencilSurface(
				m_nWidth, 
				m_nHeight, 
				D3DFormat,
				D3DMULTISAMPLE_NONE, 
				0,
				TRUE,
				&m_pD3D9Surface, 
				NULL);

			ASSERT(hr == D3D_OK);
		}
		else
		{
			ASSERT(false);
		}


		return true;
	}

	bool D3D9Texture::LoadFromData(Uint8* data, UINT size, bool generateMipmaps )
	{	
		HRESULT hr = D3D_OK;
		hr = GetD3D9DxDevive()->CreateTexture(
			m_nWidth,
			m_nHeight,
			generateMipmaps ? 0 : 1,
			D3DUSAGE_DYNAMIC,
			D3D9Mapping::GetD3DFormat(m_eFormat),
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

		if (std::string(pDataStream->GetFilePath()) == "")
		{
			return LoadFromData(pDataStream->GetData(),pDataStream->GetSize());
		}

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



