#include "SampleTerrain.h"


namespace ma
{
	SampleTerrain::SampleTerrain()
	{
		m_pTerrain = NULL;
	}


	void SampleTerrain::Load()
	{
		Vector3 lookAtPos(500,0,600);
		Vector3 eyePos;
		eyePos.x = lookAtPos.x;
		eyePos.y = lookAtPos.y + 1200;
		eyePos.z = lookAtPos.z + 1200;
		GetCamera()->LookAt(eyePos,lookAtPos,Vector3(0,1,0));

		float fZoomSpeed = GetCameraControll()->GetZoomSpeed(); 
		float fMoveCameraSpeed = GetCameraControll()->GetMoveSpeed();
		GetCameraControll()->SetZoomSpeed(fZoomSpeed * 7);
		GetCameraControll()->SetMoveSpeed(fMoveCameraSpeed * 5);

		m_pTerrain = new Terrain();
		m_pTerrain->Create("terrain/shaolin.Terrain");
	}

	void SampleTerrain::UnLoad()
	{
		SAFE_DELETE(m_pTerrain);
	}


	void SampleTerrain::Update()
	{
	}

	void SampleTerrain::Render()
	{
		m_pTerrain->Render();
	}

}


