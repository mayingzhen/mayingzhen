#include "SampleTerrain.h"


namespace ma
{
	SampleTerrain::SampleTerrain()
	{
	}


	void SampleTerrain::Load()
	{
		GetInput()->AddKeyListener(this);

		Vector3 lookAtPos(500,0,600);
		Vector3 eyePos;
		eyePos.x = lookAtPos.x;
		eyePos.y = lookAtPos.y + 1200;
		eyePos.z = lookAtPos.z + 1200;
		GetCamera()->GetSceneNode()->LookAt(eyePos,lookAtPos,Vector3(0,1,0));

		GetTerrain()->Create("terrain/shaolin.terrain");

		GetLightSystem()->SetAmbientColor(Vector4(0.1f,0.1f,0.1f,0.1f));

// 		m_pSun = GetEntitySystem()->CreateGameObject("Sun");
// 		DirectonalLight* pDirLight = m_pSun->CreateComponent<DirectonalLight>();
// 		pDirLight->SetLightColor(Vector4(1.0f,1.0f,1.0f,1.0f));
// 		pDirLight->SetCreateShadow(true);
// 
// 		AABB aabbTerrain = GetTerrain()->GetWorldAABB();
// 
//  		Vector3 vSource = aabbTerrain.m_vMax;
// 		vSource.y += 5000.0f;
//  		Vector3 vTarget(0,0,0);
//  		Vector3 vUp(0,1,0);
// 		m_pSun->GetSceneNode()->LookAt(vSource,vTarget,vUp);
	}

	void SampleTerrain::UnLoad()
	{
		GetInput()->RemoveKeyListener(this);				
	}


	bool SampleTerrain::keyPressed(const OIS::KeyEvent &arg)
	{
// 		if ( arg.key == OIS::KC_UP )
// 		{
// 			m_pSun->GetSceneNode()->Translate(Vector3(-100.0f,0,0));
// 			m_pSun->GetSceneNode()->LookAt(Vector3(0,0,0),Vector3(0,1,0));
// 		}
// 		else if ( arg.key == OIS::KC_DOWN )
// 		{
// 			m_pSun->GetSceneNode()->Translate(Vector3(-100.0f,0,0));
// 			m_pSun->GetSceneNode()->LookAt(Vector3(0,0,0),Vector3(0,1,0));
// 		}

		return true;
	}

	bool SampleTerrain::keyReleased(const OIS::KeyEvent &arg)
	{
		return true;
	}

}


