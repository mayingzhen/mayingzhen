#include "Camera.h"

namespace ma
{
	Camera::Camera()
	{
		m_fAspect = 1.0f;
		m_fNear = 0.1f;
		m_fNearMin = 0.1f;
		m_fFar = 300.0f;
		m_fFOV = DegreesToRadians(45.0f);

		m_matVP = m_matProjInv = m_matVPInv = m_matViewInv = m_matView = Matrix4::IDENTITY;

		m_atNode = new SceneNode(NULL,NULL);
		this->AddChild(m_atNode.get());

		m_eyeNode = new SceneNode(NULL,NULL);
		this->AddChild(m_eyeNode.get());
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

	void Camera::Update()
	{
		if (m_eyeNode->BeginMatrix() || m_atNode->BeginMatrix() || this->BeginMatrix())
		{
			if (m_eyeNode->BeginMatrix() || m_atNode->BeginMatrix())
			{
				Vector3 vEye = m_eyeNode->GetPos(), vAt = m_atNode->GetPos(), vUp = Vector3::UNIT_Z;
				Vector3 vDirZ = vEye - vAt;
				vDirZ.normalise();
				if (vDirZ.positionEquals(Vector3::UNIT_Z))
				{
					vUp.y = 0.01f;
				}
				else if (vDirZ.positionEquals(Vector3::NEGATIVE_UNIT_Z))
				{
					vUp.y = 0.01f;
				}

				Vector3 vDirX = vUp.crossProduct(vDirZ);
				vDirX.normalise();

				Vector3 vDirY = vDirZ.crossProduct(vDirX);
				vDirY.normalise();
				m_eyeNode->SetRotation(Quaternion(vDirX, vDirY, vDirZ));
			}

			this->CalcMatrix();
			const Matrix4& matEye = m_eyeNode->CalcMatrix();
			m_atNode->CalcMatrix();

			this->UpdateViewMatrix(matEye);

			m_atNode->EndMatrix();
			m_eyeNode->EndMatrix();
			this->EndMatrix();
		}

		SceneNode::Update();
	}

	void Camera::UpdateViewMatrix(const Matrix4& matWorldEye)
	{
		m_matViewInv = matWorldEye;
		m_matView = matWorldEye.inverseAffine();

		this->UpdateViewProjMatrix();
	}

	void Camera::UpdateViewProjMatrix()
	{
		m_matProjInv = m_matProj.inverse();
		m_matVP = m_matProj*m_matView;
		m_matVPInv = m_matVP.inverse();

		m_frustum.Update(m_matVP, GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2);
	}

	const Matrix4& Camera::GetMatView()
	{
		return m_matView;
	}

	const Matrix4& Camera::GetMatProj()
	{
		return m_matProj;
	}

	const Matrix4& Camera::GetMatViewProj()
	{
		return m_matVP;
	}

	const Matrix4& Camera::GetMatViewProjInv()
	{
		return m_matVPInv;
	}

	void Camera::FitAABB(const AABB& aabb, float fMargin)
	{	
		AABB aabbCS = aabb;
		aabbCS.transform(m_matView);

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

		Matrix4 matViewInv = m_matView.inverseAffine();
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
		GetRenderDevice()->MakePerspectiveMatrix(m_matProj,fFOV,fAspect,fNear,fFar);
		
		UpdateViewProjMatrix();
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
		Vector3 rayOrigin = this->GetPosWS();
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

	void Camera::Yaw(const Radian& fParam)
	{
		Vector3 vDir = m_eyeNode->GetPos() - m_atNode->GetPos();
		Vector3 vUp = Vector3::UNIT_Z;
		Quaternion q(fParam, vUp);

		Vector3 vDest = q*vDir;
		m_eyeNode->SetPos(vDest + m_atNode->GetPos());
	}

	void Camera::Pitch(const Radian& fParam)
	{
		Vector3 vDir = m_eyeNode->GetPos() - m_atNode->GetPos();
		Vector3 vRight = this->GetRight();
		Quaternion q(fParam, vRight);

		Vector3 vDest = q*vDir;

		// 上下仰角限制在Degree[-90,90]之间
		Plane planeXY(Vector3::UNIT_Z, Vector3(0,0,0));
		if (vDest.dotProduct(planeXY.projectVector(vDir)) < 0)
		{
			return;
		}
		m_eyeNode->SetPos(vDest + m_atNode->GetPos());
	}

	void Camera::LookAt(const Vector3& vEye, const Vector3& vAt)
	{
		m_eyeNode->SetPos(vEye);

		m_atNode->SetPos(vAt);
	}

	Vector3	Camera::GetForward() const
	{
		return Vector3(m_matView[2][0], m_matView[2][1], m_matView[2][2]);
	}

	Vector3	Camera::GetRight() const
	{
		return Vector3(m_matView[0][0], m_matView[0][1], m_matView[0][2]);
	}

	Vector3	Camera::GetUp() const
	{
		return Vector3(m_matView[1][0], m_matView[1][1], m_matView[1][2]);
	}

	void Camera::AdjustPlanes(const AABB& aabbWorld)
	{
		if ( aabbWorld.isInfinite() )
			return;

		AABB aabbView = aabbWorld;
		aabbView.transform( m_matView );

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


