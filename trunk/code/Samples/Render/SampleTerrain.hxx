#include "SampleTerrain.h"


namespace ma
{
	SampleTerrain::SampleTerrain()
	{
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

		GetTerrain()->Create("terrain/shaolin.Terrain");

		//
		for (UINT i = 0; i < 5; ++i)
		{
			for (UINT j = 0; j < 5; ++j)
			{
				GameObject* pCharMagic = GetEntitySystem()->CreateGameObject("magic");
				MeshComponent* pMeshComp = pCharMagic->CreateComponent<MeshComponent>();
				pMeshComp->Load("magician/Body.skn","magician/Body.tga");

				AnimComponent* pAnimComp = pCharMagic->CreateComponent<AnimComponent>();
				pAnimComp->Load("magician/Body.Aniset","magician/Body.ske");
 				IAnimationObject* pAnimationObject = pAnimComp->GetAnimObject();
 				IAnimationSet* pAnimSet = pAnimationObject->GetAnimationSet();
// 				pAnimSet->AddAnimClip("gigi/210_run/bip01.ska","gigi_Run");
// 				pAnimSet->AddAnimClip("gigi/281_run_jump_start/bip01.ska","gigi_jump");
// 				pAnimSet->AddAnimClip("magician/602/bip01.ska","Mag602");
// 				pAnimSet->AddAnimClip("magician/100/bip01.ska","mag100");
				int nAnimation = pAnimSet->GetActionNumber();
				pAnimationObject->PlayAnimation( 2/*rand() % nAnimation*/ );

// 				XMLOutputArchive outAr;
// 				bool res = outAr.Open("E:/work/mydemon_svn/data/magician/Body.Aniset");
// 				pAnimSet->Serialize(outAr);
// 				outAr.Close();

				pCharMagic->GetSceneNode()->Translate(Vector3(i * 300.0f,100.0f,j * 300.0f));

				GameObject* pFire = GetEntitySystem()->CreateGameObject("Fire");
				ParticlComponent* pPaticleComp = pFire->CreateComponent<ParticlComponent>();
				pPaticleComp->Load("Particle/fire.particle");

				pFire->GetSceneNode()->Translate(Vector3(i * 300.0f,100.0f,j * 300.0f));
				pFire->GetSceneNode()->Scale(10.0f);

			}

		}


	}

	void SampleTerrain::UnLoad()
	{
					
	}


	void SampleTerrain::Update()
	{
	}

	void SampleTerrain::Render()
	{
		//m_pTerrain->Render();
	}

}


