#include "SampleTerrain.h"


namespace ma
{
	SampleTerrain::SampleTerrain()
	{
	}


	void SampleTerrain::Load()
	{
 		Vector3 lookAtPos(0,0,0);
 		Vector3 eyePos = Vector3(0,-80,80);
 		GetCamera()->GetSceneNode()->LookAt(eyePos,lookAtPos);
		
		m_pTerrain = new Terrain();
		m_pTerrain->Create("terrain/shaolin.terrain",m_pScene.get());
	}

	void SampleTerrain::UnLoad()
	{
		m_pTerrain = NULL;		
	}


}


