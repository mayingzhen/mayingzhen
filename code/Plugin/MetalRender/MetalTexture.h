#pragma once

#include "Engine/Material/Texture.h"


namespace ma
{
	class MetalTexture : public Texture
	{
	public:
		MetalTexture();

		MetalTexture(int nWidth,int nHeight,uint32 nMipMap,PixelFormat format,bool bTypeLess,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType);

		~MetalTexture();
		
		//IMetalShaderResourceView* GetShaderResourceView() {return m_pMetalShaderResourceView;}
		//IMetalShaderResourceView* GetShaderResourceViewSRGBNotEqual();

		//IMetalDepthStencilView* GetDepthStencilView();
		//void SetDepthStencilView(IMetalDepthStencilView* pView) {m_pDepthStencilView = pView;}

		//IMetalRenderTargetView* GetRenderTargetView(int level = 0, int array_index = 0, int face = 0);
		//void SetRenderTargetView(IMetalRenderTargetView* pView,int level = 0, int array_index = 0, int face = 0);
		
		id<MTLTexture>  GetNative() {return m_native;}
		//void SetTexture2D(IMetalTexture2D* pTex) {m_pMetalTex2D = pTex;}

		virtual void	CopyTo(Texture* pDesc,int nOutFace, int nOutLevel, int nInFace,int nInlevel);

		virtual bool	SaveToFile(const char* pszPath);

	private:
		virtual	bool			RT_CreateCubeTexture();	

		virtual	bool			RT_CreateTexture();			

		virtual	bool			SetLevelData(int level, int face,const PixelBox& src);

		virtual bool			GenerateMipmaps();
		
	public:
		id<MTLTexture>          m_native;

    
        MTLPixelFormat          m_descFormat;
    };


}


