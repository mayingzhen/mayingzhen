#include "Light.h"

namespace ma
{
	Light::Light()
	{
		m_eLightType = LIGHT_POINT;
		m_vUpVector = Vector3(0, 1, 0);
		m_fAspectRatio = 1.0f;
		m_vTarget = Vector3(0, 0, 0);
		m_fNearClip = 0.1f;
		m_fFarClip = 3000.0f;
		m_fRadius = 1.0f;
		m_fFOV =  ToRadian(45.0f);
		m_vLightDiffuse = Vector3(0.9f,0.9f,0.9f);
		m_bCreateShadow = false;
	}

	void Light::LookAt(const Vector3* pEye,const Vector3* pAt,const Vector3* pUp)
	{
		if (pAt)
		{
			m_vTarget = *pAt;
		}
		if (pUp)
		{
			m_vUpVector = *pUp;
		}

		// view matrix
		MatrixLookAtLH(&m_mView, pEye, &m_vTarget, &m_vUpVector);
		
		// projection matrix
		if(m_eLightType == LIGHT_SPOT)
		{
			GetRenderDevice()->MakePerspectiveMatrix(&m_mProj, m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
		}
		else if (m_eLightType == LIGHT_DIRECTIONAL)
		{
			//GetRenderDevice()->MakeOrthoMatrix(&m_mProj, 1, 1, 0, 1);
		}
	}

// 	void Light::UpdateShadowMapFrustum()
// 	{
// 		if (!m_bCreateShadow)
// 			return;
// 
// 		for (UINT i = 0; i < m_arrSMF.size(); ++i)
// 		{
// 			m_arrSMF[i]->Update();
// 		}
// 	}

// 	void Light::ShadowDepthPass()
// 	{
// 		if (!m_bCreateShadow)
// 			return;
// 
// 		for (UINT i = 0; i < m_arrSMF.size(); ++i)
// 		{
// 			m_arrSMF[i]->ShadowDepthPass();
// 		}
// 	}

}



