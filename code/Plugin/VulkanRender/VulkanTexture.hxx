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

	VulkanTexture::VulkanTexture(int nWidth,int nHeight,uint32_t nMipMap,PixelFormat format,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType)
		:Texture(nWidth,nHeight,nMipMap,format,bSRGB,eUsage,eType)
	{
	}

	VulkanTexture::~VulkanTexture()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		for (uint32_t i = 0; i < MAX_MIP; ++i)
		{
			for (uint32_t j = 0; j < 6; ++j)
			{
				if (m_rtView[i][j] == 0)
					continue;

				vkDestroyImageView(device->logicalDevice, m_rtView[i][j], nullptr);
			}
		}
		
		vkDestroyImage(device->logicalDevice, m_image, nullptr);
		vkFreeMemory(device->logicalDevice, m_deviceMemory, nullptr);
	}

	bool VulkanTexture::GenerateMipmaps()
	{	
		return true;
	}

	VkFormat GetSRGBFormat(VkFormat format)
	{
		if (format == VK_FORMAT_R8G8B8A8_UNORM)
			return VK_FORMAT_R8G8B8A8_SRGB;
		else if (format == VK_FORMAT_B8G8R8A8_UNORM)
			return VK_FORMAT_B8G8R8A8_SRGB;
		else if (format == VK_FORMAT_BC1_RGB_UNORM_BLOCK)
			return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
		else if (format == VK_FORMAT_BC1_RGBA_UNORM_BLOCK)
			return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
		else if (format == VK_FORMAT_BC2_UNORM_BLOCK)
			return VK_FORMAT_BC2_SRGB_BLOCK;
		else if (format == VK_FORMAT_BC3_UNORM_BLOCK)
			return VK_FORMAT_BC3_SRGB_BLOCK;
		else if (format == VK_FORMAT_BC4_UNORM_BLOCK)
			return VK_FORMAT_BC4_SNORM_BLOCK;
		else if (format == VK_FORMAT_BC5_UNORM_BLOCK)
			return VK_FORMAT_BC5_SNORM_BLOCK;
		else if (format == VK_FORMAT_BC6H_UFLOAT_BLOCK)
			return VK_FORMAT_BC6H_SFLOAT_BLOCK;
		else if (format == VK_FORMAT_BC7_UNORM_BLOCK)
			return VK_FORMAT_BC7_SRGB_BLOCK;
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
		if (m_eUsage == TEXTURE_USAGE::USAGE_DEPTHSTENCIL)
		{
			image.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

			m_imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		}
		else if (m_eUsage == TEXTURE_USAGE::USAGE_RENDERTARGET)
		{
			image.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			//m_imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		image.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		VK_CHECK_RESULT(vkCreateImage(device->logicalDevice, &image, nullptr, &m_image));

		VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
		VkMemoryRequirements memReqs;
		vkGetImageMemoryRequirements(device->logicalDevice, m_image, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAlloc, nullptr, &m_deviceMemory));
		VK_CHECK_RESULT(vkBindImageMemory(device->logicalDevice, m_image, m_deviceMemory, 0));

		CreateRenderTargetView();

		CreateShaderView();

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
		image.mipLevels = std::max<uint32_t>(m_nMipLevels,1);
		image.arrayLayers = 1;
		image.samples = VK_SAMPLE_COUNT_1_BIT;
		image.tiling = VK_IMAGE_TILING_OPTIMAL;
		image.format = m_vkformat;	
		image.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		if (m_eUsage == TEXTURE_USAGE::USAGE_DEPTHSTENCIL)
		{
			image.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

			m_imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		}
		else if (m_eUsage == TEXTURE_USAGE::USAGE_RENDERTARGET)
		{
			image.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			//m_imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		//image.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		VK_CHECK_RESULT(vkCreateImage(device->logicalDevice, &image, nullptr, &m_image));

		VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
		VkMemoryRequirements memReqs;
		vkGetImageMemoryRequirements(device->logicalDevice, m_image, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAlloc, nullptr, &m_deviceMemory));
		VK_CHECK_RESULT(vkBindImageMemory(device->logicalDevice, m_image, m_deviceMemory, 0));

		CreateRenderTargetView();
			
		CreateShaderView();

		return true;
	}

	void VulkanTexture::CreateShaderView()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		// Create image view
		// Textures are not directly accessed by the shaders and
		// are abstracted by image views containing additional
		// information and sub resource ranges
		VkImageViewCreateInfo view = vks::initializers::imageViewCreateInfo();
		view.viewType = m_eType == TEXTYPE_CUBE ? VK_IMAGE_VIEW_TYPE_CUBE : VK_IMAGE_VIEW_TYPE_2D;
		view.format = m_vkformat;
		if (m_eUsage == TEXTURE_USAGE::USAGE_DEPTHSTENCIL)
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
		view.subresourceRange.layerCount = m_eType == TEXTYPE_CUBE ? 6 : 1;
		// Linear tiling usually won't support mip maps
		// Only set mip map count if optimal tiling is used
		view.subresourceRange.levelCount = m_nMipLevels/* (useStaging) ? texture.mipLevels : 1*/;
		// The view will be based on the texture's image
		view.image = m_image;
		VK_CHECK_RESULT(vkCreateImageView(device->logicalDevice, &view, nullptr, &m_shaderView));
	}

	void VulkanTexture::CreateRenderTargetView()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		// Create image view
		// Textures are not directly accessed by the shaders and
		// are abstracted by image views containing additional
		// information and sub resource ranges
		VkImageViewCreateInfo view = vks::initializers::imageViewCreateInfo();
		view.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view.format = m_vkformat;
		if (m_eUsage == TEXTURE_USAGE::USAGE_DEPTHSTENCIL)
		{
			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		else
		{
			view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		// The subresource range describes the set of mip levels (and array layers) that can be accessed through this image view
		// It's possible to create multiple image views for a single image referring to different (and/or overlapping) ranges of the image
		uint32_t nFaceCount = GetType() == TEXTYPE_CUBE ? 6 : 1;
		for (uint32_t nMip = 0; nMip < m_nMipLevels; ++nMip)
		{
			for (uint32_t nFace = 0; nFace < nFaceCount; ++nFace)
			{
				view.subresourceRange.baseMipLevel = nMip;
				view.subresourceRange.baseArrayLayer = nFace;
				view.subresourceRange.layerCount = 1;
				// Linear tiling usually won't support mip maps
				// Only set mip map count if optimal tiling is used
				view.subresourceRange.levelCount = 1;
				// The view will be based on the texture's image
				view.image = m_image;
				VK_CHECK_RESULT(vkCreateImageView(device->logicalDevice, &view, nullptr, &m_rtView[nMip][nFace]));
			}
		}


		// SetLoayout
// 		VkCommandBuffer Cmd = device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
// 
// 		VkImageSubresourceRange subresourceRange = {};
// 		if (m_eUsage == USAGE_DEPTHSTENCIL)
// 		{
// 			subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
// 		}
// 		else
// 		{
// 			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		}
// 		subresourceRange.baseMipLevel = 0;
// 		subresourceRange.levelCount = m_nMipLevels;
// 		subresourceRange.layerCount = nFaceCount;
// 
// 		// Change texture image layout to shader read after all mip levels have been copied
// 		m_imageLayout = VK_IMAGE_LAYOUT_GENERAL;
// 		vks::tools::setImageLayout(
// 			Cmd,
// 			m_image,
// 			VK_IMAGE_LAYOUT_UNDEFINED,
// 			m_imageLayout,
// 			subresourceRange);
// 
// 		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();
// 
// 		device->flushCommandBuffer(Cmd, pRender->m_queue);
	}

	bool VulkanTexture::LoadFromImagData(const ImageData& imageData)
	{
		// Get source image format and adjust if required
		m_eFormat = VulkanMapping::_getClosestSupportedPF(imageData.m_eFormat);
		if (m_eFormat != imageData.m_eFormat)
		{
			ImageData& tem = const_cast<ImageData&>(imageData);
			tem.bulkPixelConversion(m_eFormat);
		}
		m_vkformat = VulkanMapping::_getPF(m_eFormat);
		if (m_bSRGB)
		{
			m_vkformat = GetSRGBFormat(m_vkformat);
		}

		// The custom mipmaps in the image have priority over everything
		//size_t imageMips = imageData.num_mipmaps;

		bool bAutoMipMap = m_bMipMap;

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

				uint32_t size =  PixelUtil::getMemorySize(width, height, 1, m_eFormat);

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
		imageCreateInfo.extent = { (uint32_t)m_nWidth, (uint32_t)m_nHeight, 1 };
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

		CreateShaderView();

		m_eResState = ResReady;

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
		bool supportsBlit = true;

		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		// Check blit support for source and destination
		VkFormatProperties formatProps;

		// Check if the device supports blitting from optimal images (the swapchain images are in optimal format)
		vkGetPhysicalDeviceFormatProperties(device->physicalDevice, pRender->m_swapChain.colorFormat, &formatProps);
		if (!(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT))
		{
			std::cerr << "Device does not support blitting from optimal tiled images, using copy instead of blit!" << std::endl;
			supportsBlit = false;
		}

		// Check if the device supports blitting to linear images 
		vkGetPhysicalDeviceFormatProperties(device->physicalDevice, m_vkformat, &formatProps);
		if (!(formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT)) 
		{
			std::cerr << "Device does not support blitting to linear tiled images, using copy instead of blit!" << std::endl;
			supportsBlit = false;
		}

		// Source for the copy is the last rendered swapchain image
		VkImage srcImage = m_image;//swapChain.images[currentBuffer];

		uint32_t nLayerCount = m_eType == TEXTYPE_CUBE ? 6 : 1;

		// Create the linear tiled destination image to copy to and to read the memory from
		VkImageCreateInfo imageCreateCI(vks::initializers::imageCreateInfo());
		imageCreateCI.imageType = VK_IMAGE_TYPE_2D;
		// Note that vkCmdBlitImage (if supported) will also do format conversions if the swapchain color format would differ
		imageCreateCI.format = m_vkformat;
		imageCreateCI.extent.width = this->GetWidth();
		imageCreateCI.extent.height = this->GetHeight();
		imageCreateCI.extent.depth = 1;
		imageCreateCI.arrayLayers = nLayerCount;
		imageCreateCI.mipLevels = m_nMipLevels;
		imageCreateCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateCI.tiling = VK_IMAGE_TILING_LINEAR;
		imageCreateCI.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		// Create the image
		VkImage dstImage;
		VK_CHECK_RESULT(vkCreateImage(device->logicalDevice, &imageCreateCI, nullptr, &dstImage));
		// Create memory to back up the image
		VkMemoryRequirements memRequirements;
		VkMemoryAllocateInfo memAllocInfo(vks::initializers::memoryAllocateInfo());
		VkDeviceMemory dstImageMemory;
		vkGetImageMemoryRequirements(device->logicalDevice, dstImage, &memRequirements);
		memAllocInfo.allocationSize = memRequirements.size;
		// Memory must be host visible to copy from
		memAllocInfo.memoryTypeIndex = device->getMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAllocInfo, nullptr, &dstImageMemory));
		VK_CHECK_RESULT(vkBindImageMemory(device->logicalDevice, dstImage, dstImageMemory, 0));

		// Do the actual blit from the swapchain image to our host visible destination image
		VkCommandBuffer copyCmd = device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

		VkImageMemoryBarrier imageMemoryBarrier = vks::initializers::imageMemoryBarrier();

		// Transition destination image to transfer destination layout
		vks::tools::insertImageMemoryBarrier(
			copyCmd,
			dstImage,
			0,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

		// Transition swapchain image from present to transfer source layout
		vks::tools::insertImageMemoryBarrier(
			copyCmd,
			srcImage,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_ACCESS_TRANSFER_READ_BIT,
			m_imageLayout,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

		// If source and destination support blit we'll blit as this also does automatic format conversion (e.g. from BGR to RGB)
		if (supportsBlit)
		{
			// Define the region to blit (we will blit the whole swapchain image)
			VkOffset3D blitSize;
			blitSize.x = this->GetWidth();
			blitSize.y = this->GetHeight();
			blitSize.z = 1;
			VkImageBlit imageBlitRegion{};
			imageBlitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlitRegion.srcSubresource.layerCount = nLayerCount;
			imageBlitRegion.srcOffsets[1] = blitSize;
			imageBlitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlitRegion.dstSubresource.layerCount = nLayerCount;
			imageBlitRegion.dstOffsets[1] = blitSize;

			// Issue the blit command
			vkCmdBlitImage(
				copyCmd,
				srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&imageBlitRegion,
				VK_FILTER_NEAREST);
		}
		else
		{
			// Otherwise use image copy (requires us to manually flip components)
			VkImageCopy imageCopyRegion{};
			imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.srcSubresource.layerCount = nLayerCount;
			imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.dstSubresource.layerCount = nLayerCount;
			imageCopyRegion.extent.width = this->GetWidth();
			imageCopyRegion.extent.height = this->GetHeight();
			imageCopyRegion.extent.depth = 1;

			// Issue the copy command
			vkCmdCopyImage(
				copyCmd,
				srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&imageCopyRegion);
		}

		// Transition destination image to general layout, which is the required layout for mapping the image memory later on
		vks::tools::insertImageMemoryBarrier(
			copyCmd,
			dstImage,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_GENERAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

		// Transition back the swap chain image after the blit is done
// 		vks::tools::insertImageMemoryBarrier(
// 			copyCmd,
// 			srcImage,
// 			VK_ACCESS_TRANSFER_READ_BIT,
// 			VK_ACCESS_MEMORY_READ_BIT,
// 			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
// 			m_imageLayout,
// 			VK_PIPELINE_STAGE_TRANSFER_BIT,
// 			VK_PIPELINE_STAGE_TRANSFER_BIT,
// 			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

		device->flushCommandBuffer(copyCmd, pRender->m_queue);

		// Get layout of the image (including row pitch)
		VkImageSubresource subResource{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 0 };
		VkSubresourceLayout subResourceLayout;
		vkGetImageSubresourceLayout(device->logicalDevice, dstImage, &subResource, &subResourceLayout);

		// Map image memory so we can start copying from it
		const char* data;
		vkMapMemory(device->logicalDevice, dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void**)&data);
		data += subResourceLayout.offset;

		std::ofstream file(pszPath, std::ios::out | std::ios::binary);

		// ppm header
		file << "P6\n" << this->GetWidth() << "\n" << this->GetHeight() << "\n" << 255 << "\n";

		// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'll have to manually swizzle color components
		bool colorSwizzle = false;
		// Check if source is BGR 
		// Note: Not complete, only contains most common and basic BGR surface formats for demonstation purposes
		if (!supportsBlit)
		{
			std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
			colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), pRender->m_swapChain.colorFormat) != formatsBGR.end());
		}

		// ppm binary pixel data
		for (int y = 0; y < this->GetHeight(); y++)
		{
			unsigned int *row = (unsigned int*)data;
			for (int x = 0; x < this->GetWidth(); x++)
			{
				if (colorSwizzle)
				{
					file.write((char*)row + 2, 1);
					file.write((char*)row + 1, 1);
					file.write((char*)row, 1);
				}
				else
				{
					file.write((char*)row, 3);
				}
				row++;
			}
			data += subResourceLayout.rowPitch;
		}
		file.close();

		std::cout << "Screenshot saved to disk" << std::endl;

		// Clean up resources
		vkUnmapMemory(device->logicalDevice, dstImageMemory);
		vkFreeMemory(device->logicalDevice, dstImageMemory, nullptr);
		vkDestroyImage(device->logicalDevice, dstImage, nullptr);

		return false;
	}
}


