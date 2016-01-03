#include "CameraController.h"




namespace ma
{
	CameraController::CameraController(Camera* pCamera)
	{
		m_fMoveStep = 100.0f;

		m_vTarget = Vector3::ZERO;


		Vector3 vCameraPos = pCamera->GetPos();

		m_fTargetDis = vCameraPos.length();

		m_pCamera = pCamera;

		ResetCamera();
	}

	void CameraController::ResetCamera()
	{
		if (m_pCamera == NULL)
			return;

		m_pCamera->LookAt(Vector3(0, 200, -300), Vector3(0, 0, 0));

		int nWndWidth,nWndHeigh;
		Platform::GetInstance().GetWindowSize(nWndWidth,nWndHeigh);
		float fFOV = DegreesToRadians(50.0f);
		float fAspect = (float)nWndWidth / (float)nWndHeigh;
		float fNearClip = 0.10f;
		float fFarClip = 10000.0f;
		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);
	}

	void CameraController::UpdateInput()
	{
		if (m_pCamera == NULL || GetInput() == NULL)
			return;

#if PLATFORM_ANDROID == 1 || PLAFTORM_IOS == 1
		return;
#endif
		UpdateTargetDis();

		const MouseState& mouseState = GetInput()->GetMouseState();

		if ( GetInput()->IsMouseButtonDown(MB_Left) && GetInput()->IsKeyDown(Keyboard::KEY_ALT) ) // Atl + ×ó¼ü
		{
			RotateMoveCamera();
		}
		else if ( GetInput()->IsMouseButtonDown(MB_Middle) /*|| GetInput()->IsMouseButtonDown(MB_Left)*/ ) // ÖÐ¼ü»ò×ó¼ü
		{ 
			MoveCamera();

			UpdateTarget();
		}
		else if ( GetInput()->IsKeyDown(Keyboard::KEY_ALT) && GetInput()->IsMouseButtonDown(MB_Right) ) // Atl + ÓÒ¼ü 
		{
			float fDeltaZoom = mouseState.X.rel + mouseState.Y.rel ;

			ZoomCamera(fDeltaZoom);	

			UpdateTargetDis();
		}
		else if (GetInput()->IsMouseButtonDown(MB_Right))
		{
			RotateCamera();

			UpdateTarget();
		}
		else if (mouseState.Z.rel != 0)
		{
			float fDeltaZoom = mouseState.Z.rel;

			ZoomCamera(fDeltaZoom);	

			UpdateTargetDis();
		}
	}

	void CameraController::UpdateTarget()
	{
		Vector3 vCameraPos = m_pCamera->GetPos();
		Vector3 vCameraForward = m_pCamera->GetForward();

		m_vTarget = vCameraPos + vCameraForward * m_fTargetDis;
	}

	void CameraController::UpdateTargetDis()
	{
		Vector3 vCameraPos = m_pCamera->GetPos();
		m_fTargetDis = (vCameraPos - m_vTarget).length();
		if (m_fTargetDis <= 10.0f)
			m_fTargetDis = 10.0f;
	}


	void CameraController::RotateMoveCamera()
	{
		Vector2 vScreenOffset = GetMouseProjVec();

		Vector3 vAxis;
		float fAngle;
		if ( Math::Abs(vScreenOffset.x) > Math::Abs(vScreenOffset.y) )
		{
			vAxis = m_pCamera->GetUp();
			fAngle = RadiansToDegrees(vScreenOffset.x);
		}
		else
		{
			vAxis = m_pCamera->GetRight();
			fAngle = RadiansToDegrees(vScreenOffset.y);	
		}

		m_pCamera->RotateAround(m_vTarget,vAxis,fAngle);
	}

	Vector2	CameraController::GetMouseProjVec()
	{
		// 		const OIS::MouseState& mouseState = GetInput()->GetMouseState();
		// 
		// 		int w,h;
		// 		Platform::GetInstance().GetWindowSize(w,h);
		// 
		// 		Vector2 vScreenOffset(mouseState.X.rel, mouseState.Y.rel);
		// 		vScreenOffset.x = -vScreenOffset.x * 2.f / w;
		// 		vScreenOffset.y = vScreenOffset.y * 2.f / h;
		// 		//vScreenOffset.x = vScreenOffset.x * 2.0f / w - 1.0f;
		// 		//ScreenOffset.y = 1.0f - vScreenOffset.y * 2.0f / h;
		// 
		// 		return vScreenOffset;
		return Vector2::ZERO;
	}

	Vector3	CameraController::ProjToWorldNormal(const Vector2* pVec,float fDepth) 
	{
		Vector3 vRet;
		Matrix4 matVPInv = m_pCamera->GetMatViewProj().inverse();
		Vector3 p0(0.0f,0.0f,-1.0f);
		Vector3 p1(pVec->x ,pVec->y ,-1.0f);
		p0 = matVPInv * p0;
		p1 = matVPInv * p1;
		vRet = p1 - p0;
		vRet *= fDepth / m_pCamera->GetNearClip();
		return vRet;
	}

	void CameraController::MoveCamera()
	{
		Vector2 vScreenOffset = GetMouseProjVec();

		Vector3 v = ProjToWorldNormal(&vScreenOffset, m_fTargetDis);
		m_pCamera->Translate(v);
	}

	void CameraController::RotateCamera()
	{
		Vector2 vScreenOffset = GetMouseProjVec();

		if ( Math::Abs(vScreenOffset.x) > Math::Abs(vScreenOffset.y) )
		{
			m_pCamera->Yaw(vScreenOffset.x);	
		}
		else
		{
			m_pCamera->Pitch(vScreenOffset.y);
		}

	}

	void CameraController::ZoomCamera(float fDeltaZoom)
	{
		fDeltaZoom *= m_fTargetDis * 0.01f / m_fMoveStep;

		m_pCamera->Forward(fDeltaZoom);

		LogInfo("fDeltaZoom = %f",fDeltaZoom);
	}

}
