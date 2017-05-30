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
		
		IVulkanShaderResourceView* GetShaderResourceView() {return m_pVulkanShaderResourceView;}
		IVulkanShaderResourceView* GetShaderResourceViewSRGBNotEqual();

		IVulkanDepthStencilView* GetDepthStencilView();
		void SetDepthStencilView(IVulkanDepthStencilView* pView) {m_pDepthStencilView = pView;}	

		IVulkanRenderTargetView* GetRenderTargetView(int level = 0, int array_index = 0, int face = 0); 
		void SetRenderTargetView(IVulkanRenderTargetView* pView,int level = 0, int array_index = 0, int face = 0);
		
		IVulkanTexture2D* GetTexture2D() {return m_pVulkanTex2D;}
		void SetTexture2D(IVulkanTexture2D* pTex) {m_pVulkanTex2D = pTex;}

		virtual void	CopyTo(Texture* pDesc,int nOutFace, int nOutLevel, int nInFace,int nInlevel);

		virtual bool	SaveToFile(const char* pszPath);

	private:
		virtual	bool			RT_CreateCubeTexture();	

		virtual	bool			RT_CreateTexture();			

		virtual	bool			SetLevelData(int level, int face,const PixelBox& src);

		virtual bool			GenerateMipmaps();
		
	private:
		IVulkanTexture2D*			m_pVulkanTex2D;

		IVulkanShaderResourceView*	m_pVulkanShaderResourceView;	
		IVulkanShaderResourceView*	m_pVulkanShaderResourceViewSRGBNotEqual;

		IVulkanDepthStencilView*		m_pDepthStencilView;

		enum {MAX_MIP = 10};
		IVulkanRenderTargetView*		m_pRenderTargetView[MAX_MIP][6];

		DXGI_FORMAT					m_descFormat;
	};


}


