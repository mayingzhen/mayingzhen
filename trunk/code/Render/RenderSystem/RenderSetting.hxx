#include "RenderSetting.h"

namespace ma
{
	static RenderSetting* gpRenderSetting = NULL;

	RenderSetting* GetRenderSetting()
	{
		return gpRenderSetting;
	}

	void SetRenderSetting(RenderSetting* pRenderSetting)
	{
		gpRenderSetting = pRenderSetting;
	}

	RenderSetting::RenderSetting()
	{
		m_bShadow = true; 
		m_bDefferLight = true;
		m_bRenderThread = true;
		m_bPartcleThread = true;
		m_cClearClor = Color(0,45.0f / 255.0f,50.0f/255.0f,170.0f/255.0f);
		m_fShadowMapSize = 2048.0f;
		m_bHWShadowMap = false;
		m_bIsHDRRending = true;
	}

	void RenderSetting::Init()
	{
		m_bHWShadowMap = GetRenderDevice()->CheckTextureFormat(FMT_D24S8,USAGE_DEPTHSTENCIL);
	}
}