#ifndef  _D3D9RendTexture__H__
#define  _D3D9RendTexture__H__



namespace ma
{
	class D3D9RENDER_API D3D9Texture : public Texture
	{
	public:
		D3D9Texture();

		~D3D9Texture();

		virtual bool Create(int nWidth,int nHeight,int nMipLevel = 1,FORMAT Format = FMT_X8R8G8B8,USAGE Usage = USAGE_STATIC);

		virtual bool Load(const char* pszPath = NULL,bool generateMipmaps = false);

		IDirect3DTexture9* GetD3DTexture() {return m_pD3DTex;}

		IDirect3DSurface9* GetD3DSurface() {return mRenderTarget;}
		
	private:
		IDirect3DTexture9*			m_pD3DTex;
		IDirect3DSurface9*			mRenderTarget;
		//IDirect3DDevice9 *          mD3D9Device;
	};

}



#endif