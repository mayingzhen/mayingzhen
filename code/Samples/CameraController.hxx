#include "CameraController.h"

#ifdef WIN32
#include "windows.h"
#endif


namespace ma
{
	CameraController::CameraController(Camera* pCamera)
	{
		m_fMoveSpeed = 20.f;
		m_fRotateSpeed = 0.1f;

		m_pCamera = pCamera;

	}

	CameraController::~CameraController()
	{
	}

	void CameraController::Init()
	{
		Quaternion rot = m_pCamera->GetRotationWS();
		Vector3 vAngele = rot.EulerAngles();
		pitch_ = vAngele.x;
		yaw_ = vAngele.y;
	}

	void CameraController::Process(float dwElapsed)
    {
		if (GetInput()->IsMouseButtonDown(MB_Left))
		{
			Vector2 mouseMove = GetInput()->GetMouseMove();
			yaw_ += mouseMove.x * m_fRotateSpeed;
			pitch_ += mouseMove.y * m_fRotateSpeed;
			//pitch_ = Math::Clamp(pitch_, DegreesToRadians(-90.0f), DegreesToRadians(90.0f));
			Quaternion rota;
			rota.FromEulerAngles(pitch_, yaw_, 0.0f);
			m_pCamera->SetRotation(rota);

			Vector3 vAngele = rota.EulerAngles();
		}

		if (GetInput()->IsKeyDown(Keyboard::KEY_W))
		{
			m_pCamera->Translate(Vector3::UNIT_Z * m_fMoveSpeed * dwElapsed);
		}
		else if (GetInput()->IsKeyDown(Keyboard::KEY_S))
		{
			m_pCamera->Translate(-Vector3::UNIT_Z * m_fMoveSpeed * dwElapsed);
		}
		else if (GetInput()->IsKeyDown(Keyboard::KEY_A))
		{
			m_pCamera->Translate(Vector3::UNIT_X * m_fMoveSpeed * dwElapsed);
		}
		else if (GetInput()->IsKeyDown(Keyboard::KEY_D))
		{
			m_pCamera->Translate(-Vector3::UNIT_X * m_fMoveSpeed * dwElapsed);
 		}
	}

}
