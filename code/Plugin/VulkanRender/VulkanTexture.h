#pragma once

#include "Engine/Material/Texture.h"

namespace ma
{
	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture();

		VulkanTexture(int nWidth,int nHeight,uint32_t nMipMap,PixelFormat format,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType);

		~VulkanTexture();
	
		virtual void	CopyTo(Texture* pDesc,int nOutFace, int nOutLevel, int nInFace,int nInlevel);

		virtual bool	SaveToFile(const char* pszPath);

		VkImageView		GetImageView(int nInlevel, int nFace) { return m_view[nInlevel][nFace]; }

	private:
		virtual	bool	RT_CreateCubeTexture();	

		virtual	bool	RT_CreateTexture();			

		virtual bool	LoadFromImagData(const ImageData& imageData);

		virtual bool	GenerateMipmaps();
		
	public:
		VkImage			m_image = 0;
		VkImageLayout	m_imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		enum { MAX_MIP = 10 };
		VkImageView		m_view[MAX_MIP][6] = { 0 };

		VkDeviceMemory	m_deviceMemory;

		VkFormat		m_vkformat;
	};

}

