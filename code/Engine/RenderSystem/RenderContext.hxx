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

		m_pCurScene = NULL;
		m_pCurLight = NULL;
	}

	void RenderContext::SetCamera(Camera* pCamera)
	{
		m_matViewProj[GetRenderSystem()->CurThreadFill()].SetMatView( pCamera->GetMatView() );
		m_matViewProj[GetRenderSystem()->CurThreadFill()].SetMatProj( pCamera->GetMatProj() );
		m_vEyeWordPos[GetRenderSystem()->CurThreadFill()] = pCamera->GetPosWS();

		m_fFar[GetRenderSystem()->CurThreadFill()] = pCamera->GetFarClip();
		m_fNear[GetRenderSystem()->CurThreadFill()] = pCamera->GetNearClip();
	}

	void RenderContext::SetViewMatrix(const Matrix4& matView) 
	{
		m_matViewProj[GetRenderSystem()->CurThreadFill()].SetMatView(matView);
	}

	const Matrix4& RenderContext::GetViewMatrix() const
	{
		return m_matViewProj[GetRenderSystem()->CurThreadProcess()].GetMatView();
	}

	const Matrix4& RenderContext::GetViewMatrixInv() const
	{
		return m_matViewProj[GetRenderSystem()->CurThreadProcess()].GetMatViewInv();
	}

	void RenderContext::SetProjMatrix(const Matrix4& matProj) 
	{
		m_matViewProj[GetRenderSystem()->CurThreadFill()].SetMatProj(matProj);
	}

	const Matrix4& RenderContext::GetProjMatrix() const 
	{
		return m_matViewProj[GetRenderSystem()->CurThreadProcess()].GetMatProj();
	}

	const Matrix4& RenderContext::GetViewProjMatrix() const
	{
		return m_matViewProj[GetRenderSystem()->CurThreadProcess()].GetMatViewProj();
	}

	const Vector3& RenderContext::GetEyeWorldPos() const
	{
		return m_vEyeWordPos[GetRenderSystem()->CurThreadProcess()];
	}

	float RenderContext::GetNearClip() 
	{
		return m_fNear[GetRenderSystem()->CurThreadProcess()];
	}

	float RenderContext::GetFarClip() 
	{
		return m_fFar[GetRenderSystem()->CurThreadProcess()];
	}

	Vector3	RenderContext::GetAmbientColor() const
	{
		if (m_pCurScene == NULL)
			return Vector3::ZERO;

		return m_pCurScene->GetAmbientColor();
	}

	Vector3	RenderContext::GetDirLightColor() const
	{
		if (m_pCurScene == NULL)
			return Vector3::ZERO;

		ColourValue color = m_pCurScene->GetMainDirLight()->GetLightColor();
		return Vector3(color.r,color.g,color.b);
	}

	Vector3	RenderContext::GetDirLightDir() const
	{
		if (m_pCurScene == NULL)
			return Vector3::ZERO;

		return m_pCurScene->GetMainDirLight()->GetSceneNode()->GetForward();
	}


}