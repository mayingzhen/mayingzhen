#include "Camera.h"

namespace ma
{
	IMPL_OBJECT(Camera,Component)

	Camera::Camera(GameObject* pGameObj)
	:Component(pGameObj)
	{
		m_fAspect = 1.0f;
		m_fNear = 0.1f;
		m_fNearMin = 0.1f;
		m_fFar = 300.0f;
		m_fFOV = ToRadian(45.0f);
	}

	Camera::~Camera()
	{

	}

	void Camera::FitAABB(const AABB& aabb, float fMargin)
	{	

		AABB aabbCS = aabb;
		aabbCS.Transform( m_matViewProj.GetMatView() );

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
		MatrixInverse(&matViewInv,NULL,& m_matViewProj.GetMatView());
		Vector3 vDummyPosWS;
		Vec3TransformCoord(&vDummyPosWS,&vPosCS,&matViewInv);

		Vector3 vPosOffsetWS = aabb.Center() - vDummyPosWS;

		Vector3* vPos = (Vector3*)(&matViewInv._41);
		*vPos += vPosOffsetWS;
		//MatrixInverse(& m_matViewProj.GetMatView(),NULL,&matViewInv);
	}

	Vector3	Camera::ProjToWorldNormal(const Vector2* pVec,float fDepth) 
	{
		Vector3 vRet;
		Matrix4x4 matVPInv;
		MatrixInverse(&matVPInv,NULL,& m_matViewProj.GetMatViewProj());
		Vector3 p0(0.0f,0.0f,-1.0f);
		Vector3 p1(pVec->x ,pVec->y ,-1.0f);	
		Vec3TransformCoord(&p0,&p0,&matVPInv);
		Vec3TransformCoord(&p1,&p1,&matVPInv);
		vRet = p1 - p0;
		vRet *= fDepth / GetNearClip();
		return vRet;
	}

	bool Camera::IsCull(AABB aabb)
	{
		if ( aabb.IsNull() )
			return false;

		aabb.Transform( m_matViewProj.GetMatViewProj() );

		if (aabb.m_vMax.x < -1 || aabb.m_vMin.x > +1 ||
			aabb.m_vMax.y < -1 || aabb.m_vMin.y > +1 ||
			aabb.m_vMax.z < -1 || aabb.m_vMin.z > +1) // gl -1 ~ 1;
		{
			return true;
		}

		return false;
	}

	Vector2 Camera::GetNearPlaneSize() const
	{
		Vector2 vSize;	
		vSize.y = tanf(m_fFOV*0.5f) * m_fNear * 2.0f;
		vSize.x = m_fAspect * vSize.y;
		return vSize;
	}

	void Camera::SetPerspective(float fFOV,float fAspect,float fNear,float fFar)
	{
		m_fAspect = fAspect;
		m_fFOV = fFOV;
		m_fNear = fNear;
		m_fFar = fFar;
		Matrix4x4 matProj;
		GetRenderDevice()->MakePerspectiveMatrix(&matProj,fFOV,fAspect,fNear,fFar);
		m_matViewProj.SetMatProj(matProj);
	}

	void Camera::UpdateTransform()
	{
		Matrix4x4 matView;
		MatrixInverse(&matView,NULL,&GetGameObject()->GetSceneNode()->GetWorldMatrix());
		m_matViewProj.SetMatView(matView);
	}

	void Camera::GetWorldRayCast(const Vector2& clientSize,const Vector2& point, Vector3& worldOrig, Vector3& worldDir)
	{
		float viewportW = clientSize.x;
		float viewPortH = clientSize.y;

		Matrix4x4 matVPInv = m_matViewProj.GetMatViewProj();
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

	void Camera::AdjustPlanes(const AABB& aabbWorld)
	{
		AABB aabbView = aabbWorld;
		aabbView.Transform( m_matViewProj.GetMatView() );

		float fNear = Max(aabbView.m_vMin.z, m_fNearMin);
		float fFar = Max(aabbView.m_vMax.z, fNear + 1.0f);

		SetPerspective(m_fFOV, m_fAspect,fNear,fFar);
	}
}


