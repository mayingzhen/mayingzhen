#ifndef  _D3D9RendTexture__H__
#define  _D3D9RendTexture__H__

#include "Engine/Material/Texture.h"

namespace ma
{
	class D3D9Texture : public Texture
	{
	public:
		D3D9Texture();

		D3D9Texture(int nWidth,int nHeight,PixelFormat format = PF_A8R8G8B8,TEXTURE_USAGE eUsage = USAGE_STATIC);

		~D3D9Texture();

		IDirect3DTexture9*		GetD3DTexture() {return m_pD3DTex;}

		IDirect3DSurface9*		GetD3DSurface() {return m_pD3D9Surface;}

		void					SetD3DSurface(IDirect3DSurface9* pD3D9Surface) {m_pD3D9Surface = pD3D9Surface;}

	private:
		virtual	bool			RT_CreateTexture(bool bMinMap);		

		virtual	bool			RT_CreateDepthStencil();		

		virtual	bool			SetLevelData(int level, const PixelBox& src);

		virtual bool			GenerateMipmaps();
		
	private:
		IDirect3DTexture9*			m_pD3DTex;

		IDirect3DSurface9*			m_pD3D9Surface;

		D3DPOOL						m_D3DPool;
	};

}



#endif