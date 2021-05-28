#include "RenderContext.h"

namespace ma
{
	void SceneContext::SetCamera(Camera* pCamera)
	{
		m_matViewProj.SetMatView( pCamera->GetMatView() );
		m_matViewProj.SetMatProj( pCamera->GetMatProj() );
		m_vEyeWordPos = pCamera->GetPosWS();

		m_fFar = pCamera->GetFarClip();
		m_fNear = pCamera->GetNearClip();
		m_frustum = pCamera->GetFrustum();
		m_fFov = pCamera->GetFov();
		m_fAspect = pCamera->GetAspect();
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
}