#include "RenderContext.h"

namespace ma
{
	RenderContext* gpRenderContext = NULL;

	RenderContext* GetRenderContext()
	{
		return gpRenderContext;
	}

	void SetRenderContext(RenderContext* pRenderContext)
	{
		gpRenderContext = pRenderContext;
	}

	RenderContext::RenderContext()
	{
	}

	void RenderContext::SetCamera(Camera* pCamera)
	{
		//m_pCamera = pCamera;
		m_matViewProj.SetMatView( pCamera->GetMatView() );
		m_matViewProj.SetMatProj( pCamera->GetMatProj() );

		m_fFar = pCamera->GetFarClip();
		m_fNear = pCamera->GetNearClip();
	}

}