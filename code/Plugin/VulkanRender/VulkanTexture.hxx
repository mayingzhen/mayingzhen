#include "VulkanTexture.h"
#include "VulkanMapping.h"
#include "VulkanRenderDevice.h"
#include "Engine\ImageData\ImageCodec.h"

namespace ma
{
	VulkanTexture::VulkanTexture()
		:Texture()
	{
// 		m_pVulkanTex2D = NULL;
// 		m_pVulkanShaderResourceView = NULL;
// 		m_pVulkanShaderResourceViewSRGBNotEqual = NULL;
// 		m_pDepthStencilView = NULL;
// 		memset(m_pRenderTargetView,0,sizeof(m_pRenderTargetView));
	}

	VulkanTexture::VulkanTexture(int nWidth,int nHeight,uint32 nMipMap,PixelFormat format,bool bTypeLess,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType)
		:Texture(nWidth,nHeight,nMipMap,format,bTypeLess,bSRGB,eUsage,eType)
	{
// 		m_pVulkanTex2D = NULL;
// 		m_pVulkanShaderResourceView = NULL;
// 		m_pVulkanShaderResourceViewSRGBNotEqual = NULL;
// 		m_pDepthStencilView = NULL;
// 		memset(m_pRenderTargetView,0,sizeof(m_pRenderTargetView));
	}

	VulkanTexture::~VulkanTexture()
	{
// 		SAFE_RELEASE(m_pVulkanTex2D);
// 		SAFE_RELEASE(m_pVulkanShaderResourceView);
// 		SAFE_RELEASE(m_pVulkanShaderResourceViewSRGBNotEqual);
// 		SAFE_RELEASE(m_pDepthStencilView);
// 		for (uint32 i = 0; i < MAX_MIP; ++i)
// 		{
// 			for (uint32 j = 0; j < 6; ++j)
// 			{
// 				SAFE_RELEASE(m_pRenderTargetView[i][j]);
// 			}
// 		}
	}

	bool VulkanTexture::GenerateMipmaps()
	{	
		return true;
	}

// 	DXGI_FORMAT GetSRVFormat(DXGI_FORMAT format)
// 	{
// 		if (format == DXGI_FORMAT_R24G8_TYPELESS)
// 			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
// 		else if (format == DXGI_FORMAT_R16_TYPELESS)
// 			return DXGI_FORMAT_R16_UNORM;
// 		else if (format == DXGI_FORMAT_R32_TYPELESS)
// 			return DXGI_FORMAT_R32_FLOAT;
// 		else if (format == DXGI_FORMAT_R8G8B8A8_TYPELESS)
// 			return DXGI_FORMAT_R8G8B8A8_UNORM;
// 		else
// 			return format;
// 	}

// 	DXGI_FORMAT GetDSVFormat(DXGI_FORMAT format)
// 	{
// 		if (format == DXGI_FORMAT_R24G8_TYPELESS)
// 			return DXGI_FORMAT_D24_UNORM_S8_UINT;
// 		else if (format == DXGI_FORMAT_R16_TYPELESS)
// 			return DXGI_FORMAT_D16_UNORM;
// 		else if (format == DXGI_FORMAT_R32_TYPELESS)
// 			return DXGI_FORMAT_D32_FLOAT;
// 		else
// 			return format;
// 	}

// 	DXGI_FORMAT GetSRGBFormat(DXGI_FORMAT format)
// 	{
// 		if (format == DXGI_FORMAT_R8G8B8A8_UNORM)
// 			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
// 		else if (format == DXGI_FORMAT_BC1_UNORM)
// 			return DXGI_FORMAT_BC1_UNORM_SRGB;
// 		else if (format == DXGI_FORMAT_BC2_UNORM)
// 			return DXGI_FORMAT_BC2_UNORM_SRGB;
// 		else if (format == DXGI_FORMAT_BC3_UNORM)
// 			return DXGI_FORMAT_BC3_UNORM_SRGB;
// 		else
// 			return format;
// 	}

// 	DXGI_FORMAT GetTypelessFormat(DXGI_FORMAT format) {
// 		switch(format) {
// 		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
// 		case DXGI_FORMAT_R8G8B8A8_UNORM:
// 		case DXGI_FORMAT_R8G8B8A8_UINT:
// 		case DXGI_FORMAT_R8G8B8A8_SNORM:
// 		case DXGI_FORMAT_R8G8B8A8_SINT:
// 			return DXGI_FORMAT_R8G8B8A8_TYPELESS;
// 
// 		case DXGI_FORMAT_BC1_UNORM_SRGB:
// 		case DXGI_FORMAT_BC1_UNORM:
// 			return DXGI_FORMAT_BC1_TYPELESS;
// 		case DXGI_FORMAT_BC2_UNORM_SRGB:
// 		case DXGI_FORMAT_BC2_UNORM:
// 			return DXGI_FORMAT_BC2_TYPELESS;
// 		case DXGI_FORMAT_BC3_UNORM_SRGB:
// 		case DXGI_FORMAT_BC3_UNORM:
// 			return DXGI_FORMAT_BC3_TYPELESS;
// 
// 		// Depth
// 		case DXGI_FORMAT_D24_UNORM_S8_UINT:
// 			return DXGI_FORMAT_R24G8_TYPELESS;
// 		};
// 		
// 		ASSERT(false);
// 		return format;
// 	}

// 	bool IsCompressed(DXGI_FORMAT format)
// 	{
// 		return format == DXGI_FORMAT_BC1_UNORM || format == DXGI_FORMAT_BC2_UNORM || format == DXGI_FORMAT_BC3_UNORM;
// 	}

	bool VulkanTexture::RT_CreateCubeTexture()
	{
// 		m_eFormat = VulkanMapping::_getClosestSupportedPF(m_eFormat);
// 		m_descFormat = VulkanMapping::_getPF(m_eFormat);
// 		if (m_bSRGB)
// 		{
// 			m_descFormat = GetSRGBFormat(m_descFormat);
// 		}
// 		if (m_bTypeLess)
// 		{
// 			m_descFormat = GetTypelessFormat(m_descFormat);
// 		}
// 
// 		Vulkan_TEXTURE2D_DESC textureDesc;
// 		memset(&textureDesc, 0, sizeof textureDesc);
// 		textureDesc.Width = (UINT)m_nWidth;
// 		textureDesc.Height = (UINT)m_nHeight;
// 		textureDesc.MipLevels = m_nMipLevels;
// 		textureDesc.ArraySize = 6;
// 		textureDesc.Format = m_descFormat;
// 		textureDesc.SampleDesc.Count = 1;
// 		textureDesc.SampleDesc.Quality = 0;
// 		textureDesc.Usage = m_eUsage == USAGE_DYNAMIC ? Vulkan_USAGE_DYNAMIC : Vulkan_USAGE_DEFAULT;
// 		textureDesc.BindFlags = Vulkan_BIND_SHADER_RESOURCE;
// 		if (m_eUsage == USAGE_RENDERTARGET)
// 			textureDesc.BindFlags |= Vulkan_BIND_RENDER_TARGET;
// 		else if (m_eUsage == USAGE_DEPTHSTENCIL)
// 			textureDesc.BindFlags |= Vulkan_BIND_DEPTH_STENCIL;
// 		textureDesc.CPUAccessFlags = m_eUsage == USAGE_DYNAMIC ? Vulkan_CPU_ACCESS_WRITE : 0;
// 		textureDesc.MiscFlags |= Vulkan_RESOURCE_MISC_TEXTURECUBE;
// 
// 		GetVulkanDxDevive()->CreateTexture2D(&textureDesc, 0, (IVulkanTexture2D**)&m_pVulkanTex2D);
// 		ASSERT(m_pVulkanTex2D);
// 		if (m_pVulkanTex2D == NULL)
// 		{
// 			LogError("Failed to create DepthStencile");
// 			return false;
// 		}
// 
// 		DXGI_FORMAT srvFormat = VulkanMapping::_getPF(m_eFormat);
// 		if (m_bSRGB)
// 		{
// 			srvFormat = GetSRGBFormat(srvFormat);
// 		}
// 		if (m_bTypeLess)
// 		{
// 			srvFormat = GetSRVFormat(m_descFormat);
// 		}
// 
// 		Vulkan_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
// 		memset(&resourceViewDesc, 0, sizeof resourceViewDesc);
// 		resourceViewDesc.Format = srvFormat;
// 		resourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
// 		resourceViewDesc.TextureCube.MipLevels = m_nMipLevels;
// 
// 		GetVulkanDxDevive()->CreateShaderResourceView(m_pVulkanTex2D, &resourceViewDesc,&m_pVulkanShaderResourceView);
// 		if (!m_pVulkanShaderResourceView)
// 		{
// 			LogError("Failed to create shader resource view for texture");
// 			return false;
// 		}

		return true;
	}

	bool VulkanTexture::RT_CreateTexture()
	{
// 		m_eFormat = VulkanMapping::_getClosestSupportedPF(m_eFormat);
// 		m_descFormat = VulkanMapping::_getPF(m_eFormat);
// 		if (m_bSRGB)
// 		{
// 			m_descFormat = GetSRGBFormat(m_descFormat);
// 		}
// 		if (m_bTypeLess)
// 		{
// 			m_descFormat = GetTypelessFormat(m_descFormat);
// 		}
// 
// 		Vulkan_TEXTURE2D_DESC textureDesc;
// 		memset(&textureDesc, 0, sizeof textureDesc);
// 		textureDesc.Width = (UINT)m_nWidth;
// 		textureDesc.Height = (UINT)m_nHeight;
// 		textureDesc.MipLevels = m_nMipLevels;
// 		textureDesc.ArraySize = 1;
// 		textureDesc.Format = m_descFormat;
// 		textureDesc.SampleDesc.Count = 1;
// 		textureDesc.SampleDesc.Quality = 0;
// 		//if (m_eUsage == USAGE_STATIC)
// 		//{
// 		//	textureDesc.Usage = Vulkan_USAGE_IMMUTABLE;
// 		//}
// 		//else 
// 		if (m_eUsage == USAGE_DYNAMIC)
// 		{
// 			textureDesc.Usage = Vulkan_USAGE_DYNAMIC;
// 		}
// 		else 
// 		{
// 			textureDesc.Usage = Vulkan_USAGE_DEFAULT;
// 		}
// 		textureDesc.BindFlags = Vulkan_BIND_SHADER_RESOURCE;
// 		if (m_eUsage == USAGE_RENDERTARGET)
// 			textureDesc.BindFlags |= Vulkan_BIND_RENDER_TARGET;
// 		else if (m_eUsage == USAGE_DEPTHSTENCIL)
// 			textureDesc.BindFlags |= Vulkan_BIND_DEPTH_STENCIL;
// 		textureDesc.CPUAccessFlags = m_eUsage == USAGE_DYNAMIC ? Vulkan_CPU_ACCESS_WRITE : 0;
// 
// 		GetVulkanDxDevive()->CreateTexture2D(&textureDesc, 0, (IVulkanTexture2D**)&m_pVulkanTex2D);
// 		ASSERT(m_pVulkanTex2D);
// 		if (m_pVulkanTex2D == NULL)
// 		{
// 			LogError("Failed to create DepthStencile");
// 			return false;
// 		}
// 
// 		DXGI_FORMAT srvFormat = VulkanMapping::_getPF(m_eFormat);
// 		if (m_bSRGB)
// 		{
// 			srvFormat = GetSRGBFormat(srvFormat);
// 		}
// 		if (m_bTypeLess)
// 		{
// 			srvFormat = GetSRVFormat(m_descFormat);
// 		}
// 
// 		Vulkan_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
// 		memset(&resourceViewDesc, 0, sizeof resourceViewDesc);
// 		resourceViewDesc.Format = srvFormat;
// 		resourceViewDesc.ViewDimension = Vulkan_SRV_DIMENSION_TEXTURE2D;
// 		resourceViewDesc.Texture2D.MipLevels = m_nMipLevels;
// 
// 		GetVulkanDxDevive()->CreateShaderResourceView(m_pVulkanTex2D, &resourceViewDesc,&m_pVulkanShaderResourceView);
// 		if (!m_pVulkanShaderResourceView)
// 		{
// 			LogError("Failed to create shader resource view for texture");
// 			return false;
// 		}

		return true;
	}

// 	IVulkanShaderResourceView* VulkanTexture::GetShaderResourceViewSRGBNotEqual()
// 	{
// 		if (m_pVulkanShaderResourceViewSRGBNotEqual)
// 			return m_pVulkanShaderResourceViewSRGBNotEqual;
// 
// 		ASSERT(m_bSRGB);
// 
// 		DXGI_FORMAT srvFormat = VulkanMapping::_getPF(m_eFormat);
// 
// 		Vulkan_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
// 		memset(&resourceViewDesc, 0, sizeof resourceViewDesc);
// 		resourceViewDesc.Format = srvFormat;
// 		resourceViewDesc.ViewDimension = Vulkan_SRV_DIMENSION_TEXTURE2D;
// 		resourceViewDesc.Texture2D.MipLevels = m_nMipLevels;
// 
// 		GetVulkanDxDevive()->CreateShaderResourceView(m_pVulkanTex2D, &resourceViewDesc,&m_pVulkanShaderResourceViewSRGBNotEqual);
// 		if (!m_pVulkanShaderResourceView)
// 		{
// 			LogError("Failed to create shader resource view for texture");
// 			return NULL;
// 		}
// 
// 		return m_pVulkanShaderResourceViewSRGBNotEqual;
// 	}

// 	IVulkanDepthStencilView* VulkanTexture::GetDepthStencilView()
// 	{
// 		if (m_pDepthStencilView)
// 			return m_pDepthStencilView;
// 
// 		Vulkan_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
// 		memset(&depthStencilViewDesc, 0, sizeof depthStencilViewDesc);
// 		depthStencilViewDesc.Format = GetDSVFormat(m_descFormat);
// 		depthStencilViewDesc.ViewDimension = Vulkan_DSV_DIMENSION_TEXTURE2D;
// 
// 		GetVulkanDxDevive()->CreateDepthStencilView(m_pVulkanTex2D,&depthStencilViewDesc,&m_pDepthStencilView);
// 		if (!m_pDepthStencilView)
// 		{
// 			LogError("Failed to create depth-stencil view for texture");
// 			return NULL;
// 		}
// 
// 		return m_pDepthStencilView;
// 	}

// 	IVulkanRenderTargetView* VulkanTexture::GetRenderTargetView(int level, int array_index, int face)
// 	{
// 		ASSERT(array_index == 0 && level < MAX_MIP);
// 		if (array_index != 0 || level >= MAX_MIP)
// 			return NULL;
// 
// 		if (m_pRenderTargetView[level][face])
// 			return m_pRenderTargetView[level][face];
// 
// 		m_eFormat = VulkanMapping::_getClosestSupportedPF(m_eFormat);
// 		m_descFormat = VulkanMapping::_getPF(m_eFormat);
// 		if (m_bSRGB)
// 		{
// 			m_descFormat = GetSRGBFormat(m_descFormat);
// 		}
// 
// 		Vulkan_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
// 		memset(&renderTargetViewDesc, 0, sizeof renderTargetViewDesc);
// 		renderTargetViewDesc.Format = m_descFormat;
// 		if (m_eType == TEXTYPE_2D)
// 		{
// 			renderTargetViewDesc.ViewDimension = Vulkan_RTV_DIMENSION_TEXTURE2D;
// 		}
// 		else if (m_eType == TEXTYPE_CUBE)
// 		{
// 			renderTargetViewDesc.ViewDimension = Vulkan_RTV_DIMENSION_TEXTURE2DARRAY;
// 			renderTargetViewDesc.Texture2DArray.MipSlice = level;
// 			renderTargetViewDesc.Texture2DArray.FirstArraySlice = array_index * 6 + face;
// 			renderTargetViewDesc.Texture2DArray.ArraySize = 1;
// 		}
// 
// 		IVulkanRenderTargetView*	pRenderTargetView = NULL;
// 		GetVulkanDxDevive()->CreateRenderTargetView((IVulkanResource*)m_pVulkanTex2D, &renderTargetViewDesc,&pRenderTargetView);
// 		if (!pRenderTargetView)
// 		{
// 			LogError("Failed to create renderTarget view for texture");
// 			return NULL;
// 		}
// 	
// 		m_pRenderTargetView[level][face] = pRenderTargetView;
// 
// 		return pRenderTargetView;
// 	}

// 	void VulkanTexture::SetRenderTargetView(IVulkanRenderTargetView* pView,int level, int array_index, int face)
// 	{
// 		ASSERT(array_index == 0 && level < MAX_MIP);
// 		if (array_index != 0 || level >= MAX_MIP)
// 			return;
// 
// 		m_pRenderTargetView[level][face] = pView;
// 	}


// 	Vulkan_BOX OgreImageBoxToDx11Box(const Box &inBox)
// 	{
// 		Vulkan_BOX res;
// 		res.left	= static_cast<UINT>(inBox.left);
// 		res.top		= static_cast<UINT>(inBox.top);
// 		res.front	= static_cast<UINT>(inBox.front);
// 		res.right	= static_cast<UINT>(inBox.right);
// 		res.bottom	= static_cast<UINT>(inBox.bottom);
// 		res.back	= static_cast<UINT>(inBox.back);
// 
// 		return res;
// 	}

	bool VulkanTexture::SetLevelData(int nLevel, int nFace, const PixelBox& src)
	{
		// for scoped deletion of conversion buffer
// 		RefPtr<MemoryStream> buf;
// 		PixelBox converted = src;
// 		
// 		int width = m_nWidth >> nLevel;
// 		int height = m_nHeight >> nLevel;
// 
// 		if (PixelUtil::isCompressed(converted.format))
// 		{
// 			width += 3;
// 			width &= 0xfffffffc;
// 			height += 3;
// 			height &= 0xfffffffc;
// 		}
// 
// 		Box dstBox(0,0,width,height);
// 
// 		Vulkan_BOX dstBoxDx11 = OgreImageBoxToDx11Box(dstBox);
// 		dstBoxDx11.front = 0;
// 		dstBoxDx11.back = converted.getDepth();
// 
// 		// convert to pixelbuffer's native format if necessary
// 		if (VulkanMapping::_getPF(src.format) == DXGI_FORMAT_UNKNOWN)
// 		{
// 			buf = CreateMemoryStream(PixelUtil::getMemorySize(src.getWidth(), src.getHeight(), src.getDepth(),m_eFormat), false);
// 			converted = PixelBox(src.getWidth(), src.getHeight(), src.getDepth(), m_eFormat, buf->GetPtr());
// 			PixelUtil::bulkPixelConversion(src, converted);
// 		}
// 
// 		uint32 rowWidth;
// 		if (PixelUtil::isCompressed(converted.format))
// 		{
// 			// D3D wants the width of one row of cells in bytes
// 			if (converted.format == PF_DXT1)
// 			{
// 				// 64 bits (8 bytes) per 4x4 block
// 				rowWidth = (converted.rowPitch / 4) * 8;
// 			}
// 			else
// 			{
// 				// 128 bits (16 bytes) per 4x4 block
// 				rowWidth = (converted.rowPitch / 4) * 16;
// 			}
// 		}
// 		else
// 		{
// 			rowWidth = converted.rowPitch * PixelUtil::getNumElemBytes(converted.format);
// 		}
// 
// 		unsigned subResource = VulkanCalcSubresource(nLevel, nFace, m_nMipLevels);
// 
// 		GetVulkanDxDeviveContext()->UpdateSubresource( 
// 			m_pVulkanTex2D, 
// 			subResource,
// 			&dstBoxDx11,
// 			converted.data,
// 			rowWidth,
// 			0 );
		
		return true;
	}

	bool VulkanTexture::LoadFromImagData(const ImageData& imageData)
	{
		// Set desired texture size and properties from images[0]
		m_nWidth = imageData.m_nWidth;
		m_nHeight = imageData.m_nHeight;

		// Get source image format and adjust if required
		m_eFormat = VulkanMapping::_getClosestSupportedPF(imageData.m_eFormat);
		if (m_eFormat != imageData.m_eFormat)
		{
			ImageData& tem = const_cast<ImageData&>(imageData);
			tem.bulkPixelConversion(m_eFormat);
		}
		m_vkformat = VulkanMapping::_getPF(m_eFormat);

		// The custom mipmaps in the image have priority over everything
		//size_t imageMips = imageData.num_mipmaps;

		bool bAutoMipMap = m_bMipMap;

		if (imageData.m_nNumMipmaps > 0)
		{
			m_nMipLevels = imageData.m_nNumMipmaps;
			// Disable flag for auto mip generation
			//bAutoMipMap  = false;
		}

		vks::VulkanDevice* device = GetVulkanDevice();

		// Get device properites for the requested texture format
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(device->physicalDevice, m_vkformat, &formatProperties);

		VkMemoryAllocateInfo memAllocInfo = vks::initializers::memoryAllocateInfo();
		VkMemoryRequirements memReqs;

		VkCommandBuffer copyCmd = device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

		// Create a host-visible staging buffer that contains the raw image data
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;

		VkBufferCreateInfo bufferCreateInfo = vks::initializers::bufferCreateInfo();
		bufferCreateInfo.size = imageData.m_nSize;
		// This buffer is used as a transfer source for the buffer copy
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK_RESULT(vkCreateBuffer(device->logicalDevice, &bufferCreateInfo, nullptr, &stagingBuffer));

		// Get memory requirements for the staging buffer (alignment, memory type bits)
		vkGetBufferMemoryRequirements(device->logicalDevice, stagingBuffer, &memReqs);

		memAllocInfo.allocationSize = memReqs.size;
		// Get memory type index for a host visible buffer
		memAllocInfo.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAllocInfo, nullptr, &stagingMemory));
		VK_CHECK_RESULT(vkBindBufferMemory(device->logicalDevice, stagingBuffer, stagingMemory, 0));

		// Copy texture data into staging buffer
		uint8_t *data;
		VK_CHECK_RESULT(vkMapMemory(device->logicalDevice, stagingMemory, 0, memReqs.size, 0, (void **)&data));
		memcpy(data, imageData.m_pMemory->GetPtr(), imageData.m_pMemory->GetSize());
		vkUnmapMemory(device->logicalDevice, stagingMemory);

		// Setup buffer copy regions for each mip level
		std::vector<VkBufferImageCopy> bufferCopyRegions;
		uint32_t offset = 0;

		for (uint32_t i = 0; i < m_nMipLevels/*imageData.num_mipmaps*/; i++)
		{
			int width = m_nWidth >> i;
			int height = m_nHeight >> i;

			UINT size = PixelUtil::getMemorySize(width, height, 1, m_eFormat);

			VkBufferImageCopy bufferCopyRegion = {};
			bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferCopyRegion.imageSubresource.mipLevel = i;
			bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
			bufferCopyRegion.imageSubresource.layerCount = 1;
			bufferCopyRegion.imageExtent.width = static_cast<uint32_t>(width);
			bufferCopyRegion.imageExtent.height = static_cast<uint32_t>(height);
			bufferCopyRegion.imageExtent.depth = 1;
			bufferCopyRegion.bufferOffset = offset;

			bufferCopyRegions.push_back(bufferCopyRegion);

			offset += static_cast<uint32_t>(size);
		}

		// Create optimal tiled target image
		VkImageCreateInfo imageCreateInfo = vks::initializers::imageCreateInfo();
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = m_vkformat;
		imageCreateInfo.mipLevels = m_nMipLevels;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.extent = { (uint32)m_nWidth, (uint32)m_nHeight, 1 };
		imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT/*imageUsageFlags*/;
		// Ensure that the TRANSFER_DST bit is set for staging
		if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
		{
			imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}
		VK_CHECK_RESULT(vkCreateImage(device->logicalDevice, &imageCreateInfo, nullptr, &m_image));

		vkGetImageMemoryRequirements(device->logicalDevice, m_image, &memReqs);

		memAllocInfo.allocationSize = memReqs.size;

		memAllocInfo.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAllocInfo, nullptr, &m_deviceMemory));
		VK_CHECK_RESULT(vkBindImageMemory(device->logicalDevice, m_image, m_deviceMemory, 0));

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = m_nMipLevels;
		subresourceRange.layerCount = 1;

		// Image barrier for optimal image (target)
		// Optimal image will be used as destination for the copy
		vks::tools::setImageLayout(
			copyCmd,
			m_image,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			subresourceRange);

		// Copy mip levels from staging buffer
		vkCmdCopyBufferToImage(
			copyCmd,
			stagingBuffer,
			m_image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			static_cast<uint32_t>(bufferCopyRegions.size()),
			bufferCopyRegions.data()
		);

		// Change texture image layout to shader read after all mip levels have been copied
		m_imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		vks::tools::setImageLayout(
			copyCmd,
			m_image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			m_imageLayout,
			subresourceRange);

		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		device->flushCommandBuffer(copyCmd, pRender->m_queue);

		// Clean up staging resources
		vkFreeMemory(device->logicalDevice, stagingMemory, nullptr);
		vkDestroyBuffer(device->logicalDevice, stagingBuffer, nullptr);

		// Create image view
		// Textures are not directly accessed by the shaders and
		// are abstracted by image views containing additional
		// information and sub resource ranges
		VkImageViewCreateInfo view = vks::initializers::imageViewCreateInfo();
		view.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view.format = m_vkformat;
		view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		// The subresource range describes the set of mip levels (and array layers) that can be accessed through this image view
		// It's possible to create multiple image views for a single image referring to different (and/or overlapping) ranges of the image
		view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		view.subresourceRange.baseMipLevel = 0;
		view.subresourceRange.baseArrayLayer = 0;
		view.subresourceRange.layerCount = 1;
		// Linear tiling usually won't support mip maps
		// Only set mip map count if optimal tiling is used
		view.subresourceRange.levelCount = m_nMipLevels/* (useStaging) ? texture.mipLevels : 1*/;
		// The view will be based on the texture's image
		view.image = m_image;
		VK_CHECK_RESULT(vkCreateImageView(device->logicalDevice, &view, nullptr, &m_view));


		m_eResState = ResInited;

		return true;

	}

	void VulkanTexture::CopyTo(Texture* pDes,int nOutFace, int nOutLevel, int nInFace,int nInlevel)
	{
// 		VulkanTexture* pDestVulkan = (VulkanTexture*)(pDes);
// 
// 		Vulkan_BOX src_box;
// 		src_box.left = 0;
// 		src_box.top = 0;
// 		src_box.front = 0;
// 		src_box.right = GetWidth();
// 		src_box.bottom = GetHeight();
// 		src_box.back = 1;
// 
// 		GetVulkanDxDeviveContext()->CopySubresourceRegion(
// 			pDestVulkan->GetTexture2D(), VulkanCalcSubresource(nOutLevel, nOutFace, pDestVulkan->GetMipMapNumber()), 0, 0, 0, 
// 			this->GetTexture2D(), VulkanCalcSubresource(nInlevel,nInFace, this->GetMipMapNumber()), &src_box);
	}

	bool VulkanTexture::SaveToFile(const char* pszPath)
	{
// 		HRESULT hr = D3DX11SaveTextureToFile(GetVulkanDxDeviveContext(),GetTexture2D(),D3DX11_IFF_DDS,pszPath);
// 		return hr == S_OK;
		return false;
	}
}



