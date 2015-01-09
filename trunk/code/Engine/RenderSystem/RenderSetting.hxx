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
		m_bShadow = false; 
		m_bDefferLight = false;
		m_bRenderThread = false;
		m_bPartcleThread = true;
		m_cClearClor = ColourValue::Black;
		m_fShadowMapSize = 2048.0f;
		m_bIsHDRRending = false;
	}
}
