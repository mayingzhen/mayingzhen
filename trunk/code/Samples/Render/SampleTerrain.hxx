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


