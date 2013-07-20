#include "SampleTerrain.h"


namespace ma
{
	SampleTerrain::SampleTerrain()
	{
		m_pTerrain = NULL;
	}


	void SampleTerrain::Init()
	{
		Sample::Init();

		Vector3 lookAtPos(500,0,600);
		Vector3 eyePos;
		eyePos.x = lookAtPos.x;
		eyePos.y = lookAtPos.y + 1200;
		eyePos.z = lookAtPos.z + 1200;
// 		g_Camera.m_fFOV = D3DX_PI / 4;
// 		g_Camera.m_fAspect = (float)g_nWndWidth / (float)g_nWndHeigh;
// 		g_Camera.m_fNearClip = 1.0f;
// 		g_Camera.m_fFarClip = 30000.0f;
// 		g_Camera.SetViewParams( eyePos, lookAtPos ); 
		m_pCamera->LookAt(eyePos,lookAtPos,Vector3(0,1,0));

		m_fZoomSpeed *= 5;
		m_fMoveCameraSpeed *= 5;

		m_pTerrain = new Terrain();
		m_pTerrain->Create("terrain/shaolin.Terrain");
	}



	void SampleTerrain::Update()
	{
		Sample::Update();
	}

	void SampleTerrain::Render()
	{
		m_pTerrain->Render();
	}

}


