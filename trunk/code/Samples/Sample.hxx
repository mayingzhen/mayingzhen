#include "Samples/Sample.h"

namespace ma
{
	Sample::Sample()
	{
		m_pCamera = NULL;
		//m_pScene = NULL;
		m_fMoveCameraSpeed = 20.0f;
		m_fZoomSpeed = 500.0f;
	}

	void Sample::Init()
	{	
		m_pDefaultEffect = GetRenderDevice()->CreateShaderProgram();
		//m_pDefaultEffect->CreateFromShaderName("sprite");

		//m_pScene = new Scene();
		m_pCamera = new Camera();
		m_pCameraControl = new CameraController(m_pCamera);
		//m_pScene->GetRootNode()->AddChildNode(m_pCamera);

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

// 	void Sample::keyEvent(Keyboard::KeyEvent evt, int key)
// 	{
// // 		if (evt== Keyboard::KEY_PRESS && key == Keyboard::KEY_W)
// // 		{
// // 			m_pCamera->m
// // 		}
// 	}
// 
// 	void Sample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
// 	{
// 
// 	}
// 
// 	bool Sample::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
// 	{
// 		if (evt == Mouse::MOUSE_WHEEL && wheelDelta != 0)
// 		{
// 			float fZoom =  wheelDelta * m_fZoomSpeed * GetTimer()->GetFrameDeltaTime();
// 
// 			NodeTransform tsfCamera = m_pCamera->GetTransform();
// 
// 			Vector3 vDist(0.0f,0.0f,-fZoom);
// 			Vec3TransformNormal(&vDist,&vDist,&tsfCamera);
// 			tsfCamera.m_vPos += vDist;
// 
// 			m_pCamera->SetTransform(tsfCamera);
// 		}
// 
// 
// 		return false;
// 	}

}

