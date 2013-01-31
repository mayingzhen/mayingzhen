#include "Samples/SimpleSceneView.h"

namespace ma
{
	void SimpleSceneView::Init(Application* pApplication)
	{
		assert(pApplication);
		if (pApplication == NULL)
			return;

		m_pApplication = pApplication;

		D3DXVECTOR3 eyePos = D3DXVECTOR3(0, 600, 1000);
		D3DXVECTOR3 lookatPos = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
		D3DXMatrixLookAtLH(&m_matView,&eyePos,&lookatPos,&vUp);

		int nWndWidth = 0;
		int nWndHeigh = 0;
		pApplication->GetWindowSize(nWndHeigh,nWndHeigh);

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

	void SimpleSceneView::Tick(float timeElapsed)
	{
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
