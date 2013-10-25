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
		GetCamera()->LookAt(eyePos,lookAtPos,Vector3(0,1,0));

		float fZoomSpeed = GetCameraControll()->GetZoomSpeed(); 
		float fMoveCameraSpeed = GetCameraControll()->GetMoveSpeed();
		GetCameraControll()->SetZoomSpeed(fZoomSpeed * 7);
		GetCameraControll()->SetMoveSpeed(fMoveCameraSpeed * 5);

		GetTerrain()->Create("terrain/shaolin.Terrain");

		GetRenderSystem()->SetAmbientColor(Vector4(0.1f,0.1f,0.1f,0.1f));

		m_pSun = GetEntitySystem()->CreateGameObject("Sun");
		DirectonalLight* pDirLight = m_pSun->CreateComponent<DirectonalLight>();
		pDirLight->SetLightColor(Vector4(1.0f,1.0f,1.0f,1.0f));
		//pDirLight->SetDirection(Vector3(1.0f,1.0f,1.0f));
		pDirLight->SetCreateShadow(true);

		m_vRotae = Vector3(30,0,0);
		m_pSun->GetSceneNode()->RotateXAxisLS(45);
		m_pSun->GetSceneNode()->Translate(Vector3(0.0f,3000.0f,-3000.0f));

		//GetEntitySystem()->GetCameraObject()->GetSceneNode()->RotateXAxisLS(45);
		//GetEntitySystem()->GetCameraObject()->GetSceneNode()->Translate(Vector3(0.0f,3000.0f,-3000.0f));

		//
		for (UINT i = 0; i < 2; ++i)
		{
			for (UINT j = 0; j < 2; ++j)
			{
				{
					GameObject* pCharMagic = GetEntitySystem()->CreateGameObject("magic");

					RenderMesh* pMeshComp = pCharMagic->CreateComponent<RenderMesh>();
					pMeshComp->Load("magician/Body.skn","magician/Body.mat");

					IAnimationObject* pAnimationObject = pCharMagic->CreateComponent<IAnimationObject>();
					pAnimationObject->Load("magician/Body.Aniset","magician/Body.ske");
					pAnimationObject->PlayAnimation("Mag602");

					Vector3 vPos(500 + i * 300,0,300 + j * 300);
					//vPos.y = GetTerrain()->GetHeight(vPos.x,vPos.z);
					pCharMagic->GetSceneNode()->Translate(vPos);
				}


				{
// 					GameObject* pFire = GetEntitySystem()->CreateGameObject("Fire");
// 					//ParticleEmitter* pPaticleComp = pFire->CreateComponent<ParticleEmitter>();
// 					//pPaticleComp->Load("Particle/fire.particle");
// 
// 					PointLight* pPointLight = pFire->CreateComponent<PointLight>();
// 					pPointLight->SetLightColor(Vector4(1.0f,1.0f,1.0f,1.0f));
// 					pPointLight->SetRadius(500);
// 
// 					pFire->GetSceneNode()->Translate(Vector3(i * 350.0f,100.0f,j * 300.0f));
// 					pFire->GetSceneNode()->Scale(10.0f);
				}

			}

		}


	}

	void SampleTerrain::UnLoad()
	{
		GetInput()->RemoveKeyListener(this);				
	}


	bool SampleTerrain::keyPressed(const OIS::KeyEvent &arg)
	{
		if ( arg.key == OIS::KC_UP )
		{
			m_vRotae.x += 1.0f;
			m_pSun->GetSceneNode()->RotateXAxisLS(m_vRotae.x);
		}
		else if ( arg.key == OIS::KC_DOWN )
		{
			m_vRotae.x -= 1.0f;
			m_pSun->GetSceneNode()->RotateXAxisLS(m_vRotae.x);
		}

		return true;
	}

	bool SampleTerrain::keyReleased(const OIS::KeyEvent &arg)
	{
		return true;
	}

	void SampleTerrain::Update()
	{

	}

	void SampleTerrain::Render()
	{
		//m_pTerrain->Render();
	}

}


