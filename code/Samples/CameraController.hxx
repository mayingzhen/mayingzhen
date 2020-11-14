#include "CameraController.h"

#ifdef WIN32
#include "windows.h"
#endif


namespace ma
{
	CameraController::CameraController(Camera* pCamera)
	{
		m_fMoveSpeed = 20.f;
		m_fRotateSpeed = 0.4f;

		m_pCamera = pCamera;

		Quaternion rot = m_pCamera->GetRotationWS();
		Vector3 vAngele = rot.EulerAngles();
		pitch_ = vAngele.x;
		yaw_ = vAngele.y;

	}

	CameraController::~CameraController()
	{
	}

	void CameraController::Update()
    {
		float dwElapsed = GetTimer()->GetFrameDeltaTime();

		Quaternion rot = m_pCamera->GetRotation();
		Vector3 vAngele = rot.EulerAngles();
		pitch_ = vAngele.x;
		yaw_ = vAngele.y;

		if (GetInput()->IsMouseButtonDown(MB_Left))
		{
			Vector2 mouseMove = GetInput()->GetMouseMove();
			yaw_ += mouseMove.x * m_fRotateSpeed;
			pitch_ += mouseMove.y * m_fRotateSpeed;
			//pitch_ = Math::Clamp(pitch_, -90.0f, 90.0f);
			Quaternion rota;
			rota.FromEulerAngles(pitch_, yaw_, vAngele.z);
			m_pCamera->SetRotation(rota);

			
		}

		float fMoveSpeed = m_fMoveSpeed * dwElapsed;

// 		if (GetInput()->IsKeyDown(Keyboard::KEY_Q))
// 		{
// 			yaw_ += m_fRotateSpeed;
// 			Quaternion rota;
// 			rota.FromEulerAngles(pitch_, yaw_, 0.0f);
// 			m_pCamera->SetRotation(rota);
// 		}
// 		else if (GetInput()->IsKeyDown(Keyboard::KEY_E))
// 		{
// 			yaw_ += m_fRotateSpeed;
// 			Quaternion rota;
// 			rota.FromEulerAngles(pitch_, yaw_, 0.0f);
// 			m_pCamera->SetRotation(rota);
// 		}
// 		else 
		if (GetInput()->IsKeyDown(Keyboard::KEY_W))
		{
			m_pCamera->Forward(fMoveSpeed);
		}
		else if (GetInput()->IsKeyDown(Keyboard::KEY_S))
		{
			m_pCamera->Forward(-fMoveSpeed);
		}
		else if (GetInput()->IsKeyDown(Keyboard::KEY_A))
		{
			m_pCamera->Right(-fMoveSpeed);
		}
		else if (GetInput()->IsKeyDown(Keyboard::KEY_D))
		{
			m_pCamera->Right(fMoveSpeed);
 		}
	}

}
