#include "Light.h"

namespace ma
{
	Light::Light()
	{
		m_bCreateShadow = false;
		m_cLightColor = Vector4(1,1,1,1);
	}


	void SpotLight::LookAt(const Vector3& vEye,const Vector3& vAt,const Vector3& vUp)
	{
		m_vTarget = vAt;
		m_vUpVector = vUp;

		// view matrix
		MatrixLookAtLH(&m_mView, &vEye, &vAt, &vUp);

		// MatWorld
		Matrix4x4 matInv;
		MatrixInverse(&matInv,NULL,&m_mView);
		TransformFromMatrix(&m_tsfWS,&matInv);
		
		// projection matrix
		GetRenderDevice()->MakePerspectiveMatrix(&m_mProj, m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
	}



}



