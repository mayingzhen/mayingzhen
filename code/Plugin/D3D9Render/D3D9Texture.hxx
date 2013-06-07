#include "D3D9Render/D3D9Texture.h"

namespace ma
{
	D3D9Texture::D3D9Texture()
	{
		m_pD3DTex = NULL;
	}

	D3D9Texture::~D3D9Texture()
	{

	}

	bool D3D9Texture::Create(int nWidth,int nHeight,int nMipLevel,FORMAT Format,USAGE Usage)
	{
		if (nWidth == -1 && nHeight == -1)
		{
// 			const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();
// 			rWidth = dp->Width;
// 			rHeight = dp->Height;
			GetRenderDevice()->GetRenderWndSize(nWidth,nHeight);
		}

		ASSERT(nWidth && nHeight);
		//d_assert (FindTexture(sName).IsNull());

		//if (!RenderSystem::Instance()->CheckRenderTargetFormat(Format))
		//{
		//	EXCEPTION("Your device can't support texture format '" + D3D9Mapping::GetFormatString(Format) + 
		//		"' for render target");
		//}

		HRESULT hr = D3D_OK;
		DWORD D3DUsage = D3DUSAGE_RENDERTARGET;
		D3DPOOL D3DPool = D3DPOOL_DEFAULT;
		D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(Format);
		IDirect3DTexture9 * pD3D9Texture = NULL;
		
		D3D9RenderDevice* pDxRenderDevice = (D3D9RenderDevice*)GetRenderDevice();

		hr = GetD3D9DxDevive()->CreateTexture(
			nWidth, nHeight, 1,
			D3DUsage, D3DFormat, D3DPool,
			&pD3D9Texture, NULL);

// 		if (FAILED(hr))
// 		{
// 			EXCEPTION("D3D Error: CreateTexture failed, desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
// 		}

		//D3D9Texture * pTexture = new D3D9Texture(mD3D9Device);

		//pTexture->mName = sName;
		m_pD3DTex = pD3D9Texture;
		mWidth = nWidth;
		mHeight = nHeight;
		mDepth = 1;
		mUsage = USAGE_DYNAMIC;
		mFormat = Format;
		mType = TEXTYPE_RENDERTARGET;
		mMipLevels = nMipLevel;

		hr = pD3D9Texture->GetSurfaceLevel(0,&mRenderTarget);

		//mTextures.Insert(pTexture->GetName(), pTexture);

		//return TexturePtr(pTexture);

		return true;
	}

	bool D3D9Texture::Load(const char* pszPath,bool generateMipmaps)
	{
		if (pszPath)
		{
			m_sResPath = pszPath;
		}
	
		//D3DXCreateTextureFromFile(GetD3D9DxDevive(),pszPath,&m_pD3DTex);


		D3DXIMAGE_INFO ImgInfo;

		IDirect3DTexture9 * pD3D9Texture = NULL;
		HRESULT hr = D3DXCreateTextureFromFileEx(GetD3D9DxDevive(),
			pszPath,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			&ImgInfo,
			NULL,
			&pD3D9Texture);

		//D3DErrorExceptionFunction(D3DXCreateTextureFromFileEx, hr);

		D3DSURFACE_DESC desc;

		pD3D9Texture->GetLevelDesc(0, &desc);

		m_pD3DTex = pD3D9Texture;
		mWidth = desc.Width;
		mHeight = desc.Height;
		mDepth = 1;
		mFormat = D3D9Mapping::GetFormat(desc.Format);
		mMipLevels = pD3D9Texture->GetLevelCount();

		//mLoadState = Resource::LOADED;

		return true;
	}
}



