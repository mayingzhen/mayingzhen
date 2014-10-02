#include "Camera.h"

namespace ma
{
	IMPL_OBJECT(Camera,Component)

	Camera::Camera(SceneNode* pGameObj)
	:Component(pGameObj)
	{
		m_fAspect = 1.0f;
		m_fNear = 0.1f;
		m_fNearMin = 0.1f;
		m_fFar = 300.0f;
		m_fFOV = DegreesToRadians(45.0f);

		m_bMatViewDirty = true;
	}

	Camera::~Camera()
	{

	}

	void Camera::UpdateMatView()
	{
		Matrix4 matView = m_pSceneNode->GetWorldMatrix().inverse();
		m_matViewProj.SetMatView(matView);
	}


	const Matrix4& Camera::GetMatView()
	{
		if (m_bMatViewDirty)
		{
			UpdateMatView();	
		}

		return m_matViewProj.GetMatView();
	}

	const Matrix4& Camera::GetMatProj()
	{
		return m_matViewProj.GetMatProj();
	}

	const Matrix4& Camera::GetMatViewProj()
	{
		if (m_bMatViewDirty)
		{
			UpdateMatView();
		}

		return m_matViewProj.GetMatViewProj();
	}

	void Camera::FitAABB(const AABB& aabb, float fMargin)
	{	

		AABB aabbCS = aabb;
		aabbCS.transform( m_matViewProj.GetMatView() );

		//Center object
		Vector3 aabbSize = aabbCS.getSize();
		bool bUseWidth = false;
		if (aabbSize.y <= Math::FEPS)
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

		Matrix4 matViewInv = m_matViewProj.GetMatView().inverseAffine();
		Vector3 vDummyPosWS = matViewInv * vPosCS;

		Vector3 vPosOffsetWS = aabb.getCenter() - vDummyPosWS;

		Vector3 vPos = matViewInv.getTrans();
		vPos += vPosOffsetWS;
		matViewInv.setTrans(vPos);
	}

	Vector3	Camera::ProjToWorldNormal(const Vector2* pVec,float fDepth) 
	{
		Vector3 vRet;
		Matrix4 matVPInv = m_matViewProj.GetMatViewProj().inverse();
		Vector3 p0(0.0f,0.0f,-1.0f);
		Vector3 p1(pVec->x ,pVec->y ,-1.0f);
		p0 = matVPInv * p0;
		p1 = matVPInv * p1;
		vRet = p1 - p0;
		vRet *= fDepth / m_fNear;
		return vRet;
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
		Matrix4 matProj = Matrix4::IDENTITY;
		GetRenderDevice()->MakePerspectiveMatrix(matProj,fFOV,fAspect,fNear,fFar);
		m_matViewProj.SetMatProj(matProj);
	}


	void Camera::GetWorldRayCast(const Vector2& clientSize,const Vector2& point, Vector3& worldOrig, Vector3& worldDir)
	{
		float viewportW = clientSize.x;
		float viewPortH = clientSize.y;

		Matrix4 matVPInv = m_matViewProj.GetMatViewProj().inverse();

		Vector3 vProj0( 2.0f * point.x / viewportW  - 1.0f, -2.0f * point.y / viewPortH + 1.0f, -1.0f);
		Vector3 vProj1( 2.0f * point.x / viewportW  - 1.0f, -2.0f * point.y / viewPortH + 1.0f, 0.0f);
		Vector3 vWorld0 = matVPInv * vProj0;
		Vector3 vWorld1 = matVPInv * vProj1;

		worldOrig = vWorld0;
		worldDir = (vWorld1 - vWorld0).normalisedCopy();
	}

	void Camera::AdjustPlanes(const AABB& aabbWorld)
	{
		if ( aabbWorld.isInfinite() )
			return;

		AABB aabbView = aabbWorld;
		aabbView.transform( m_matViewProj.GetMatView() );

		float fNear = Math::Max(aabbView.getMinimum().z, m_fNearMin);
		float fFar = Math::Max(aabbView.getMaximum().z, fNear + 1.0f);

		SetPerspective(m_fFOV, m_fAspect,fNear,fFar);
	}
}


