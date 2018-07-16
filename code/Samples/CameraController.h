#ifndef __CameraController_H__
#define __CameraController_H__


namespace ma
{

	class CameraController
	{

	public:
		CameraController(Camera* pCamObj);
		~CameraController();

		void Init();

		void Process(float dwElapsed);

	private:
		float m_fMoveSpeed = 0.0f;
		float m_fRotateSpeed = 0.0f;

		float yaw_ = 0.0f;
		/// Camera pitch angle.
		float pitch_ = 0.0f;

		Camera*	m_pCamera;
	};
}

#endif 



