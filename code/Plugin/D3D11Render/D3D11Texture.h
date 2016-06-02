#pragma once

#include "Engine/Material/Texture.h"

namespace ma
{
	class D3D11Texture : public Texture
	{
	public:
		D3D11Texture();

		D3D11Texture(int nWidth,int nHeight,uint32 nMipMap,PixelFormat format,bool bTypeLess,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType);

		~D3D11Texture();
		
		ID3D11ShaderResourceView* GetShaderResourceView() {return m_pD3D11ShaderResourceView;}
		ID3D11ShaderResourceView* GetShaderResourceViewSRGBNotEqual();

		ID3D11DepthStencilView* GetDepthStencilView();
		void SetDepthStencilView(ID3D11DepthStencilView* pView) {m_pDepthStencilView = pView;}	

		ID3D11RenderTargetView* GetRenderTargetView(int level = 0, int array_index = 0, int face = 0); 
		void SetRenderTargetView(ID3D11RenderTargetView* pView,int level = 0, int array_index = 0, int face = 0);
		
		ID3D11Texture2D* GetTexture2D() {return m_pD3D11Tex2D;}
		void SetTexture2D(ID3D11Texture2D* pTex) {m_pD3D11Tex2D = pTex;}

		virtual void	CopyTo(Texture* pDesc,int nFace,int level);

		virtual bool	SaveToFile(const char* pszPath);

	private:
		virtual	bool			RT_CreateCubeTexture();	

		virtual	bool			RT_CreateTexture();			

		virtual	bool			SetLevelData(int level, int face,const PixelBox& src);

		virtual bool			GenerateMipmaps();
		
	private:
		ID3D11Texture2D*			m_pD3D11Tex2D;

		ID3D11ShaderResourceView*	m_pD3D11ShaderResourceView;	
		ID3D11ShaderResourceView*	m_pD3D11ShaderResourceViewSRGBNotEqual;

		ID3D11DepthStencilView*		m_pDepthStencilView;

		enum {MAX_MIP = 10};
		ID3D11RenderTargetView*		m_pRenderTargetView[MAX_MIP][6];

		DXGI_FORMAT					m_descFormat;
	};


}


