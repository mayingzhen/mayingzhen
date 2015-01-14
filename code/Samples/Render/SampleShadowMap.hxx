#include "SampleShadowMap.h"

namespace ma
{
	SampleShadowMap::SampleShadowMap()
	{
		m_pDirectLight = NULL;
	}

	void SampleShadowMap::Load()
	{
		Vector3 vEyePos = Vector3(0, -400, 600);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);
		
		// Render Mesh
		{
			SceneNode* pCharMagic = m_pScene->CreateNode("magic");
			MeshComponent* pMeshComp = pCharMagic->CreateComponent<MeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.mat");
			pMeshComp->SetShadowCaster(true);
 			AnimationComponent* pAnimObj = pCharMagic->CreateComponent<AnimationComponent>();
 			pAnimObj->Load("magician/Body.Aniset","magician/Body.ske");
 			pAnimObj->PlayAnimation((UINT)0);
 			pCharMagic->SetPos(Vector3(200,0,00));

			SceneNode* pPlatform = m_pScene->CreateNode("platform");			
			MeshComponent* pPlatformMesh = pPlatform->CreateComponent<MeshComponent>();
			pPlatformMesh->Load("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mat");
			pPlatformMesh->SetShadowCaster(true);

			SceneNode* pBox = m_pScene->CreateNode("box");			
			MeshComponent* pBoxMesh = pBox->CreateComponent<MeshComponent>();
			pBoxMesh->SetShadowCaster(true);
			pBoxMesh->Load("Fbx/Box.skn","Fbx/Box.mat");
			pBox->SetScale(Vector3(50));

			RefPtr<Terrain> pTerrain = CreateTerrain("scene/terrain/test.xml");
			m_pScene->AddChild(pTerrain.get());
		}
		
		m_pScene->SetDirectionalLight(ColourValue(0.49, 0.65, 0.95, 1.f), Vector3(-1.f, -0.f, -1.f));
		GetRenderShadowCSM()->SetEnabled(true);
		// Light
		{
// 			SceneNodePtr pLightObj = m_pScene->CreateNode("Light");
// 			pLightObj->LookAt(Vector3(10,10,10),Vector3(0,0,0));
// 			m_pDirectLight = pLightObj->CreateComponent<DirectonalLight>();
// 			m_pDirectLight->SetLightColor(ColourValue::White);

			//m_pDirectLight->SetCreateShadow(true);
		}

		GetLightSystem()->SetAmbientColor(ColourValue(0.2f,0.2f,0.2f,0.0f));

	}

	void SampleShadowMap::UnLoad()
	{
		GetLightSystem()->SetAmbientColor(ColourValue::White);		

		m_pDirectLight = NULL;
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
			Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetDirection();
			GetLineRender()->DrawLine(vSrc,vDir,ColourValue(1,0,0,0));
		}
	}

}


