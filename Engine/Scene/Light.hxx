#include "Light.h"

namespace ma
{
	Light::Light(Scene* pSene, const char* pNodeName):
	SceneNode(pSene,pNodeName)
	{
		m_bCreateShadow = false;
		m_Type = TYPE_ORTHOGRAPHIC;
		m_vUpVector = D3DXVECTOR3(0, 1, 0);
		m_fAspectRatio = 1.0f;
		m_vSource = D3DXVECTOR3(0, 0, 0);
		m_vTarget = D3DXVECTOR3(0, 0, 0);
		m_fNear = 0.1f;
		m_fFar = 3000.0f;
		m_fFOV =  D3DXToRadian(90.0f);
		m_fRange = m_fFar - m_fNear;
		m_vLightDiffuse = D3DXVECTOR3( 2.0f, 2.0f, 2.0f);//D3DXVECTOR3(0.7f,0.7f,0.7f);
		m_vLightAmbient = D3DXVECTOR3(0.25f,0.25f,0.25f);
		//m_fSpecular = D3DXVECTOR3();

		//D3DXVECTOR4( 2.0f, 2.0f, 2.0f, 1.0f );

	}

	void Light::Update()
	{
		m_vSource = GetPositionWS();

		CalculateMatrices();
	}

	void Light::CalculateMatrices()
	{
// 		m_ControlState.m_vRotation.y = Clamp(m_ControlState.m_vRotation.y, DegreeToRadian(-89.9f), DegreeToRadian(0.0f));
// 		float ch = cosf(m_ControlState.m_vRotation.x);
// 		float sh = sinf(m_ControlState.m_vRotation.x);
// 		float cp = cosf(m_ControlState.m_vRotation.y);
// 		float sp = sinf(m_ControlState.m_vRotation.y);
		//D3DXVECTOR3 vDist = m_vTarget - m_vSource;
		//float leng = D3DXVec3Length(&vDist);
		//m_vSource = m_vTarget + D3DXVECTOR3(sh*cp, -sp, cp*ch) * leng;

		// view matrix
		D3DXMatrixLookAtLH(&m_mView, &m_vSource, &m_vTarget, &m_vUpVector);
		//m_mView = MatrixLookAtLH(m_vSource, m_vTarget, m_vUpVector);

		// projection matrix
		if(m_Type == TYPE_PERSPECTIVE)
		{
			//m_mProj = MatrixPerspectiveFovLH(m_fFOV, m_fAspectRatio, m_fNear, m_fFar);
			D3DXMatrixPerspectiveFovLH(&m_mProj, m_fFOV, m_fAspectRatio, m_fNear, m_fFar);
		}
		else
		{
			// this is just a funny way to calculate a size for the light using FOV
			float fFarPlaneSize = 2 * tanf(m_fFOV * 0.5f) * m_fFar;
			//m_mProj = MatrixOrthoLH(fFarPlaneSize * m_fAspectRatio, fFarPlaneSize, m_fNear, m_fFar);
			D3DXMatrixOrthoLH(&m_mProj, fFarPlaneSize * m_fAspectRatio, fFarPlaneSize, m_fNear, m_fFar);
		}
	}

}

