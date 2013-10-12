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

		GetRenderSystem()->SetAmbientColor(Vector4(0.2f,0.2f,0.2f,0.2f));


		//
		for (UINT i = 0; i < 2; ++i)
		{
			for (UINT j = 0; j < 2; ++j)
			{
				{
					GameObject* pCharMagic = GetEntitySystem()->CreateGameObject("magic");
					MeshComponent* pMeshComp = pCharMagic->CreateComponent<MeshComponent>();
					pMeshComp->Load("magician/Body.skn","magician/Body.mat");

					AnimComponent* pAnimComp = pCharMagic->CreateComponent<AnimComponent>();
					pAnimComp->Load("magician/Body.Aniset","magician/Body.ske");
 					IAnimationObject* pAnimationObject = pAnimComp->GetAnimObject();
					pAnimationObject->PlayAnimation("Mag602");

					pCharMagic->GetSceneNode()->Translate(Vector3(i * 300.0f,100.0f,j * 300.0f));
				}


				{
					GameObject* pFire = GetEntitySystem()->CreateGameObject("Fire");
					ParticlComponent* pPaticleComp = pFire->CreateComponent<ParticlComponent>();
					pPaticleComp->Load("Particle/fire.particle");

					PointLightComponent* pPointLightComp = pFire->CreateComponent<PointLightComponent>();
					pPointLightComp->GetLight()->SetLightColor(Vector4(1.0f,1.0f,1.0f,1.0f));
					pPointLightComp->GetLight()->SetRadius(100);

					pFire->GetSceneNode()->Translate(Vector3(i * 350.0f,100.0f,j * 300.0f));
					pFire->GetSceneNode()->Scale(10.0f);
				}

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


