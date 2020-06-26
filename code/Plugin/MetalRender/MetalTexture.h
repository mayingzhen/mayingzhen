#pragma once

#import <Metal/Metal.h>

#include "Engine/Material/Texture.h"


namespace ma
{
	class MetalTexture : public Texture
	{
	public:
		MetalTexture();

		MetalTexture(int nWidth,int nHeight,uint32_t nMipMap,PixelFormat format,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType);

		~MetalTexture();
		
		id<MTLTexture>  GetNative() {return m_native;}

		virtual void	CopyTo(Texture* pDesc,int nOutFace, int nOutLevel, int nInFace,int nInlevel);

		virtual bool	SaveToFile(const char* pszPath);

    protected:
        virtual bool            LoadFromImagData(const ImageData& imageData);
        
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


