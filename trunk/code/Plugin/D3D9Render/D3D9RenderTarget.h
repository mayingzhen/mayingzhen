#ifndef  _D3D9RenderTarget__H__
#define  _D3D9RenderTarget__H__



namespace ma
{
	class  D3D9RenderTarget : public RenderTarget
	{
	public:
		D3D9RenderTarget();

		~D3D9RenderTarget();

		virtual Texture*	GetTexture();

		void				Create(int nWidth = -1,int nHeight = -1,FORMAT format = FMT_A8R8G8B8);

		IDirect3DSurface9*	GetD3DSurface() {return m_pD3D9Surface;}

		void				SetD3DSurface(IDirect3DSurface9* pD3D9Surface) {m_pD3D9Surface = pD3D9Surface;}

	private:
		D3D9Texture*			m_pTexture;	

		IDirect3DSurface9*		m_pD3D9Surface;
	};

}



#endif