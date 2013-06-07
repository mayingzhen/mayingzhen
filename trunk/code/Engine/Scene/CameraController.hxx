#include "CameraController.h"




namespace ma
{
	CameraController::CameraController(Camera* pCam)
	{
		m_fZoomSpeed = 1.0f;
		m_fMoveSpeed = 5.0f;
		m_pCamera = pCam;
	}

	void CameraController::UpdateInput()
	{
		if (NULL == m_pInput || m_pCamera == NULL || GetTimer() == NULL)
		{
			return;
		}

		float fTimeElapsed = GetTimer()->GetFrameDeltaTime();
		

		//Handle rotation
		if (m_pInput->IsMouseButtonDown(OIS::MB_Middle) && m_pInput->IsKeyDown(OIS::KC_LMENU))
		{
			EulerAngleXYZ vRotEuler(0.0f,0.0f,0.0f);	
			const OIS::MouseState* pMS = m_pInput->GetMouseState();
			vRotEuler.y = pMS->X.rel / 512.0f;
			vRotEuler.x = pMS->Y.rel / 512.0f;

			RotateCamera(vRotEuler);	
		}

			
		//Handle pan
		if (!m_pInput->IsKeyDown(OIS::KC_LMENU) && m_pInput->IsMouseButtonDown(OIS::MB_Middle) )
		{
			Vector3 vPan(0.0f,0.0f,0.0f);
			float fPanDist = m_fMoveSpeed * fTimeElapsed;

			const OIS::MouseState* pMS = m_pInput->GetMouseState();
			Vector2 vPanHS(pMS->X.rel / 512.0f, pMS->Y.rel / 512.0f);
	
			Vector3 vPanMouse = m_pCamera->ProjToWorldNormal(&vPanHS);
			fPanDist *= Vec3Length(&vPanMouse);				
		
			float fPanDirLenHS = Vec2Length(&vPanHS);
			if (fPanDirLenHS > FEPS)
			{
				fPanDist /= fPanDirLenHS;
				vPan.x -= vPanHS.x * fPanDist; 
				vPan.y = vPanHS.y * fPanDist;
			}

			MoveCamera(vPan);
		}

		//Handle zoom
		float fDeltaZoom = m_pInput->GetMouseState()->Z.rel;
		if (fDeltaZoom != 0)
		{
			fDeltaZoom *=  m_fZoomSpeed * fTimeElapsed;

			ZoomCamera(fDeltaZoom);		
		}	

	}

	void CameraController::MoveCamera(const Vector3& vDeltaLS)
	{
		NodeTransform tsfCamera = m_pCamera->GetTransform();

		Vector3 vDeltaWS;
		Vec3TransformNormal(&vDeltaWS,&vDeltaLS,&tsfCamera);
		tsfCamera.m_vPos += vDeltaWS;

		m_pCamera->SetTransform(tsfCamera);
	}

	void CameraController::RotateCamera(const EulerAngleXYZ& vDeltaAngle)
	{
		Quaternion qRot;
		QuaternionFromEulerAngleXYZ(&qRot,&vDeltaAngle);

		Matrix4x4 matRotCenter;
		ComputeRotationCenter(&matRotCenter);

		NodeTransform tsfRotCenter;
		NodeTransform tsfRotCenterInv;
		NodeTransform tsfRot;

		TransformFromMatrix(&tsfRotCenter,&matRotCenter);
		TransformInverse(&tsfRotCenterInv,&tsfRotCenter);
		TransformSetIdentity(&tsfRot);
		tsfRot.m_qRot = qRot;

		NodeTransform tsfCamera = m_pCamera->GetTransform();

		TransformMul(&tsfCamera,&tsfCamera,&tsfRotCenterInv);
		TransformMul(&tsfCamera,&tsfCamera,&tsfRot);
		TransformMul(&tsfCamera,&tsfCamera,&tsfRotCenter);

		m_pCamera->SetTransform(tsfCamera);
	}

	void CameraController::ZoomCamera(float fDeltaZoom)
	{
		NodeTransform tsfCamera = m_pCamera->GetTransform();

		Vector3 vDist(0.0f,0.0f,-fDeltaZoom);
		Vec3TransformNormal(&vDist,&vDist,&tsfCamera);
		tsfCamera.m_vPos += vDist;

		m_pCamera->SetTransform(tsfCamera);
	}


	void CameraController::ComputeRotationCenter(Matrix4x4* pOut) const
	{
		MatrixIdentity(pOut);

		Vector3& vXAxis = *MatrixAsVector3(pOut,0);
		Vector3& vYAxis = *MatrixAsVector3(pOut,1);
		Vector3& vZAxis = *MatrixAsVector3(pOut,2);
		Vector3& vPos = *MatrixAsVector3(pOut,3);

		NodeTransform tsfCamera = m_pCamera->GetTransform();

		//X Axis
		vXAxis = Vector3(1.0f,0.0f,0.0f);
		Vec3TransformNormal(&vXAxis,&vXAxis,&tsfCamera);
		
		//Y Axis
		vYAxis =  Vector3(0.0f,1.0f,0.0f);

		// Z Axis
		Vec3Cross(&vZAxis,&vXAxis,&vYAxis);
		Vec3Normalize(&vZAxis,&vZAxis);
		
		Vec3Cross(&vXAxis,&vYAxis,&vZAxis);

		vPos = m_pCamera->GetLookAtPos();
	}


}