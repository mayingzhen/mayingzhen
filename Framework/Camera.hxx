#include "Framework/Camera.h"

namespace ma
{
	Camera::Camera()
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::LookAt(const xmVector3* pEye,const xmVector3* pAt,const xmVector3* pUp)
	{
		D3DXMatrixLookAtLH(&m_matView,pEye,pAt,pUp);
	}

	void Camera::SetPerspective(float fFOV,float fAspect,float fNear,float fFar)
	{
		m_fAspect = fAspect;
		m_fFOV = fFOV;
		m_fNear = fNear;
		m_fFar = fFar;
		D3DXMatrixPerspectiveFovRH(&m_matProj,fFOV,fAspect,fNear,fFar);
	}

}


