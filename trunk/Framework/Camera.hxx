#include "Framework/Camera.h"

namespace ma
{
	Camera::Camera()
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::LookAt(const D3DXVECTOR3* pEye,const D3DXVECTOR3* pAt,const D3DXVECTOR3* pUp)
	{
		D3DXMatrixLookAtLH(&m_matView,pEye,pAt,pUp);
	}

	void Camera::SetPerspective(float fFOV,float fAspect,float fNear,float fFar)
	{
		m_fAspect = fAspect;
		m_fFOV = fFOV;
		m_fNear = fNear;
		m_fFar = fFar;
		D3DXMatrixPerspectiveFovLH(&m_matProj,fFOV,fAspect,fNear,fFar);
	}

}


