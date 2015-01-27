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
		m_matViewProj[GetRenderSystem()->CurThreadFill()].SetMatView( pCamera->GetMatView() );
		m_matViewProj[GetRenderSystem()->CurThreadFill()].SetMatProj( pCamera->GetMatProj() );

		m_fFar[GetRenderSystem()->CurThreadFill()] = pCamera->GetFarClip();
		m_fNear[GetRenderSystem()->CurThreadFill()] = pCamera->GetNearClip();
	}

	void RenderContext::SetViewMatrix(const Matrix4& matView) 
	{
		m_matViewProj[GetRenderSystem()->CurThreadFill()].SetMatView(matView);
	}

	const Matrix4& RenderContext::GetViewMatrix()  
	{
		return m_matViewProj[GetRenderSystem()->CurThreadProcess()].GetMatView();
	}

	void RenderContext::SetProjMatrix(const Matrix4& matProj) 
	{
		m_matViewProj[GetRenderSystem()->CurThreadFill()].SetMatProj(matProj);
	}

	const Matrix4& RenderContext::GetProjMatrix()  
	{
		return m_matViewProj[GetRenderSystem()->CurThreadProcess()].GetMatProj();
	}

	const Matrix4& RenderContext::GetViewProjMatrix()  
	{
		return m_matViewProj[GetRenderSystem()->CurThreadProcess()].GetMatViewProj();
	}

	float RenderContext::GetNearClip() 
	{
		return m_fNear[GetRenderSystem()->CurThreadProcess()];
	}

	float RenderContext::GetFarClip() 
	{
		return m_fFar[GetRenderSystem()->CurThreadProcess()];
	}

}