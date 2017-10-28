#pragma once

#include "Engine/Material/Texture.h"

namespace ma
{
	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture();

		VulkanTexture(int nWidth,int nHeight,uint32 nMipMap,PixelFormat format,bool bTypeLess,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType);

		~VulkanTexture();
	
		virtual void	CopyTo(Texture* pDesc,int nOutFace, int nOutLevel, int nInFace,int nInlevel);

		virtual bool	SaveToFile(const char* pszPath);

		void			SetImageView(VkImageView view) { m_view = view; }

	private:
		virtual	bool			RT_CreateCubeTexture();	

		virtual	bool			RT_CreateTexture();			

		virtual bool			LoadFromImagData(const ImageData& imageData);

		virtual bool			GenerateMipmaps();
		
	public:
		VkImage	m_image = 0;
		VkImageLayout m_imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImageView m_view  = 0;

		VkDeviceMemory m_deviceMemory;

		VkFormat m_vkformat;
	};


}


