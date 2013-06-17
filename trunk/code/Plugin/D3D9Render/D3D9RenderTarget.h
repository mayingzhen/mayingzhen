#ifndef  _D3D9RenderTarget__H__
#define  _D3D9RenderTarget__H__



namespace ma
{
	class D3D9RENDER_API D3D9RenderTarget : public RenderTarget
	{
	public:
		D3D9RenderTarget();

		~D3D9RenderTarget();

		virtual bool Create(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8);


		//IDirect3DTexture9* GetD3DTexture() {return m_pD3DTex;}

		IDirect3DSurface9* GetD3DSurface() {return mRenderTarget;}

		void				SetD3DSurface(IDirect3DSurface9* pSurface) {mRenderTarget =  pSurface;}

	private:
		//IDirect3DTexture9*			m_pD3DTex;
		IDirect3DSurface9*			mRenderTarget;
	};

}



#endif