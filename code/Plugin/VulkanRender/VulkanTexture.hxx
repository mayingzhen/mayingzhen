#include "VulkanTexture.h"
#include "VulkanMapping.h"
#include "VulkanRenderDevice.h"
#include "Engine\ImageData\ImageCodec.h"

namespace ma
{
	VulkanTexture::VulkanTexture()
		:Texture()
	{

	}

	VulkanTexture::VulkanTexture(int nWidth,int nHeight,uint32 nMipMap,PixelFormat format,bool bTypeLess,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType)
		:Texture(nWidth,nHeight,nMipMap,format,bTypeLess,bSRGB,eUsage,eType)
	{
	}

	VulkanTexture::~VulkanTexture()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		vkDestroyImageView(device->logicalDevice, m_view, nullptr);
		vkDestroyImage(device->logicalDevice, m_image, nullptr);
		vkFreeMemory(device->logicalDevice, m_deviceMemory, nullptr);
	}

	bool VulkanTexture::GenerateMipmaps()
	{	
		return true;
	}


	VkFormat GetSRGBFormat(VkFormat format)
	{
		if (format == VK_FORMAT_B8G8R8A8_UNORM)
			return VK_FORMAT_B8G8R8A8_SNORM;
		else if (format == VK_FORMAT_BC1_RGB_UNORM_BLOCK)
			return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
		else if (format == VK_FORMAT_BC2_UNORM_BLOCK)
			return VK_FORMAT_BC2_SRGB_BLOCK;
		else if (format == VK_FORMAT_BC3_UNORM_BLOCK)
			return VK_FORMAT_BC3_SRGB_BLOCK;
		else
			return format;
	}

	bool VulkanTexture::RT_CreateCubeTexture()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		m_eFormat = VulkanMapping::_getClosestSupportedPF(m_eFormat);
		m_vkformat = VulkanMapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			m_vkformat = GetSRGBFormat(m_vkformat);
		}

		// For shadow mapping we only need a depth attachment
		VkImageCreateInfo image = vks::initializers::imageCreateInfo();
		image.imageType = VK_IMAGE_TYPE_2D;
		image.extent.width = m_nWidth;
		image.extent.height = m_nHeight;
		image.extent.depth = 1;
		image.mipLevels = m_nMipLevels;
		image.arrayLayers = 6;
		image.samples = VK_SAMPLE_COUNT_1_BIT;
		image.tiling = VK_IMAGE_TILING_OPTIMAL;
		image.format = m_vkformat;
		image.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		if (m_eUsage == USAGE_DEPTHSTENCIL)
		{
			image.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

			m_imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		}
		else if (m_eUsage == USAGE_RENDERTARGET)
		{
			image.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}
		VK_CHECK_RESULT(vkCreateImage(device->logicalDevice, &image, nullptr, &m_image));

		VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
		VkMemoryRequirements memReqs;
		vkGetImageMemoryRequirements(device->logicalDevice, m_image, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAlloc, nullptr, &m_deviceMemory));
		VK_CHECK_RESULT(vkBindImageMemory(device->logicalDevice, m_image, m_deviceMemory, 0));

		// Create image view
		// Textures are not directly accessed by the shaders and
		// are abstracted by image views containing additional
		// information and sub resource ranges
		VkImageViewCreateInfo view = vks::initializers::imageViewCreateInfo();
		view.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		view.format = m_vkformat;
		if (m_eUsage == USAGE_DEPTHSTENCIL)
		{
			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		}
		else
		{
			view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}
		// The subresource range describes the set of mip levels (and array layers) that can be accessed through this image view
		// It's possible to create multiple image views for a single image referring to different (and/or overlapping) ranges of the image
		view.subresourceRange.baseMipLevel = 0;
		view.subresourceRange.baseArrayLayer = 0;
		view.subresourceRange.layerCount = 6;
		// Linear tiling usually won't support mip maps
		// Only set mip map count if optimal tiling is used
		view.subresourceRange.levelCount = m_nMipLevels;
		// The view will be based on the texture's image
		view.image = m_image;
		VK_CHECK_RESULT(vkCreateImageView(device->logicalDevice, &view, nullptr, &m_view));

		return true;
	}

	bool VulkanTexture::RT_CreateTexture()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		m_eFormat = VulkanMapping::_getClosestSupportedPF(m_eFormat);
		m_vkformat = VulkanMapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			m_vkformat = GetSRGBFormat(m_vkformat);
		}

		// For shadow mapping we only need a depth attachment
		VkImageCreateInfo image = vks::initializers::imageCreateInfo();
		image.imageType = VK_IMAGE_TYPE_2D;
		image.extent.width = m_nWidth;
		image.extent.height = m_nHeight;
		image.extent.depth = 1;
		image.mipLevels = m_nMipLevels;
		image.arrayLayers = 1;
		image.samples = VK_SAMPLE_COUNT_1_BIT;
		image.tiling = VK_IMAGE_TILING_OPTIMAL;
		image.format = m_vkformat;	
		image.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		if (m_eUsage == USAGE_DEPTHSTENCIL)
		{
			image.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

			m_imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		}
		else if (m_eUsage == USAGE_RENDERTARGET)
		{
			image.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}
		VK_CHECK_RESULT(vkCreateImage(device->logicalDevice, &image, nullptr, &m_image));

		VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
		VkMemoryRequirements memReqs;
		vkGetImageMemoryRequirements(device->logicalDevice, m_image, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAlloc, nullptr, &m_deviceMemory));
		VK_CHECK_RESULT(vkBindImageMemory(device->logicalDevice, m_image, m_deviceMemory, 0));

		// Create image view
		// Textures are not directly accessed by the shaders and
		// are abstracted by image views containing additional
		// information and sub resource ranges
		VkImageViewCreateInfo view = vks::initializers::imageViewCreateInfo();
		view.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view.format = m_vkformat;
		if (m_eUsage == USAGE_DEPTHSTENCIL)
		{
			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		}
		else
		{
			view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}
		// The subresource range describes the set of mip levels (and array layers) that can be accessed through this image view
		// It's possible to create multiple image views for a single image referring to different (and/or overlapping) ranges of the image
		view.subresourceRange.baseMipLevel = 0;
		view.subresourceRange.baseArrayLayer = 0;
		view.subresourceRange.layerCount = 1;
		// Linear tiling usually won't support mip maps
		// Only set mip map count if optimal tiling is used
		view.subresourceRange.levelCount = m_nMipLevels/* (useStaging) ? texture.mipLevels : 1*/;
		// The view will be based on the texture's image
		view.image = m_image;
		VK_CHECK_RESULT(vkCreateImageView(device->logicalDevice, &view, nullptr, &m_view));

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

		uint32_t nFace = imageData.getNumFaces();
		
		for (uint32_t face = 0; face < nFace; face++)
		{
			for (uint32_t level = 0; level < m_nMipLevels; level++)
			{
				int width = m_nWidth >> level;
				int height = m_nHeight >> level;

				UINT size =  PixelUtil::getMemorySize(width, height, 1, m_eFormat);

				VkBufferImageCopy bufferCopyRegion = {};
				bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				bufferCopyRegion.imageSubresource.mipLevel = level;
				bufferCopyRegion.imageSubresource.baseArrayLayer = face;
				bufferCopyRegion.imageSubresource.layerCount = 1;
				bufferCopyRegion.imageExtent.width = static_cast<uint32_t>(width);
				bufferCopyRegion.imageExtent.height = static_cast<uint32_t>(height);
				bufferCopyRegion.imageExtent.depth = 1;
				bufferCopyRegion.bufferOffset = offset;

				bufferCopyRegions.push_back(bufferCopyRegion);

				offset += static_cast<uint32_t>(size);
			}
		}

		// Create optimal tiled target image
		VkImageCreateInfo imageCreateInfo = vks::initializers::imageCreateInfo();
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = m_vkformat;
		imageCreateInfo.mipLevels = m_nMipLevels;
		imageCreateInfo.arrayLayers = imageData.getNumFaces();
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
		subresourceRange.layerCount = imageData.getNumFaces();

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
		view.viewType = imageData.getNumFaces() == 1 ? VK_IMAGE_VIEW_TYPE_2D : VK_IMAGE_VIEW_TYPE_CUBE;
		view.format = m_vkformat;
		view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		// The subresource range describes the set of mip levels (and array layers) that can be accessed through this image view
		// It's possible to create multiple image views for a single image referring to different (and/or overlapping) ranges of the image
		view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		view.subresourceRange.baseMipLevel = 0;
		view.subresourceRange.baseArrayLayer = 0;
		view.subresourceRange.layerCount = imageData.getNumFaces();
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



