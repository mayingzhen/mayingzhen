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

		m_pSun = GetEntitySystem()->CreateGameObject("Sun");
		DirectonalLight* pDirLight = m_pSun->CreateComponent<DirectonalLight>();
		pDirLight->SetLightColor(Vector4(1.0f,1.0f,1.0f,1.0f));
		//pDirLight->SetDirection(Vector3(1.0f,1.0f,1.0f));
		pDirLight->SetCreateShadow(true);

		AABB aabbTerrain = GetTerrain()->GetWorldAABB();

// 		Matrix4x4 matView;
 		Vector3 vSource = aabbTerrain.m_vMax;
		vSource.y += 5000.0f;
 		Vector3 vTarget(0,0,0);
 		Vector3 vUp(0,1,0);
		m_pSun->GetSceneNode()->LookAt(vSource,vTarget,vUp);

		//GetEntitySystem()->GetCameraObject()->GetSceneNode()->RotateXAxisLS(45);
		//GetEntitySystem()->GetCameraObject()->GetSceneNode()->Translate(Vector3(-5418.0f,10220.0f,-12891.0f));

		//
		for (UINT i = 0; i < 2; ++i)
		{
			for (UINT j = 0; j < 2; ++j)
			{
				{
					GameObject* pCharMagic = GetEntitySystem()->CreateGameObject("magic");

					RenderMesh* pMeshComp = pCharMagic->CreateComponent<RenderMesh>();
					pMeshComp->Load("magician/Body.skn","magician/body.mat");

					IAnimationObject* pAnimationObject = pCharMagic->CreateComponent<IAnimationObject>();
					pAnimationObject->Load("magician/body.aniset","magician/body.ske");
					pAnimationObject->PlayAnimation("Mag602");

					Vector3 vCharPos(500 + i * 500,0,300 + j * 500);
					//vPos.y = GetTerrain()->GetHeight(vPos.x,vPos.z);
					pCharMagic->GetSceneNode()->Translate(vCharPos);

					GameObject* pBox = GetEntitySystem()->CreateGameObject("Box");
					RenderMesh* pBoxMesh = pBox->CreateComponent<RenderMesh>();
					pBoxMesh->Load("Fbx/Box.skn","Fbx/Box.mat");
					Vector3 vBoxPos(500 + i * 500,0,500 + j * 500);
					pBox->GetSceneNode()->Translate(vBoxPos);
					pBox->GetSceneNode()->Scale(60.0f);
				}


				{
					GameObject* pFire = GetEntitySystem()->CreateGameObject("Fire");
					ParticleEmitter* pPaticleComp = pFire->CreateComponent<ParticleEmitter>();
					pPaticleComp->Load("Particle/fire.particle");
 					pPaticleComp->start();

// 					PointLight* pPointLight = pFire->CreateComponent<PointLight>();
// 					pPointLight->SetLightColor(Vector4(1.0f,1.0f,1.0f,1.0f));
// 					pPointLight->SetRadius(500);

					pFire->GetSceneNode()->Translate(Vector3(i * 350.0f,100.0f,j * 300.0f));
					pFire->GetSceneNode()->Scale(10.0f);
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
			m_pSun->GetSceneNode()->Translate(Vector3(-100.0f,0,0));
			m_pSun->GetSceneNode()->LookAt(Vector3(0,0,0),Vector3(0,1,0));
		}
		else if ( arg.key == OIS::KC_DOWN )
		{
			m_pSun->GetSceneNode()->Translate(Vector3(-100.0f,0,0));
			m_pSun->GetSceneNode()->LookAt(Vector3(0,0,0),Vector3(0,1,0));
		}

		return true;
	}

	bool SampleTerrain::keyReleased(const OIS::KeyEvent &arg)
	{
		return true;
	}

	void SampleTerrain::Update()
	{
// 		float fTime = GetTimer()->GetFrameDeltaTime();
// 		if ( GetInput()->IsKeyDown( OIS::KC_UP ) )
// 		{
// 			m_pSun->GetSceneNode()->Translate(Vector3(100.0f * fTime,0,0));
// 			m_pSun->GetSceneNode()->LookAt(Vector3(0,0,0),Vector3(0,1,0));
// 		}
// 		else if ( GetInput()->IsKeyDown( OIS::KC_DOWN ) )
// 		{
// 			m_pSun->GetSceneNode()->Translate(Vector3(-100.0f * fTime,0,0));
// 			m_pSun->GetSceneNode()->LookAt(Vector3(0,0,0),Vector3(0,1,0));
// 		}
	}

	void SampleTerrain::Render()
	{
		//m_pTerrain->Render();
	}

}


