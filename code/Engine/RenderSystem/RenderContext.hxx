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
		m_fNear[0] = 0;
		m_fNear[1] = 0;
		m_fFar[0] = 0;
		m_fFar[1] = 0;

		m_pCurObject = NULL;
		m_pCurScene = NULL;
		m_pCurLight = NULL;
	}

	void RenderContext::SetCamera(Camera* pCamera)
	{
		//m_pCamera = pCamera;
		m_matViewProj[GetRenderSystem()->GetThreadList()].SetMatView( pCamera->GetMatView() );
		m_matViewProj[GetRenderSystem()->GetThreadList()].SetMatProj( pCamera->GetMatProj() );

		m_fFar[GetRenderSystem()->GetThreadList()] = pCamera->GetFarClip();
		m_fNear[GetRenderSystem()->GetThreadList()] = pCamera->GetNearClip();
	}

	void RenderContext::SetViewMatrix(const Matrix4& matView) 
	{
		m_matViewProj[GetRenderSystem()->GetThreadList()].SetMatView(matView);
	}

	const Matrix4& RenderContext::GetViewMatrix()  
	{
		return m_matViewProj[GetRenderSystem()->GetThreadList()].GetMatView();
	}

	void RenderContext::SetProjMatrix(const Matrix4& matProj) 
	{
		m_matViewProj[GetRenderSystem()->GetThreadList()].SetMatProj(matProj);
	}

	const Matrix4& RenderContext::GetProjMatrix()  
	{
		return m_matViewProj[GetRenderSystem()->GetThreadList()].GetMatProj();
	}

	const Matrix4& RenderContext::GetViewProjMatrix()  
	{
		return m_matViewProj[GetRenderSystem()->GetThreadList()].GetMatViewProj();
	}

	float RenderContext::GetNearClip() 
	{
		return m_fNear[GetRenderSystem()->GetThreadList()];
	}

	float RenderContext::GetFarClip() 
	{
		return m_fFar[GetRenderSystem()->GetThreadList()];
	}

}