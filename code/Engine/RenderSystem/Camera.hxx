#include "Camera.h"

namespace ma
{
	IMPL_OBJECT(Camera,Component)

	Camera::Camera(GameObject* pGameObj)
	:Component(pGameObj)
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

	void Camera::FitAABB(const AABB& aabb, float fMargin)
	{	

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
		GetRenderDevice()->MakePerspectiveMatrix(&m_matProj,fFOV,fAspect,fNear,fFar);
	}

	void Camera::UpdateTransform()
	{
		MatrixInverse(&m_matView,NULL,&GetGameObject()->GetSceneNode()->GetWorldMatrix());
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

	Vector3	Camera::ProjToWorldNormal(const Vector2* pVec) const
	{
		int index = GetRenderThread()->GetThreadList();

        Vector3 vDir = m_vLookAtPt - m_vEyePt;
		float fDepth = Vec3Length(&vDir);

		float fRatio = fDepth / m_fNear;

		Matrix4x4 matWorld;
		MatrixInverse(&matWorld,NULL,&m_matView);

		Vector2 vNearPlaneSize = GetNearPlaneSize();
		Vector3 vVecNearPlan(pVec->x * vNearPlaneSize.x,pVec->y * vNearPlaneSize.y,0.0f);
		Vector3 vWorld = vVecNearPlan * fRatio;
		Vec3TransformNormal(&vWorld,&vWorld,&matWorld);
		return vWorld;

	}

	void Camera::GetWorldRayCast(const Vector2& clientSize,const Vector2& point, Vector3& worldOrig, Vector3& worldDir)
	{
		float viewportW = clientSize.x;
		float viewPortH = clientSize.y;

		Matrix4x4 matVPInv;

		MatrixMultiply(&matVPInv,& GetViewMatrix(),& GetProjMatrix());
		MatrixInverse(&matVPInv,NULL,&matVPInv);

		Vector3 vProj0( 2.0f * point.x / viewportW  - 1.0f, -2.0f * point.y / viewPortH + 1.0f, -1.0f);
		Vector3 vProj1( 2.0f * point.x / viewportW  - 1.0f, -2.0f * point.y / viewPortH + 1.0f, 0.0f);
		Vector3 vWorld0,vWorld1;

		Vec3TransformCoord(&vWorld0,&vProj0,&matVPInv);
		Vec3TransformCoord(&vWorld1,&vProj1,&matVPInv);
		worldOrig = vWorld0;
		worldDir = vWorld1 - vWorld0;
		Vec3Normalize(&worldDir,&worldDir);
	}

}


