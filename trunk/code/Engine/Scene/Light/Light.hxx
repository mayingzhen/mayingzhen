#include "Light.h"
#include "LightSystem.h"


namespace ma
{
	IMPL_OBJECT(Light,RenderComponent)

	Light::Light()
	{
		m_bCreateShadow = false;
		m_cLightColor = ColourValue::White;
		m_fLightIntensity = 1.0f;
	
		GetLightSystem()->AddLight(this);
	}

	Light::~Light()
	{
		GetLightSystem()->RemoveLight(this);
	}

	void Light::GetDepthBiasParams(float& fConstantBias, float& fSlopeScaleBias) const
	{
		fConstantBias = m_fConstantBias;
		fSlopeScaleBias = m_fSlopeScaledBias;
	}

	void Light::SetDepthBiasParams(float fConstantBias, float fSlopeScaleBias)
	{
		m_fConstantBias = fConstantBias;
		m_fSlopeScaledBias = fSlopeScaleBias;
	}

	IMPL_OBJECT(PointLight,Light)
	
	Vector3 PointLight::GetPos()
	{
		return m_pSceneNode->GetTransformWS().m_vPos;
	}

	IMPL_OBJECT(SpotLight,Light)

	
	IMPL_OBJECT(DirectonalLight,Light)

	DirectonalLight::DirectonalLight()
	{
		m_eLightType = LIGHT_DIRECTIONAL;
	}

	Vector3 DirectonalLight::GetDirection()
	{
		return m_pSceneNode->GetForward();
	}

	void DirectonalLight::SetCreateShadow(bool bCreateShaow)
	{
		if ( !GetRenderSetting()->m_bShadow )
			return;

		m_bCreateShadow = bCreateShaow;

// 		if (m_bCreateShadow && m_arrShadowFrustum.empty())
// 		{
// 			for (UINT i = 0; i < NUM_PSSM; ++i)
// 			{
// 				ShadowMapFrustum* pSMF = new ShadowMapFrustum(this,i);
// 				m_arrShadowFrustum.push_back(pSMF);
// 			}
// 		}
	}

	const Vector4& DirectonalLight::GetSplitPos() 
	{
		return m_curSplitPos;
	}


	void DirectonalLight::Show(Camera* pCamera)
	{
		if (!m_bEnable)
			return;

		m_nCurSplitCount = 0;
		memset(&m_curSplitPos,0,sizeof(m_curSplitPos));

		float fNearSplit = pCamera->GetNearClip();
		float fFarSplit;

		while (m_nCurSplitCount < m_nMaxSplitCount)
		{
			// If split is completely beyond camera far clip, we are done
			if (fNearSplit > pCamera->GetFarClip())
				break;

			fFarSplit = min(pCamera->GetFarClip(), m_SplitPosParam[m_nCurSplitCount]);
			if (fFarSplit <= fNearSplit)
				break;

			// Setup the shadow camera for the split
			m_curSplitPos[m_nCurSplitCount] = fFarSplit;

			m_SpitFrustum[m_nCurSplitCount].Update(pCamera,fNearSplit,fFarSplit);

			m_matShadow[m_nCurSplitCount] =  m_SpitFrustum[m_nCurSplitCount].GetTexAdjustMatrix() * m_SpitFrustum[m_nCurSplitCount].GetShadowMatrix();

			if (m_eShadowBleurLevel == SHADOW_JITTERIN)
			{
				Vector3 vViewPosLS = m_SpitFrustum[m_nCurSplitCount].GetLightViewMatrix() * pCamera->GetSceneNode()->GetPosWS();
				Vector3 vViewVectLS = m_SpitFrustum[m_nCurSplitCount].GetLightViewMatrix() * pCamera->GetSceneNode()->GetForward();
				m_viewPosVecLS[m_nCurSplitCount] = Vector4(vViewPosLS.x,vViewPosLS.y,vViewVectLS.x,vViewVectLS.y);
			}

			fNearSplit = fFarSplit;
			++m_nCurSplitCount;
		}

		// Upadte shadowDetphFade
		float shadowRange = fFarSplit;
		float fadeStart = m_fShadowFadeStart * shadowRange;
		float fadeEnd = shadowRange;
		float fadeRange = fadeEnd - fadeStart;

		m_vShadowDepthFade = Vector4(0,0,fadeStart,1.0f / fadeRange);
	}

}



