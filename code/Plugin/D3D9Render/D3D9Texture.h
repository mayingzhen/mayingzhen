#ifndef  _D3D9RendTexture__H__
#define  _D3D9RendTexture__H__



namespace ma
{
	class D3D9RENDER_API D3D9Texture : public Texture
	{
	public:
		D3D9Texture();

		~D3D9Texture();

		virtual bool CreateRT(int nWidth,int nHeight,FORMAT Format = FMT_A8R8G8B8);

		virtual bool Load(const char* pszPath = NULL,bool generateMipmaps = false);

		virtual bool Load(DataStream* pDataStream, bool generateMipmaps = false);

		virtual bool LoadFromData(FORMAT format,UINT width,UINT height,Uint8* data,UINT size, bool generateMipmaps = false);

		IDirect3DTexture9* GetD3DTexture() {return m_pD3DTex;}

		//IDirect3DSurface9* GetD3DSurface() {return mRenderTarget;}
		
	private:
		IDirect3DTexture9*			m_pD3DTex;
		//IDirect3DSurface9*			mRenderTarget;
		//IDirect3DDevice9 *          mD3D9Device;
	};

}



#endif