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
		m_matViewProj.SetMatView( pCamera->GetMatView() );
		m_matViewProj.SetMatProj( pCamera->GetMatProj() );
		m_vEyeWordPos = pCamera->GetPosWS();

		m_fFar = pCamera->GetFarClip();
		m_fNear = pCamera->GetNearClip();
	}

	const Matrix4& RenderContext::GetViewMatrix() const
	{
		return m_matViewProj.GetMatView();
	}

	const Matrix4& RenderContext::GetViewMatrixInv() const
	{
		return m_matViewProj.GetMatViewInv();
	}

	const Matrix4& RenderContext::GetProjMatrix() const 
	{
		return m_matViewProj.GetMatProj();
	}

	const Matrix4& RenderContext::GetViewProjMatrix() const
	{
		return m_matViewProj.GetMatViewProj();
	}

	const Vector3& RenderContext::GetEyeWorldPos() const
	{
		return m_vEyeWordPos;
	}

	float RenderContext::GetNearClip() 
	{
		return m_fNear;
	}

	float RenderContext::GetFarClip() 
	{
		return m_fFar;
	}

	Vector3	RenderContext::GetAmbientColor() const
	{
		//ASSERT(false);
		return Vector3::ZERO;
// 		if (m_pCurScene == NULL)
// 			return Vector3::ZERO;
// 
// 		return m_pCurScene->GetAmbientColor();
	}

	Vector3	RenderContext::GetDirLightColor() const
	{
		//ASSERT(false);
		return Vector3::UNIT_SCALE;
// 		if (m_pCurScene == NULL)
// 			return Vector3::ZERO;
// 
// 		ColourValue color = m_pCurScene->GetMainDirLight()->GetLightColor();
// 		return Vector3(color.r,color.g,color.b);
	}

	Vector3	RenderContext::GetDirLightDir() const
	{
		//ASSERT(false);
		return Vector3::UNIT_SCALE;

// 		if (m_pCurScene == NULL)
// 			return Vector3::ZERO;
// 
// 		return m_pCurScene->GetMainDirLight()->GetSceneNode()->GetForward();
	}

	void RenderContext::AddLight(Light* pLight)
	{

	}

	void RenderContext::Clear()
	{

	}

}