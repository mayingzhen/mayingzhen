#include "CameraController.h"




namespace ma
{
	CameraController::CameraController(Camera* pCamera)
	{
		m_fMoveStep = 100.0f;

		m_vTarget = Vector3::ZERO;


		Vector3 vCameraPos = pCamera->GetSceneNode()->GetPos();
		
		m_fTargetDis = vCameraPos.length();

		m_pCamera = pCamera;
	}

	void CameraController::UpdateInput()
	{
#if PLATFORM_ANDROID == 1 || PLAFTORM_IOS == 1
		return;
#endif
		UpdateTargetDis();

		const OIS::MouseState& mouseState = GetInput()->GetMouseState();

		if ( GetInput()->IsMouseButtonDown(OIS::MB_Left) && GetInput()->IsKeyDown(OIS::KC_LMENU) ) // Atl + ×ó¼ü
		{
			RotateMoveCamera();
		}
		else if ( GetInput()->IsMouseButtonDown(OIS::MB_Middle) || GetInput()->IsMouseButtonDown(OIS::MB_Left) ) // ÖÐ¼ü»ò×ó¼ü
		{ 
			MoveCamera();

			UpdateTarget();
		}
		else if ( GetInput()->IsKeyDown(OIS::KC_LMENU) && GetInput()->IsMouseButtonDown(OIS::MB_Right) ) // Atl + ÓÒ¼ü 
		{
			float fDeltaZoom = mouseState.X.rel + mouseState.Y.rel ;

			ZoomCamera(fDeltaZoom);	

			UpdateTargetDis();
		}
		else if (GetInput()->IsMouseButtonDown(OIS::MB_Right))
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
		Vector3 vCameraPos = m_pCamera->GetSceneNode()->GetPos();
		Vector3 vCameraForward = m_pCamera->GetSceneNode()->GetForward();

		m_vTarget = vCameraPos + vCameraForward * m_fTargetDis;
	}

	void CameraController::UpdateTargetDis()
	{
		Vector3 vCameraPos = m_pCamera->GetSceneNode()->GetPos();
		m_fTargetDis = (vCameraPos - m_vTarget).length();
		if (m_fTargetDis <= 10.0f)
			m_fTargetDis = 10.0f;
	}


	void CameraController::RotateMoveCamera()
	{
		Vector2 vScreenOffset = GetMouseScreenOffset();
		
		Vector3 vAxis;
		float fAngle;
		if ( Math::Abs(vScreenOffset.x) > Math::Abs(vScreenOffset.y) )
		{
			vAxis = m_pCamera->GetSceneNode()->GetUp();
			fAngle = RadiansToDegrees(vScreenOffset.x);
		}
		else
		{
			vAxis = m_pCamera->GetSceneNode()->GetRight();
			fAngle = RadiansToDegrees(vScreenOffset.y);	
		}

		m_pCamera->GetSceneNode()->RotateAround(m_vTarget,vAxis,fAngle);
	}

	Vector2	CameraController::GetMouseScreenOffset()
	{
		const OIS::MouseState& mouseState = GetInput()->GetMouseState();

		int w,h;
		Platform::GetInstance().GetWindowSize(w,h);

		Vector2 vScreenOffset(mouseState.X.rel, mouseState.Y.rel);
		vScreenOffset.x = -vScreenOffset.x * 2.f / w;
		vScreenOffset.y = vScreenOffset.y * 2.f / h;

		return vScreenOffset;
	}

	void CameraController::MoveCamera()
	{
		Vector2 vScreenOffset = GetMouseScreenOffset();
	
		Vector3 v = m_pCamera->ProjToWorldNormal(&vScreenOffset, m_fTargetDis);
		m_pCamera->GetSceneNode()->Translate(v);
	}

	void CameraController::RotateCamera()
	{
		Vector2 vScreenOffset = GetMouseScreenOffset();

		Matrix3 matEuler;
		if ( Math::Abs(vScreenOffset.x) > Math::Abs(vScreenOffset.y) )
		{
			matEuler.FromEulerAnglesXZY(Radian(0), Radian(0), Radian(vScreenOffset.x));
		}
		else
		{
			matEuler.FromEulerAnglesXZY(Radian(vScreenOffset.y), Radian(0), Radian(0));
		}

		Transform tsfWS = m_pCamera->GetSceneNode()->GetTransform();
		tsfWS.m_qRot = tsfWS.m_qRot * Quaternion(matEuler) ;

		m_pCamera->GetSceneNode()->SetTransform(tsfWS);
	}

	void CameraController::ZoomCamera(float fDeltaZoom)
	{
 		fDeltaZoom *= m_fTargetDis * 0.01f / m_fMoveStep;

		m_pCamera->GetSceneNode()->Forward(fDeltaZoom);

		std::string deubg = "fDeltaZoom = " + StringConverter::toString(fDeltaZoom);
		_OutputDebugString(deubg.c_str());
	}

}
