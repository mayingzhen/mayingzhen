#include "Camera.h"

namespace ma
{
	Camera::Camera()
	{
		m_vEyePt = D3DXVECTOR3(0,0,0);
		m_vLookAtPt = D3DXVECTOR3(0,0,0);
		m_fNear = 0.1f;
		m_fFar = 300.0f;
		m_fFOV = D3DXToRadian(45.0f); 
		m_vUpVector = D3DXVECTOR3(0, 1, 0);
	}

	Camera::~Camera()
	{

	}

	void Camera::Update()
	{
		SyncFromSceneNode();
		
		CalculateSplitPositions();

		CalculateFrustum();
	}

	void Camera::SyncFromSceneNode()
	{	
		m_vEyePt = GetPositionWS();
		D3DXMatrixInverse(&m_matView,NULL,&GetWorldMatrix());
		//D3DXMatrixMultiply(&m_matViewProj,&m_matView,&m_matProj);
	}

	void Camera::SyncToSceneNode()
	{
		maNodeTransform tsfWS;
		D3DXMATRIX matViewInv;
		D3DXMatrixInverse(&matViewInv,NULL,&m_matView);
		maTransformFromMatrix(&tsfWS,matViewInv);
		SetTransformWS(tsfWS);
	}

	void Camera::LookAt(const D3DXVECTOR3* pEye,const D3DXVECTOR3* pAt,const D3DXVECTOR3* pUp)
	{
		if (pAt)
		{
			m_vLookAtPt = *pAt;
		}
		if (pUp)
		{
			m_vUpVector = *pUp;
		}
		if (pEye)
		{
			m_vEyePt = *pEye;
		}
		
		D3DXMatrixLookAtLH(&m_matView,&m_vEyePt,&m_vLookAtPt,&m_vUpVector);
		
		// 
		SyncToSceneNode();
	}

	void Camera::SetPerspective(float fFOV,float fAspect,float fNear,float fFar)
	{
		m_fAspect = fAspect;
		m_fFOV = fFOV;
		m_fNear = fNear;
		m_fFar = fFar;
		D3DXMatrixPerspectiveFovLH(&m_matProj,fFOV,fAspect,fNear,fFar);
	}

	void Camera::CalculateFrustum()
	{
		m_frustum = Frustum(m_matView * m_matProj);

		for (UINT i = 0; i < NUM_PSSM; ++i)
		{
			D3DXMATRIX matProj;
			D3DXMatrixPerspectiveFovLH(&matProj,m_fFOV,m_fAspect,m_fSplitPos[i],m_fSplitPos[i + 1]);
			m_arrSplitFrustum[i] = Frustum(m_matView * matProj);
		}
	}

	Frustum Camera::CalculateFrustum(float fNear, float fFar)
	{
		D3DXMATRIX matProj;
		D3DXMatrixPerspectiveFovLH(&matProj,m_fFOV,m_fAspect,fNear,fFar);
		Frustum frustum(m_matView * matProj);
// 		D3DXVECTOR3 vX,vY,vZ,vTemp;
// 		D3DXVec3Normalize( &vZ, &(m_vLookAtPt - m_vEyePt) );
// 		D3DXVec3Cross(&vTemp, &m_vUpVector, &vZ);
// 		D3DXVec3Normalize(&vX, &vTemp);
// 		D3DXVec3Cross(&vTemp, &vZ, &vX);
// 		D3DXVec3Normalize(&vY, &vTemp);
// 
// 		float fNearPlaneHalfHeight = tanf(m_fFOV * 0.5f) * m_fNear;
// 		float fNearPlaneHalfWidth = fNearPlaneHalfHeight * m_fAspect;
// 
// 		float fFarPlaneHalfHeight = tanf(m_fFOV * 0.5f) * m_fFar;
// 		float fFarPlaneHalfWidth = fFarPlaneHalfHeight * m_fAspect;
// 
// 		D3DXVECTOR3 vNearPlaneCenter = m_vEyePt + vZ * m_fNear;
// 		D3DXVECTOR3 vFarPlaneCenter = m_vEyePt + vZ * m_fFar;
// 
// 		
// 
// 		frustum.vPts[0] = D3DXVECTOR3(vNearPlaneCenter - vX*fNearPlaneHalfWidth - vY*fNearPlaneHalfHeight);
// 		frustum.vPts[1] = D3DXVECTOR3(vNearPlaneCenter - vX*fNearPlaneHalfWidth + vY*fNearPlaneHalfHeight);
// 		frustum.vPts[2] = D3DXVECTOR3(vNearPlaneCenter + vX*fNearPlaneHalfWidth + vY*fNearPlaneHalfHeight);
// 		frustum.vPts[3] = D3DXVECTOR3(vNearPlaneCenter + vX*fNearPlaneHalfWidth - vY*fNearPlaneHalfHeight);
// 
// 		frustum.vPts[4] = D3DXVECTOR3(vFarPlaneCenter - vX*fFarPlaneHalfWidth - vY*fFarPlaneHalfHeight);
// 		frustum.vPts[5] = D3DXVECTOR3(vFarPlaneCenter - vX*fFarPlaneHalfWidth + vY*fFarPlaneHalfHeight);
// 		frustum.vPts[6] = D3DXVECTOR3(vFarPlaneCenter + vX*fFarPlaneHalfWidth + vY*fFarPlaneHalfHeight);
// 		frustum.vPts[7] = D3DXVECTOR3(vFarPlaneCenter + vX*fFarPlaneHalfWidth - vY*fFarPlaneHalfHeight);
	
		return frustum;
	}

	void Camera::CalculateSplitPositions()
	{
		// Practical split scheme:
		//
		// CLi = n*(f/n)^(i/numsplits)
		// CUi = n + (f-n)*(i/numsplits)
		// Ci = CLi*(lambda) + CUi*(1-lambda)
		//
		// lambda scales between logarithmic and uniform
		//
		float fSplitSchemeWeight = 0.5;
		for(int i = 0; i < NUM_PSSM; i++)
		{
			float fIDM = i / (float)NUM_PSSM;
			float fLog = m_fNear  * powf(m_fFar/m_fNear, fIDM);
			float fUniform = m_fNear + (m_fFar - m_fNear) * fIDM;
			m_fSplitPos[i] = fLog * fSplitSchemeWeight + fUniform * (1 - fSplitSchemeWeight);
		}

		// make sure border values are accurate
		m_fSplitPos[0] = m_fNear;
		m_fSplitPos[NUM_PSSM] = m_fFar;
	}

}


