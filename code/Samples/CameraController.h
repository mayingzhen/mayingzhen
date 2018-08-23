#pragma once

namespace ma
{

	class CameraController : public Component
	{

	public:
		CameraController(Camera* pCamObj);
		~CameraController();

		virtual	void Update();

	private:
		float m_fMoveSpeed = 0.0f;
		float m_fRotateSpeed = 0.0f;

		float yaw_ = 0.0f;
		/// Camera pitch angle.
		float pitch_ = 0.0f;

		Camera*	m_pCamera;
	};
}




