#include "Samples/Sample.h"

namespace ma
{
	Sample::Sample()
	{
		m_pCamera = NULL;
		m_fMoveCameraSpeed = 20.0f;
		m_fZoomSpeed = 50.0f;
	}

	void Sample::Init()
	{	
		m_pCamera = new Camera();
		m_pCameraControl = new CameraController(m_pCamera);

		Material::SetAuotBingCamera(m_pCamera);
		RenderQueue::SetCamera(m_pCamera);

		Vector3 vEyePos = Vector3(0, 200, 300);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		m_pCamera->LookAt(vEyePos,VAtPos,vUp);

		int nWndWidth,nWndHeigh;
		Platform::GetInstance().GetWindowSize(nWndWidth,nWndHeigh);
		float fFOV = PI / 4;
		float fAspect = (float)nWndWidth / (float)nWndHeigh;
		float fNearClip = 1.0f;
		float fFarClip = 30000.0f;
		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);
	}

	void Sample::ShoutDown()
	{

	}

	void Sample::Update()
	{
		if (m_pCameraControl)
		{
			m_pCameraControl->SetMoveSpeed(m_fMoveCameraSpeed);
			m_pCameraControl->SetZoomSpeed(m_fZoomSpeed);
			m_pCameraControl->UpdateInput();
		}
	}

	void Sample::Render()
	{

	}

}

