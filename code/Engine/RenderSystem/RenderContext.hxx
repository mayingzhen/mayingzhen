#include "RenderContext.h"

namespace ma
{
	SceneContext* gpRenderContext = NULL;

	SceneContext* GetSceneContext()
	{
		return gpRenderContext;
	}

	void SetSceneContext(SceneContext* pRenderContext)
	{
		gpRenderContext = pRenderContext;
	}

	SceneContext::SceneContext()
	{

	}

	void SceneContext::SetCamera(Camera* pCamera)
	{
		m_matViewProj.SetMatView( pCamera->GetMatView() );
		m_matViewProj.SetMatProj( pCamera->GetMatProj() );
		m_vEyeWordPos = pCamera->GetPosWS();

		m_fFar = pCamera->GetFarClip();
		m_fNear = pCamera->GetNearClip();
	}

	void SceneContext::SetLightViewProj(const Matrix4& matLightViewProj)
	{
		m_matLightViewProj = matLightViewProj;
	}

	void SceneContext::SetMainLight(Light* pMianLight, ColourValue cAmbientColor)
	{
		if (pMianLight)
		{
			m_cMianLightColor = pMianLight->GetLightColor();
			m_vMainLightDir = -pMianLight->GetSceneNode()->GetForward().normalisedCopy();
		}

		m_cAmbientColor = cAmbientColor;
	}

	const Matrix4& SceneContext::GetViewMatrix() const
	{
		return m_matViewProj.GetMatView();
	}

	const Matrix4& SceneContext::GetViewMatrixInv() const
	{
		return m_matViewProj.GetMatViewInv();
	}

	const Matrix4& SceneContext::GetProjMatrix() const 
	{
		return m_matViewProj.GetMatProj();
	}

	const Matrix4& SceneContext::GetViewProjMatrix() const
	{
		return m_matViewProj.GetMatViewProj();
	}

	const Vector3& SceneContext::GetEyeWorldPos() const
	{
		return m_vEyeWordPos;
	}

	float SceneContext::GetNearClip() 
	{
		return m_fNear;
	}

	float SceneContext::GetFarClip() 
	{
		return m_fFar;
	}

	Vector3	SceneContext::GetAmbientColor() const
	{
		return Vector3(m_cAmbientColor.r, m_cAmbientColor.g, m_cAmbientColor.b);
	}

	Vector3	SceneContext::GetDirLightColor() const
	{
		return Vector3(m_cMianLightColor.r, m_cMianLightColor.g, m_cMianLightColor.b);
	}

	Vector3	SceneContext::GetDirLightDir() const
	{
		return m_vMainLightDir;
	}

}