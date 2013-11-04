#include "CameraController.h"




namespace ma
{
	CameraController::CameraController(Camera* pCamera)
	{
		m_fMoveStep = 100.0f;

		m_vTarget = Vec3Zero();


		Vector3 vCameraPos = pCamera->GetSceneNode()->GetTransform().m_vPos;
		
		m_fTargetDis = Vec3Length(&vCameraPos);

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
		Matrix4x4 matCamera;
		NodeTransform tsfCamera = m_pCamera->GetSceneNode()->GetTransform();
		MatrixFromTransform(&matCamera,&tsfCamera);

		Vector3 vCameraPos = matCamera.GetRow(3);
		Vector3 vCameraForward = matCamera.GetRow(2);
		Vec3Normalize(&vCameraForward,&vCameraForward);

		m_vTarget = vCameraPos + vCameraForward * m_fTargetDis;
	}

	void CameraController::UpdateTargetDis()
	{
		Vector3 vCameraPos = m_pCamera->GetSceneNode()->GetTransform().m_vPos;
		Vector3 vDis = vCameraPos - m_vTarget;
		m_fTargetDis = Vec3Length(&vDis);
		if (m_fTargetDis <= 10.0f)
			m_fTargetDis = 10.0f;
	}


	void CameraController::RotateMoveCamera()
	{
		const OIS::MouseState& mouseState = GetInput()->GetMouseState();

		EulerAngleXYZ vRotEuler(0.0f,0.0f,0.0f);	
		vRotEuler.y = mouseState.X.rel / 512.0f;
		vRotEuler.x = mouseState.Y.rel / 512.0f;


		Quaternion qRot;
		QuaternionFromEulerAngleXYZ(&qRot,&vRotEuler);


		Matrix4x4 matRotCenter;
		ComputeRotationCenter(&matRotCenter);


		NodeTransform tsfRotCenter;
		NodeTransform tsfRotCenterInv;
		NodeTransform tsfRot;

		TransformFromMatrix(&tsfRotCenter,&matRotCenter);
		TransformInverse(&tsfRotCenterInv,&tsfRotCenter);
		TransformSetIdentity(&tsfRot);
		tsfRot.m_qRot = qRot;

		NodeTransform tsfCamera = m_pCamera->GetSceneNode()->GetTransform();

		TransformMul(&tsfCamera,&tsfCamera,&tsfRotCenterInv);
		TransformMul(&tsfCamera,&tsfCamera,&tsfRot);
		TransformMul(&tsfCamera,&tsfCamera,&tsfRotCenter);

		m_pCamera->GetSceneNode()->SetTransform(tsfCamera);
	}

	void CameraController::MoveCamera()
	{
		const OIS::MouseState& mouseState = GetInput()->GetMouseState();

		Vector3 vDeltaLS(0.0f,0.0f,0.0f);

		float fPanDist = m_fMoveStep / 100.0f;

		Vector2 vPanHS(mouseState.X.rel / 512.0f, mouseState.Y.rel / 512.0f);

		Vector3 vPanMouse = m_pCamera->ProjToWorldNormal(&vPanHS,m_fTargetDis);
		fPanDist *= Vec3Length(&vPanMouse);				

		float fPanDirLenHS = Vec2Length(&vPanHS);
		if (fPanDirLenHS > FEPS)
		{
			fPanDist /= fPanDirLenHS;
			vDeltaLS.x -= vPanHS.x * fPanDist; 
			vDeltaLS.y = vPanHS.y * fPanDist;
		}

		NodeTransform tsfCamera = m_pCamera->GetSceneNode()->GetTransform();

		Vector3 vDeltaWS;
		Vec3TransformNormal(&vDeltaWS,&vDeltaLS,&tsfCamera);
		tsfCamera.m_vPos += vDeltaWS;

		m_pCamera->GetSceneNode()->SetTransform(tsfCamera);
	}

	void CameraController::RotateCamera()
	{
		const OIS::MouseState& mouseState = GetInput()->GetMouseState();

		NodeTransform tsfCamera = m_pCamera->GetSceneNode()->GetTransform();

		Quaternion qRot;
		QuaternionIdentity(&qRot);

		EulerAngleXYZ vRotEuler(0.0f,0.0f,0.0f);	
		if ( Abs(mouseState.X.rel) > Abs(mouseState.Y.rel) )
		{
			vRotEuler.y = mouseState.X.rel / 512.0f;

			QuaternionFromEulerAngleXYZ(&qRot,&vRotEuler);

			QuaternionMultiply(&tsfCamera.m_qRot,&tsfCamera.m_qRot,&qRot);

			m_pCamera->GetSceneNode()->SetTransform(tsfCamera);
		}
		else
		{
			vRotEuler.x = mouseState.Y.rel / 512.0f;

			QuaternionFromEulerAngleXYZ(&qRot,&vRotEuler);

			QuaternionMultiply(&tsfCamera.m_qRot,&qRot,&tsfCamera.m_qRot);

			m_pCamera->GetSceneNode()->SetTransform(tsfCamera);
		}
	}

	void CameraController::ZoomCamera(float fDeltaZoom)
	{
		NodeTransform tsfCamera = m_pCamera->GetSceneNode()->GetTransform();

		fDeltaZoom *= m_fTargetDis * 0.1f / m_fMoveStep;

		Vector3 vDist(0.0f,0.0f,fDeltaZoom);
		Vec3TransformNormal(&vDist,&vDist,&tsfCamera);
		tsfCamera.m_vPos += vDist;

		m_pCamera->GetSceneNode()->SetTransform(tsfCamera);
	}


	void CameraController::ComputeRotationCenter(Matrix4x4* pOut) const
	{
		Vector3& vXAxis = *MatrixAsVector3(pOut,0);
		Vector3& vYAxis = *MatrixAsVector3(pOut,1);
		Vector3& vZAxis = *MatrixAsVector3(pOut,2);
		Vector3& vPos = *MatrixAsVector3(pOut,3);

		MatrixIdentity(pOut);

		const Vector3 vUp(0.0f,1.0f,0.0f);

		NodeTransform tsfCamera = m_pCamera->GetSceneNode()->GetTransform();

		//Get X Axis
		vXAxis = Vector3(1.0f,0.0f,0.0f);
		Vec3TransformNormal(&vXAxis,&vXAxis,&tsfCamera);


		vYAxis = vUp;
		Vec3Cross(&vZAxis,&vXAxis,&vYAxis);
		Vec3Normalize(&vZAxis,&vZAxis);
		Vec3Cross(&vXAxis,&vYAxis,&vZAxis);

		vPos = m_vTarget;
	}


}