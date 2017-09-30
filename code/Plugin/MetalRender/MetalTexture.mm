#include "MetalTexture.h"
#include "MetalMapping.h"
#include "MetalRenderDevice.h"

namespace ma
{
	MetalTexture::MetalTexture()
		:Texture()
	{
        m_native = nil;
        //m_stencil = nil;
	}

	MetalTexture::MetalTexture(int nWidth,int nHeight,uint32 nMipMap,PixelFormat format,bool bTypeLess,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType)
		:Texture(nWidth,nHeight,nMipMap,format,bTypeLess,bSRGB,eUsage,eType)
	{
        m_native = nil;
        //m_stencil = nil;
	}

	MetalTexture::~MetalTexture()
	{
        if (m_native != nil)
        {
            [m_native release];
            m_native = nil;
        }
        //if (m_stencil != nil)
        //{
        //    [m_stencil release];
        //    m_stencil = nil;
       // }
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
        /*
		m_eFormat = MetalMapping::_getClosestSupportedPF(m_eFormat);
		m_descFormat = MetalMapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			m_descFormat = GetSRGBFormat(m_descFormat);
		}
		if (m_bTypeLess)
		{
			m_descFormat = GetTypelessFormat(m_descFormat);
		}

		Metal_TEXTURE2D_DESC textureDesc;
		memset(&textureDesc, 0, sizeof textureDesc);
		textureDesc.Width = (UINT)m_nWidth;
		textureDesc.Height = (UINT)m_nHeight;
		textureDesc.MipLevels = m_nMipLevels;
		textureDesc.ArraySize = 6;
		textureDesc.Format = m_descFormat;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = m_eUsage == USAGE_DYNAMIC ? Metal_USAGE_DYNAMIC : Metal_USAGE_DEFAULT;
		textureDesc.BindFlags = Metal_BIND_SHADER_RESOURCE;
		if (m_eUsage == USAGE_RENDERTARGET)
			textureDesc.BindFlags |= Metal_BIND_RENDER_TARGET;
		else if (m_eUsage == USAGE_DEPTHSTENCIL)
			textureDesc.BindFlags |= Metal_BIND_DEPTH_STENCIL;
		textureDesc.CPUAccessFlags = m_eUsage == USAGE_DYNAMIC ? Metal_CPU_ACCESS_WRITE : 0;
		textureDesc.MiscFlags |= Metal_RESOURCE_MISC_TEXTURECUBE;

		GetMetalDxDevive()->CreateTexture2D(&textureDesc, 0, (IMetalTexture2D**)&m_pMetalTex2D);
		ASSERT(m_pMetalTex2D);
		if (m_pMetalTex2D == NULL)
		{
			LogError("Failed to create DepthStencile");
			return false;
		}

		DXGI_FORMAT srvFormat = MetalMapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			srvFormat = GetSRGBFormat(srvFormat);
		}
		if (m_bTypeLess)
		{
			srvFormat = GetSRVFormat(m_descFormat);
		}

		Metal_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
		memset(&resourceViewDesc, 0, sizeof resourceViewDesc);
		resourceViewDesc.Format = srvFormat;
		resourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
		resourceViewDesc.TextureCube.MipLevels = m_nMipLevels;

		GetMetalDxDevive()->CreateShaderResourceView(m_pMetalTex2D, &resourceViewDesc,&m_pMetalShaderResourceView);
		if (!m_pMetalShaderResourceView)
		{
			LogError("Failed to create shader resource view for texture");
			return false;
		}
         */

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
		textureDesc.width = (UINT)m_nWidth;
		textureDesc.height = (UINT)m_nHeight;
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
/*
	IMetalShaderResourceView* MetalTexture::GetShaderResourceViewSRGBNotEqual()
	{
		if (m_pMetalShaderResourceViewSRGBNotEqual)
			return m_pMetalShaderResourceViewSRGBNotEqual;

		ASSERT(m_bSRGB);

		DXGI_FORMAT srvFormat = MetalMapping::_getPF(m_eFormat);

		Metal_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
		memset(&resourceViewDesc, 0, sizeof resourceViewDesc);
		resourceViewDesc.Format = srvFormat;
		resourceViewDesc.ViewDimension = Metal_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MipLevels = m_nMipLevels;

		GetMetalDxDevive()->CreateShaderResourceView(m_pMetalTex2D, &resourceViewDesc,&m_pMetalShaderResourceViewSRGBNotEqual);
		if (!m_pMetalShaderResourceView)
		{
			LogError("Failed to create shader resource view for texture");
			return NULL;
		}

		return m_pMetalShaderResourceViewSRGBNotEqual;
	}

	IMetalDepthStencilView* MetalTexture::GetDepthStencilView()
	{
		if (m_pDepthStencilView)
			return m_pDepthStencilView;

		Metal_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		memset(&depthStencilViewDesc, 0, sizeof depthStencilViewDesc);
		depthStencilViewDesc.Format = GetDSVFormat(m_descFormat);
		depthStencilViewDesc.ViewDimension = Metal_DSV_DIMENSION_TEXTURE2D;

		GetMetalDxDevive()->CreateDepthStencilView(m_pMetalTex2D,&depthStencilViewDesc,&m_pDepthStencilView);
		if (!m_pDepthStencilView)
		{
			LogError("Failed to create depth-stencil view for texture");
			return NULL;
		}

		return m_pDepthStencilView;
	}

	IMetalRenderTargetView* MetalTexture::GetRenderTargetView(int level, int array_index, int face)
	{
		ASSERT(array_index == 0 && level < MAX_MIP);
		if (array_index != 0 || level >= MAX_MIP)
			return NULL;

		if (m_pRenderTargetView[level][face])
			return m_pRenderTargetView[level][face];

		m_eFormat = MetalMapping::_getClosestSupportedPF(m_eFormat);
		m_descFormat = MetalMapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			m_descFormat = GetSRGBFormat(m_descFormat);
		}

		Metal_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		memset(&renderTargetViewDesc, 0, sizeof renderTargetViewDesc);
		renderTargetViewDesc.Format = m_descFormat;
		if (m_eType == TEXTYPE_2D)
		{
			renderTargetViewDesc.ViewDimension = Metal_RTV_DIMENSION_TEXTURE2D;
		}
		else if (m_eType == TEXTYPE_CUBE)
		{
			renderTargetViewDesc.ViewDimension = Metal_RTV_DIMENSION_TEXTURE2DARRAY;
			renderTargetViewDesc.Texture2DArray.MipSlice = level;
			renderTargetViewDesc.Texture2DArray.FirstArraySlice = array_index * 6 + face;
			renderTargetViewDesc.Texture2DArray.ArraySize = 1;
		}

		IMetalRenderTargetView*	pRenderTargetView = NULL;
		GetMetalDxDevive()->CreateRenderTargetView((IMetalResource*)m_pMetalTex2D, &renderTargetViewDesc,&pRenderTargetView);
		if (!pRenderTargetView)
		{
			LogError("Failed to create renderTarget view for texture");
			return NULL;
		}
	
		m_pRenderTargetView[level][face] = pRenderTargetView;

		return pRenderTargetView;
	}

	void MetalTexture::SetRenderTargetView(IMetalRenderTargetView* pView,int level, int array_index, int face)
	{
		ASSERT(array_index == 0 && level < MAX_MIP);
		if (array_index != 0 || level >= MAX_MIP)
			return;

		m_pRenderTargetView[level][face] = pView;
	}
*/
    /*

	Metal_BOX OgreImageBoxToDx11Box(const Box &inBox)
	{
		Metal_BOX res;
		res.left	= static_cast<UINT>(inBox.left);
		res.top		= static_cast<UINT>(inBox.top);
		res.front	= static_cast<UINT>(inBox.front);
		res.right	= static_cast<UINT>(inBox.right);
		res.bottom	= static_cast<UINT>(inBox.bottom);
		res.back	= static_cast<UINT>(inBox.back);

		return res;
	}
     */
    
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
            
            for(size_t mip = 0; mip <= imageData.m_nNumMipmaps && mip < m_nMipLevels; ++mip)
            {
                for (UINT32 iFace = 0; iFace < 6; ++iFace)
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
            
            for(size_t mip = 0; mip <= imageData.m_nNumMipmaps && mip < m_nMipLevels; ++mip)
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
        //const image::Surface* imgSurface = static_cast<const image::Surface*>(image->GetMipmap(i).get());
        //MTLRegion region = MTLRegionMake2D(0, 0, (NSUInteger)imgSurface->Width(), (NSUInteger)imgSurface->Height());
        //NSUInteger bytesPerRow = BytesPerRow(imgSurface);
        //[GetMetalDevive() replaceRegion:region mipmapLevel:i slice:0 withBytes:imgSurface->Buffer() bytesPerRow:bytesPerRow bytesPerImage:0];
        
        
        //GL_ASSERT( glBindTexture(GL_TEXTURE_2D, m_pTex) );
        
        int width = m_nWidth >> nLevel;
        int height = m_nHeight >> nLevel;
        
        PixelBox conver = src;
        //PixelFormat closestFomat = GLESMapping::GetClosestFormat(m_PixelFormat,m_DataType);
        //if (closestFomat != src.format)
        //{
        //    conver.format = closestFomat;
        //    PixelUtil::bulkPixelConversion(src,conver);
       // }
        
        //const image::Surface* imgSurface = static_cast<const image::Surface*>(image->GetMipmap(i).get());
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



