#include "Camera.h"

namespace ma
{
	IMPL_OBJECT(Camera,Component)

	Camera::Camera()
	{
		m_fAspect = 1.0f;
		m_fNear = 0.1f;
		m_fNearMin = 0.1f;
		m_fFar = 300.0f;
		m_fFOV = DegreesToRadians(45.0f);
	}

	Camera::~Camera()
	{

	}

	void Camera::RegisterAttribute()
	{
		REGISTER_OBJECT(Camera,CreateCamera);

		ACCESSOR_ATTRIBUTE(Camera, "Aspect", GetAspect, SetAspect, float, 1.0f, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Camera, "FovDegrees", GetFovDegrees, SetFovDegrees, float, 45.0f, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Camera, "Near", GetNearClip, SetNearClip, float, 0.1f, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Camera, "Far", GetFarClip, SetFarClip, float, 300.0f, AM_DEFAULT);
	}

	void Camera::MarkDirty()
	{
		Matrix4 matView = m_pSceneNode->GetMatrixWS().inverse();
		m_matViewProj.SetMatView(matView);

		m_frustum.Update(m_matViewProj.GetMatViewProj(),GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2);
	}

	const Matrix4& Camera::GetMatView()
	{
		return m_matViewProj.GetMatView();
	}

	const Matrix4& Camera::GetMatProj()
	{
		return m_matViewProj.GetMatProj();
	}

	const Matrix4& Camera::GetMatViewProj()
	{
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

	void Camera::SetNearClip(float fNear)
	{
		SetPerspective(m_fFOV,m_fAspect,fNear,m_fFar);
	}

	void Camera::SetFarClip(float fFar)
	{
		SetPerspective(m_fFOV,m_fAspect,m_fNear,fFar);
	}

	void Camera::SetFovDegrees(float fFov)
	{
		SetPerspective( DegreesToRadians(fFov),m_fAspect,m_fNear,m_fFar);
	}

	void Camera::SetAspect(float fAsPect)
	{
		SetPerspective(m_fFOV,fAsPect,m_fNear,m_fFar);
	}

	Ray Camera::GetWorldRayCast(const Vector2& mousePos)
	{
		ASSERT(m_nWidth > 0 && m_nHeight > 0);

		Real nx = (2.0f * mousePos.x)/m_nWidth - 1.0f;
		Real ny = 1.0f - (2.0f * mousePos.y)/m_nHeight;

		// Use midPoint rather than far point to avoid issues with infinite projection
		Vector3 farPoint (nx, ny,  1.f);

		// Get ray origin and ray target on near plane in world space
		Vector3 rayOrigin = this->GetSceneNode()->GetPosWS();
		Vector3 rayTarget = GetMatViewProj().inverse() * farPoint;

		Vector3 rayDirection = rayTarget - rayOrigin;
		rayDirection.normalise();

		// when the camera pos is too big, m_matInverseVP is -1.#N
		//ASSERT(IsCorrectNumber(rayOrigin.x) && IsCorrectNumber(rayOrigin.y) && IsCorrectNumber(rayOrigin.z));
		//ASSERT(IsCorrectNumber(rayDirection.x) && IsCorrectNumber(rayDirection.y) && IsCorrectNumber(rayDirection.z));

		Ray ray;
		ray.setOrigin(rayOrigin);
		ray.setDirection(rayDirection);
		return ray;

// 		float viewportW = clientSize.x;
// 		float viewPortH = clientSize.y;
// 
// 		Matrix4 matVPInv = m_matViewProj.GetMatViewProj().inverse();
// 
// 		Vector3 vProj0( 2.0f * point.x / viewportW  - 1.0f, -2.0f * point.y / viewPortH + 1.0f, -1.0f);
// 		Vector3 vProj1( 2.0f * point.x / viewportW  - 1.0f, -2.0f * point.y / viewPortH + 1.0f, 0.0f);
// 		Vector3 vWorld0 = matVPInv * vProj0;
// 		Vector3 vWorld1 = matVPInv * vProj1;
// 
// 		worldOrig = vWorld0;
// 		worldDir = (vWorld1 - vWorld0).normalisedCopy();
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

	RefPtr<Camera> CreateCamera()
	{
		RefPtr<Camera> pCamera = new Camera();
		return pCamera;
	}

}


