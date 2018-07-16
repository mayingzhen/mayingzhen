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

		VkImageView		GetShaderView() { return m_shaderView; }

		VkImageView		GetRenderTargetView(int nInlevel, int nFace) { return m_rtView[nInlevel][nFace]; }

	private:
		virtual	bool	RT_CreateCubeTexture();	

		virtual	bool	RT_CreateTexture();			

		virtual bool	LoadFromImagData(const ImageData& imageData);

		virtual bool	GenerateMipmaps();

		void			CreateShaderView();

		void			CreateRenderTargetView();
		
	public:
		VkImage			m_image = 0;
		VkImageLayout	m_imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		enum { MAX_MIP = 10 };
		VkImageView		m_rtView[MAX_MIP][6] = { 0 };

		VkImageView		m_shaderView = 0;

		VkDeviceMemory	m_deviceMemory;

		VkFormat		m_vkformat;
	};

}

