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

	private:
		virtual	bool			RT_CreateCubeTexture();	

		virtual	bool			RT_CreateTexture();			

		virtual bool			LoadFromImagData(const ImageData& imageData);

		virtual	bool			SetLevelData(int level, int face,const PixelBox& src);

		virtual bool			GenerateMipmaps();
		
	private:
		VkImage	m_image;
		VkImageLayout imageLayout;
		VkImageView view;
		VkDescriptorImageInfo descriptor;

		VkDeviceMemory deviceMemory;

		VkFormat format;
	};


}


