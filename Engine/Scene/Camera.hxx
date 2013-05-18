#include "Camera.h"

namespace ma
{
	Camera::Camera()
	{
		m_vEyePt = Vector3(0,0,0);
		m_vLookAtPt = Vector3(0,0,0);
		m_fNear = 0.1f;
		m_fFar = 300.0f;
		m_fFOV = ToRadian(45.0f); 
		m_vUpVector = Vector3(0, 1, 0);
	}

	Camera::~Camera()
	{

	}


	void Camera::Update()
	{
		//SyncFromSceneNode();
		
		CalculateSplitPositions();

		CalculateFrustum();
	}

	void Camera::SyncFromSceneNode()
	{	
		m_vEyePt = GetPosition(TS_WORLD);
		MatrixInverse(&m_matView,NULL,&GetWorldMatrix());
		//MatrixMultiply(&m_matViewProj,&m_matView,&m_matProj);
	}

	void Camera::SyncToSceneNode()
	{
		NodeTransform tsfWS;
		Matrix4x4 matViewInv;
		MatrixInverse(&matViewInv,NULL,&m_matView);
		TransformFromMatrix(&tsfWS,&matViewInv);
		SetTransform(tsfWS);
	}

	void Camera::FitAABB(const AABB& aabb, float fMargin)
	{	
		SyncFromSceneNode();

		AABB aabbCS = aabb;
		aabbCS.Transform(m_matView);

		//Center object
		Vector3 aabbSize = aabbCS.Size();
		bool bUseWidth = false;
		if (aabbSize.y <= FEPS)
		{
			bUseWidth = true;
		}
		else
		{
			float ratio = aabbSize.x / aabbSize.y;
			bUseWidth = (ratio > m_fAspect);
		}

		Vector3 vPosCS(0.0f,0.0f,0.0f);
		Vector2 vNearSize = GetNearPlaneSize();
		vNearSize *= (1.0f - fMargin);
		if(bUseWidth)
		{

			vPosCS.z =  m_fNear * aabbSize.x / vNearSize.x;
		}
		else
		{
			vPosCS.z = m_fNear * aabbSize.y / vNearSize.y;
		}

		Matrix4x4 matViewInv;
		MatrixInverse(&matViewInv,NULL,&m_matView);
		Vector3 vDummyPosWS;
		Vec3TransformCoord(&vDummyPosWS,&vPosCS,&matViewInv);

		Vector3 vPosOffsetWS = aabb.Center() - vDummyPosWS;

		Vector3* vPos = (Vector3*)(&matViewInv._41);
		*vPos += vPosOffsetWS;
		MatrixInverse(&m_matView,NULL,&matViewInv);

		SyncToSceneNode();
	}

	Vector2 Camera::GetNearPlaneSize() const
	{
		Vector2 vSize;	
		vSize.y = tanf(m_fFOV*0.5f) * m_fNear * 2.0f;
		vSize.x = m_fAspect * vSize.y;
		return vSize;
	}

	void Camera::LookAt(const Vector3& vEye,const Vector3& vAt,const Vector3& vUp)
	{
		m_vEyePt = vEye;
		m_vLookAtPt = vAt;
		m_vUpVector = vUp;
		MatrixLookAtLH(&m_matView,&vEye,&vAt,&vUp);
	}

	void Camera::SetPerspective(float fFOV,float fAspect,float fNear,float fFar)
	{
		m_fAspect = fAspect;
		m_fFOV = fFOV;
		m_fNear = fNear;
		m_fFar = fFar;
		GetRenderDevice()->MakeProjectionMatrix(&m_matProj,fFOV,fAspect,fNear,fFar);
		//MatrixPerspectiveFovLH(&m_matProj,fFOV,fAspect,fNear,fFar);
		//MatrixPerspectiveFovGL_LH(&m_matProj,fFOV,fAspect,fNear,fFar);
	}

	NodeTransform Camera::GetTransform() const
	{
		NodeTransform tsf;
		TransformSetIdentity(&tsf);

		Matrix4x4 matTSFWS;
		MatrixInverse(&matTSFWS,NULL,&m_matView);
		TransformFromMatrix(&tsf,&matTSFWS);

		return tsf;
	}

	void Camera::SetTransform(const NodeTransform& tsfCamra)
	{
		Matrix4x4 matViewInv;
		MatrixFromTransform(&matViewInv,&tsfCamra);

		MatrixInverse(&m_matView,NULL,&matViewInv);
	}


	void Camera::CalculateFrustum()
	{
		m_frustum = Frustum(m_matView * m_matProj);

		for (UINT i = 0; i < NUM_PSSM; ++i)
		{
			Matrix4x4 matProj;
			MatrixPerspectiveFovLH(&matProj,m_fFOV,m_fAspect,m_fSplitPos[i],m_fSplitPos[i + 1]);
			m_arrSplitFrustum[i] = Frustum(m_matView * matProj);
		}
	}

	Frustum Camera::CalculateFrustum(float fNear, float fFar)
	{
		Matrix4x4 matProj;
		MatrixPerspectiveFovLH(&matProj,m_fFOV,m_fAspect,fNear,fFar);
		Frustum frustum(m_matView * matProj);
// 		Vector3 vX,vY,vZ,vTemp;
// 		Vec3Normalize( &vZ, &(m_vLookAtPt - m_vEyePt) );
// 		Vec3Cross(&vTemp, &m_vUpVector, &vZ);
// 		Vec3Normalize(&vX, &vTemp);
// 		Vec3Cross(&vTemp, &vZ, &vX);
// 		Vec3Normalize(&vY, &vTemp);
// 
// 		float fNearPlaneHalfHeight = tanf(m_fFOV * 0.5f) * m_fNear;
// 		float fNearPlaneHalfWidth = fNearPlaneHalfHeight * m_fAspect;
// 
// 		float fFarPlaneHalfHeight = tanf(m_fFOV * 0.5f) * m_fFar;
// 		float fFarPlaneHalfWidth = fFarPlaneHalfHeight * m_fAspect;
// 
// 		Vector3 vNearPlaneCenter = m_vEyePt + vZ * m_fNear;
// 		Vector3 vFarPlaneCenter = m_vEyePt + vZ * m_fFar;
// 
// 		
// 
// 		frustum.vPts[0] = Vector3(vNearPlaneCenter - vX*fNearPlaneHalfWidth - vY*fNearPlaneHalfHeight);
// 		frustum.vPts[1] = Vector3(vNearPlaneCenter - vX*fNearPlaneHalfWidth + vY*fNearPlaneHalfHeight);
// 		frustum.vPts[2] = Vector3(vNearPlaneCenter + vX*fNearPlaneHalfWidth + vY*fNearPlaneHalfHeight);
// 		frustum.vPts[3] = Vector3(vNearPlaneCenter + vX*fNearPlaneHalfWidth - vY*fNearPlaneHalfHeight);
// 
// 		frustum.vPts[4] = Vector3(vFarPlaneCenter - vX*fFarPlaneHalfWidth - vY*fFarPlaneHalfHeight);
// 		frustum.vPts[5] = Vector3(vFarPlaneCenter - vX*fFarPlaneHalfWidth + vY*fFarPlaneHalfHeight);
// 		frustum.vPts[6] = Vector3(vFarPlaneCenter + vX*fFarPlaneHalfWidth + vY*fFarPlaneHalfHeight);
// 		frustum.vPts[7] = Vector3(vFarPlaneCenter + vX*fFarPlaneHalfWidth - vY*fFarPlaneHalfHeight);
	
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

	Vector3	Camera::ProjToWorldNormal(const Vector2* pVec) const
	{
		float fDepth = Vec3Length(&(m_vLookAtPt - m_vEyePt));

		float fRatio = fDepth / m_fNear;

		Matrix4x4 matWorld;
		MatrixInverse(&matWorld,NULL,&m_matView);

		Vector2 vNearPlaneSize = GetNearPlaneSize();
		Vector3 vVecNearPlan(pVec->x * vNearPlaneSize.x,pVec->y * vNearPlaneSize.y,0.0f);
		Vector3 vWorld = vVecNearPlan * fRatio;
		Vec3TransformNormal(&vWorld,&vWorld,&matWorld);
		return vWorld;

	}

}


