#include "SampleShadowMap.h"

namespace ma
{
	SampleShadowMap::SampleShadowMap()
	{
		m_pDirectLight = NULL;
	}

	void SampleShadowMap::Load()
	{
		//GetInput()->AddKeyListener(this);

		Vector3 vEyePos = Vector3(0, 600, 600);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->LookAt(vEyePos,VAtPos);


		RefPtr<Terrain> pTerrain = CreateTerrain("scene/terrain/test.xml");
		m_pScene->GetRootNode()->AddChild(pTerrain.get());

		SceneNode* pCharMagic = m_pScene->CreateSceneNode("magician/magician/magician.xml");
		pCharMagic->SetPos(Vector3(150.0f,pTerrain->GetHeight(150.0f,200.0f),200.0f));
		
		SkinMeshComponent* pMeshComp = pCharMagic->GetTypeComponent<SkinMeshComponent>();
		pMeshComp->SetShadowCaster(true);

		// Render Mesh
		{
// 			RefPtr<SceneNode> pCharMagic = CreateSceneNode();
//  			MeshComponent* pMeshComp = pCharMagic->CreateComponent<MeshComponent>();
//  			pMeshComp->Load("magician/Body.skn","magician/Body.mat");
//  			pMeshComp->SetShadowCaster(true);
//  			AnimationComponent* pAnimObj = pCharMagic->CreateComponent<AnimationComponent>();
//  			pAnimObj->Load("magician/Body.Aniset","magician/Body.ske");
//  			pAnimObj->PlayAnimation((UINT)0);
// 			pCharMagic->RotateAround(Vector3::ZERO, Vector3::UNIT_X, -90);
//  			//pCharMagic->SetScale(Vector3(0.1f));
//  			pCharMagic->SetPos(Vector3(150.0f,pTerrain->GetHeight(150.0f,200.0f),200.0f));
// 			m_pScene->GetRootNode()->AddChild(pCharMagic.get());

//			SceneNode* pCharMagic = m_pScene->CreateNode("magician/magician.xml");


// 			SceneNode* pPlatform = m_pScene->CreateNode("platform");			
// 			MeshComponent* pPlatformMesh = pPlatform->CreateComponent<MeshComponent>();
// 			pPlatformMesh->Load("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mat");
// 			pPlatformMesh->SetShadowCaster(true);
// 			pPlatform->SetScale(Vector3(50));
// 			pPlatform->SetPos(Vector3(2.0f,2.0f,pTerrain->GetHeight(2.0f,2.0f)));

			m_pBox = CreateSceneNode();
			MeshComponent* pBoxMesh = m_pBox->CreateComponent<MeshComponent>();
			pBoxMesh->SetShadowCaster(true);
			pBoxMesh->Load("Fbx/Box.skn","Fbx/Box.mat");
			m_pBox->SetScale(Vector3(50));
			m_pBox->SetPos(Vector3(250.0f, pTerrain->GetHeight(250.0f, 250.0f), 250.0f));
			m_pScene->GetRootNode()->AddChild(m_pBox.get());
		}
		
		//m_pScene->GetSunShaow()->GetSceneNode()->LookAt(Vector3(1.f, 1.f, 0.f),Vector3::ZERO); 
		//m_pScene->GetSunShaow()->SetLightColor(ColourValue(0.49, 0.65, 0.95, 1.f));
		m_pScene->GetSunShaow()->SetEnabled(true);

	}

	void SampleShadowMap::UnLoad()
	{
		//GetInput()->RemoveKeyListener(this);

		GetLightSystem()->SetAmbientColor(ColourValue::White);		

		m_pDirectLight = NULL;
		m_pBox = NULL;
	}

	void SampleShadowMap::Update()
	{
// 		float fDegree = 30 * GetTimer()->GetFrameDeltaTime();
// 		m_pDirectLight->GetSceneNode()->RotateYAxisLS(fDegree);
	}

	void SampleShadowMap::Render()
	{
		if (m_pDirectLight)
		{
			Vector3 vSrc = Vector3(0,0,0);
			Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetForward();
			GetLineRender()->DrawLine(vSrc,vDir,ColourValue(1,0,0,0));
		}
	}

// 	bool SampleShadowMap::keyPressed(const OIS::KeyEvent &arg)
// 	{
// 		if (arg.key == OIS::KC_UP)
// 		{
// 			m_pBox->Up(100.0f);
// 		}
// 		else if (arg.key == OIS::KC_DOWN)
// 		{
// 			m_pBox->Up(-100.0f);
// 		}
// 		else if (arg.key == OIS::KC_LEFT)
// 		{
// 			m_pBox->Right(-100.0f);
// 		}
// 		else if (arg.key == OIS::KC_RIGHT)
// 		{
// 			m_pBox->Right(100.0f);
// 		}
// 
// 		return true;
// 	}

}


