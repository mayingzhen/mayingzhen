#include "Samples/SimpleSceneView.h"

namespace ma
{
	SimpleSceneView::SimpleSceneView()
	{
		m_vEyePos = D3DXVECTOR3(0, 600, 1000);
		m_fMoveCameraSpeed = 20.0f;
	}

	void SimpleSceneView::Init(Application* pApplication)
	{
		assert(pApplication);
		if (pApplication == NULL)
			return;

		m_pApplication = pApplication;

		D3DXVECTOR3 lookatPos = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
		D3DXMatrixLookAtLH(&m_matView,&m_vEyePos,&lookatPos,&vUp);

		int nWndWidth = 0;
		int nWndHeigh = 0;
		pApplication->GetWindowSize(nWndWidth,nWndHeigh);

		float fFOV = D3DX_PI / 4;
		float fAspect = (float)nWndWidth / (float)nWndHeigh;
		float fNearClip = 0.1f;
		float fFarClip = 3000.0f;
		D3DXMatrixPerspectiveFovLH(&m_matProj,fFOV,fAspect,fNearClip,fFarClip);
	}

	void SimpleSceneView::Shutdown()
	{

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
		
		float fMoveDis = m_fMoveCameraSpeed * timeElapsed;
		
		bool isChaneg = false;
		if ( pInput->IsKeyDown(OIS::KC_UP) )
		{
			m_vEyePos.y += fMoveDis;
			isChaneg = true;
		}
		if ( pInput->IsKeyDown(OIS::KC_DOWN) )
		{
			m_vEyePos.y -= fMoveDis;
			isChaneg = true;
		}
		if ( pInput->IsKeyDown(OIS::KC_LEFT) )
		{
			m_vEyePos.x -= fMoveDis;
			isChaneg = true;
		}
		if ( pInput->IsKeyDown(OIS::KC_RIGHT) )
		{
			m_vEyePos.x += fMoveDis;
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
			D3DXVECTOR3 lookatPos = D3DXVECTOR3(0, 0, 0);
			D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
			D3DXMatrixLookAtLH(&m_matView,&m_vEyePos,&lookatPos,&vUp);
		}
	}

	void SimpleSceneView::Tick(float timeElapsed)
	{
		UpdateCamra(timeElapsed);
	}

	void SimpleSceneView::Render()
	{
	}

	void SimpleSceneView::OnResize(int w,int h)
	{
		float fFOV = D3DX_PI / 4;
		float fAspect = (float)w / (float)h;
		float fNearClip = 0.1f;
		float fFarClip = 3000.0f;
		D3DXMatrixPerspectiveFovLH(&m_matProj,fFOV,fAspect,fNearClip,fFarClip);
	}
}

