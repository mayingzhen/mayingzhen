#include "MetalTexture.h"
#include "MetalMapping.h"
#include "MetalRenderDevice.h"

namespace ma
{
	MetalTexture::MetalTexture()
		:Texture()
	{
        m_native = nil;
	}

	MetalTexture::MetalTexture(int nWidth,int nHeight,uint32_t nMipMap,PixelFormat format,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType)
		:Texture(nWidth,nHeight,nMipMap,format,bSRGB,eUsage,eType)
	{
        m_native = nil;
	}

	MetalTexture::~MetalTexture()
	{
        if (m_native != nil)
        {
            [m_native release];
            m_native = nil;
        }
	}

	bool MetalTexture::GenerateMipmaps()
	{	
		return true;
	}


	MTLPixelFormat GetSRGBFormat(MTLPixelFormat format)
	{
		if (format == MTLPixelFormatBGRA8Unorm)
			return MTLPixelFormatBGRA8Unorm_sRGB;
		else if (format == MTLPixelFormatPVRTC_RGB_2BPP)
			return MTLPixelFormatPVRTC_RGB_2BPP_sRGB;
		else if (format == MTLPixelFormatPVRTC_RGB_4BPP)
			return MTLPixelFormatPVRTC_RGB_4BPP_sRGB;
		else
			return format;
	}

	bool MetalTexture::RT_CreateCubeTexture()
	{
        m_eFormat = MetalMapping::_getClosestSupportedPF(m_eFormat);
        m_descFormat = MetalMapping::_getPF(m_eFormat);
        if (m_bSRGB)
        {
            m_descFormat = GetSRGBFormat(m_descFormat);
        }
        
        MTLTextureDescriptor* textureDesc = [MTLTextureDescriptor new];
        textureDesc.textureType = MTLTextureTypeCube;
        textureDesc.pixelFormat = m_descFormat;
        textureDesc.width = (uint32_t)m_nWidth;
        textureDesc.height = (uint32_t)m_nHeight;
        textureDesc.mipmapLevelCount = m_nMipLevels;
        textureDesc.arrayLength = 1;
        textureDesc.pixelFormat = m_descFormat;
        textureDesc.sampleCount= 1;
        if (m_eUsage == USAGE_DYNAMIC)
        {
            textureDesc.resourceOptions = MTLResourceStorageModeShared;
        }
        else
        {
            textureDesc.resourceOptions = MTLResourceCPUCacheModeDefaultCache;
        }
        
        if (m_eUsage == USAGE_RENDERTARGET || m_eUsage == USAGE_DEPTHSTENCIL)
        {
            textureDesc.usage = MTLTextureUsageShaderRead | MTLTextureUsageRenderTarget;
        }
        
        m_native = [GetMetalDevive() newTextureWithDescriptor:textureDesc];
        ASSERT(m_native != nil);
        if (m_native == nil)
        {
            [textureDesc release];
            LogError("Failed to create DepthStencile");
            return false;
        }
        
        
        [textureDesc release];
        return true;
	}

	bool MetalTexture::RT_CreateTexture()
	{
		m_eFormat = MetalMapping::_getClosestSupportedPF(m_eFormat);
		m_descFormat = MetalMapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			m_descFormat = GetSRGBFormat(m_descFormat);
		}

		MTLTextureDescriptor* textureDesc = [MTLTextureDescriptor new];
		textureDesc.textureType = MTLTextureType2D;
        textureDesc.pixelFormat = m_descFormat;
		textureDesc.width = (uint32_t)m_nWidth;
		textureDesc.height = (uint32_t)m_nHeight;
		textureDesc.mipmapLevelCount = m_nMipLevels;
		textureDesc.arrayLength = 1;
		textureDesc.pixelFormat = m_descFormat;
		textureDesc.sampleCount= 1;
		if (m_eUsage == USAGE_DYNAMIC)
		{
			textureDesc.resourceOptions = MTLResourceStorageModeShared;
		}
		else 
		{
			textureDesc.resourceOptions = MTLResourceCPUCacheModeDefaultCache;
		}
        
        if (m_eUsage == USAGE_RENDERTARGET || m_eUsage == USAGE_DEPTHSTENCIL)
        {
            textureDesc.usage = MTLTextureUsageShaderRead | MTLTextureUsageRenderTarget;
        }

		m_native = [GetMetalDevive() newTextureWithDescriptor:textureDesc];
		ASSERT(m_native != nil);
		if (m_native == nil)
		{
            [textureDesc release];
			LogError("Failed to create DepthStencile");
			return false;
		}
        

        [textureDesc release];
		return true;
	}
    
    bool MetalTexture::LoadFromImagData(const ImageData& imageData)
    {
        m_eFormat = MetalMapping::_getClosestSupportedPF(imageData.m_eFormat);
        if (m_eFormat != imageData.m_eFormat)
        {
            ImageData& tem = const_cast<ImageData&>(imageData);
            tem.bulkPixelConversion(m_eFormat);
        }
        
        // Set desired texture size and properties from images[0]
        m_nWidth = imageData.m_nWidth;
        m_nHeight = imageData.m_nHeight;
        
        // Get source image format and adjust if required
        m_eFormat = imageData.m_eFormat;
        
        // The custom mipmaps in the image have priority over everything
        //size_t imageMips = imageData.num_mipmaps;
        
        bool bAutoMipMap = m_bMipMap;
        
        if(imageData.m_nNumMipmaps > 0)
        {
            m_nMipLevels = imageData.m_nNumMipmaps;
            // Disable flag for auto mip generation
            //bAutoMipMap  = false;
        }
        
        // Create the texture
        if (imageData.m_nFlags & IF_CUBEMAP)
        {
            m_eType = TEXTYPE_CUBE;
            if (!RT_CreateCubeTexture())
            {
                LogError("Failed to createInternalResources:%d, %d, %s, %d", m_nWidth, m_nHeight, this->GetResPath(), m_eFormat);
                return false;
            }
            
            for(uint16_t mip = 0; mip <= imageData.m_nNumMipmaps && mip < m_nMipLevels; ++mip)
            {
                for (uint32_t iFace = 0; iFace < 6; ++iFace)
                {
                    PixelBox src = imageData.GetPixelBox(iFace, mip);
                    
                    SetLevelData(mip,iFace,src);
                }
            }
        }
        else
        {
            m_eType = TEXTYPE_2D;
            if (!RT_CreateTexture())
            {
                LogError("Failed to createInternalResources:%d, %d, %s, %d", m_nWidth, m_nHeight, this->GetResPath(), m_eFormat);
                return false;
            }
            
            for(uint16_t mip = 0; mip <= imageData.m_nNumMipmaps && mip < m_nMipLevels; ++mip)
            {
                PixelBox src = imageData.GetPixelBox(0, mip);
                
                SetLevelData(mip,0,src);
            }
        }
        
        if (bAutoMipMap)
        {
            GenerateMipmaps();
        }
        
        m_eResState = ResInited;
        
        return true;
    }

	bool MetalTexture::SetLevelData(int nLevel, int nFace, const PixelBox& src)
	{
        int width = m_nWidth >> nLevel;
        int height = m_nHeight >> nLevel;
        
        PixelBox conver = src;
        
        MTLRegion region = MTLRegionMake2D(0, 0, (NSUInteger)width, (NSUInteger)height);
        NSUInteger bytesPerRow = conver.rowPitch * PixelUtil::getNumElemBytes(conver.format);//BytesPerRow(imgSurface);
        [m_native replaceRegion:region mipmapLevel:nLevel slice:nFace withBytes:conver.data bytesPerRow:bytesPerRow bytesPerImage:0];
        
        return true;
	}

	void MetalTexture::CopyTo(Texture* pDes,int nOutFace, int nOutLevel, int nInFace,int nInlevel)
	{
        /*
		MetalTexture* pDestMetal = (MetalTexture*)(pDes);

		Metal_BOX src_box;
		src_box.left = 0;
		src_box.top = 0;
		src_box.front = 0;
		src_box.right = GetWidth();
		src_box.bottom = GetHeight();
		src_box.back = 1;

		GetMetalDxDeviveContext()->CopySubresourceRegion(
			pDestMetal->GetTexture2D(), MetalCalcSubresource(nOutLevel, nOutFace, pDestMetal->GetMipMapNumber()), 0, 0, 0, 
			this->GetTexture2D(), MetalCalcSubresource(nInlevel,nInFace, this->GetMipMapNumber()), &src_box);
         */
	}

	bool MetalTexture::SaveToFile(const char* pszPath)
	{
// 		HRESULT hr = D3DX11SaveTextureToFile(GetMetalDxDeviveContext(),GetTexture2D(),D3DX11_IFF_DDS,pszPath);
// 		return hr == S_OK;
		return false;
	}
}



