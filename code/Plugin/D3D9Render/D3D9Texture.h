#ifndef  _D3D9RendTexture__H__
#define  _D3D9RendTexture__H__



namespace ma
{
	class D3D9RENDER_API D3D9Texture : public Texture
	{
	public:
		D3D9Texture(const char* pszPath);

		D3D9Texture(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8,bool bDepthStencil = false);

		~D3D9Texture();

		virtual bool			CreateRT();

		virtual bool			Load(const char* pszPath = NULL,bool generateMipmaps = false);

		virtual bool			Load(DataStream* pDataStream, bool generateMipmaps = false);

		virtual bool			LoadFromData(Uint8* data,UINT size, bool generateMipmaps = false);

		IDirect3DTexture9*		GetD3DTexture() {return m_pD3DTex;}

		IDirect3DSurface9*		GetD3DSurface() {return m_pD3D9Surface;}

		void					SetD3DSurface(IDirect3DSurface9* pD3D9Surface) {m_pD3D9Surface = pD3D9Surface;}
		
	private:
		IDirect3DTexture9*			m_pD3DTex;

		IDirect3DSurface9*			m_pD3D9Surface;
	};

}



#endif