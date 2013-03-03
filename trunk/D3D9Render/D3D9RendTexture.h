#ifndef  _D3D9RendTexture__H__
#define  _D3D9RendTexture__H__

#include "Engine/RenderSystem/IRendTexture.h"

namespace ma
{
	class D3D9RENDER_API D3D9RendTexture : public IRendTexture
	{
	public:
		D3D9RendTexture();

		~D3D9RendTexture();

		virtual bool Load(const char* pszPath = NULL);

		IDirect3DTexture9* GetD3DTexture() {return m_pD3DTex;}
		
	private:
		IDirect3DTexture9* m_pD3DTex;
	};

}



#endif