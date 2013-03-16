#include "Samples/SimpleSceneView.h"
#include "D3D9Render/Module.h"

namespace ma
{
	SimpleSceneView::SimpleSceneView()
	{
		m_pCamera = NULL;
		m_pScene = NULL;
		m_pApplication = NULL;
		m_fMoveCameraSpeed = 20.0f;
	}

	void SimpleSceneView::Init(Application* pApplication)
	{
		EngineModuleInit();
		D3D9RenderModuleInit();

		GetRenderDevice()->Init(pApplication->GetWindID());

		assert(pApplication);
		if (pApplication == NULL)
			return;

		m_pApplication = pApplication;
		m_pScene = new Scene();
		m_pCamera = new Camera();
		m_pScene->GetRootNode()->AddChildNode(m_pCamera);


		D3DXVECTOR3 vEyePos =  D3DXVECTOR3(0, 600, 1000);
		m_pCamera->LookAt(&vEyePos);

 		int nWndWidth,nWndHeigh;
 		pApplication->GetWindowSize(nWndWidth,nWndHeigh);
 		float fFOV = D3DX_PI / 4;
 		float fAspect = (float)nWndWidth / (float)nWndHeigh;
 		float fNearClip = 1.0f;
 		float fFarClip = 3000.0f;
		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);
	}

	void SimpleSceneView::Shutdown()
	{
		SAFE_DELETE(m_pCamera);
		//SAFE_DELETE(m_pScene);

		D3D9RenderModuleShutdown();
		EngineModuleShutdown();
	}

	void SimpleSceneView::Load()
	{
	}

	void SimpleSceneView::Unload()
	{
	}

	void SimpleSceneView::UpdateCamra(float timeElapsed)
	{
		Input* pInput = GetInput();
		if (pInput == NULL)
			return;
		
		D3DXVECTOR3 vEyePos = m_pCamera->GetPositionWS();

		float fMoveDis = m_fMoveCameraSpeed * timeElapsed;
		
		bool isChaneg = false;
		if ( pInput->IsKeyDown(OIS::KC_UP) )
		{
			vEyePos.y += fMoveDis;
			isChaneg = true;
		}
		if ( pInput->IsKeyDown(OIS::KC_DOWN) )
		{
			vEyePos.y -= fMoveDis;
			isChaneg = true;
		}
		if ( pInput->IsKeyDown(OIS::KC_LEFT) )
		{
			vEyePos.x += fMoveDis;
			isChaneg = true;
		}
		if ( pInput->IsKeyDown(OIS::KC_RIGHT) )
		{
			vEyePos.x -= fMoveDis;
			isChaneg = true;
		}
		if (pInput->IsKeyDown(OIS::KC_ADD))
		{
			m_fMoveCameraSpeed += 1.0f;
		}
		if (pInput->IsKeyDown(OIS::KC_SUBTRACT))
		{
			m_fMoveCameraSpeed -= 1.0f;
		}

		if (isChaneg)
		{
			//D3DXVECTOR3 lookatPos = D3DXVECTOR3(0, 0, 0);
			//D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
			//D3DXMatrixLookAtLH(&m_matView,&m_vEyePos,&lookatPos,&vUp);
			m_pCamera->LookAt(&vEyePos);
		}
	}

	void SimpleSceneView::Tick(float timeElapsed)
	{
		UpdateCamra(timeElapsed);

		if (m_pScene)
		{
			m_pScene->Update(timeElapsed);
		}
	}

	void SimpleSceneView::Render()
	{
		if (m_pScene == NULL || m_pCamera == NULL)
			return;

		if (m_pScene)
		{
			m_pScene->Render(m_pCamera);
		}
	}

	void SimpleSceneView::OnResize(int w,int h)
	{
		//float fFOV = D3DX_PI / 4;
		//float fAspect = (float)w / (float)h;
		//float fNearClip = 0.1f;
		//float fFarClip = 3000.0f;
		//D3DXMatrixPerspectiveFovLH(&m_matProj,fFOV,fAspect,fNearClip,fFarClip);
	}
}

