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
		CommonModuleInit();
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


		//Vector3 vEyePos =  Vector3(0, 600, 1000);
		//m_pCamera->LookAt(&vEyePos);

 		int nWndWidth,nWndHeigh;
 		pApplication->GetWindowSize(nWndWidth,nWndHeigh);
 		float fFOV = PI / 4;
 		float fAspect = (float)nWndWidth / (float)nWndHeigh;
 		float fNearClip = 1.0f;
 		float fFarClip = 300.0f;
		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);
	}

	void SimpleSceneView::Shutdown()
	{
		SAFE_DELETE(m_pCamera);
		//SAFE_DELETE(m_pScene);

		D3D9RenderModuleShutdown();
		EngineModuleShutdown();
		CommonModuleShutdown();
	}

	void SimpleSceneView::Load()
	{
	}

	void SimpleSceneView::Unload()
	{
	}

	void SimpleSceneView::UpdateCamra(float timeElapsed)
	{
		if (m_pCamera == NULL)
			return;

		const OIS::MouseState* pMS = GetInput() ? GetInput()->GetMouseState() : NULL;
		if (pMS == NULL)
			return;
		
		Vector3 vEyePos = m_pCamera->GetPositionWS();

		float fMoveDis = m_fMoveCameraSpeed * timeElapsed;

		// RotateCamera
		if ( GetInput()->IsMouseButtonDown(OIS::MB_Right) )
		{
			EulerAngleXYZ vRotEuler(0.0f,0.0f,0.0f);	
			vRotEuler.y = pMS->X.rel / 512.0f;
			vRotEuler.x = pMS->Y.rel / 512.0f;

			Quaternion qRot;
			QuaternionFromEulerAngleXYZ(&qRot,&vRotEuler);
			m_pCamera->RotateLS(qRot);
		}

		// MoveCamera
		if ( GetInput()->IsMouseButtonDown(OIS::MB_Middle) )
		{
			float fMoveX = pMS->X.rel * fMoveDis;
			float fMoveY = pMS->Y.rel * fMoveDis;
			Vector3 vDist(fMoveX,fMoveY,0);

			m_pCamera->TranslateLS(vDist);
		}

		// ZoomCamera
		if ( pMS->Z.rel != 0 )
		{	
			float fDeltaZoom = pMS->Z.rel * fMoveDis;
			Vector3 vDist(0.0f,0.0f,-fDeltaZoom);
			m_pCamera->TranslateLS(vDist);
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
		//float fFOV = _PI / 4;
		//float fAspect = (float)w / (float)h;
		//float fNearClip = 0.1f;
		//float fFarClip = 3000.0f;
		//MatrixPerspectiveFovLH(&m_matProj,fFOV,fAspect,fNearClip,fFarClip);
	}
}

