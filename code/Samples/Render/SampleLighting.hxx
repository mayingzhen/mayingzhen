#include "SampleLighting.h"

namespace ma
{
	SampleLighting::SampleLighting()
	{
	}

	void SampleLighting::Load()
	{
		Vector3 vEyePos = Vector3(0, -600, 200);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->LookAt(vEyePos,VAtPos);
		
		m_pScene->SetRenderScheme(RenderScheme::DeferredLighting);

		RefPtr<Terrain> pTerrain = CreateTerrain("scene/terrain/test.xml");

		m_pScene->GetRootNode()->AddChild(pTerrain.get());

		// Render Mesh
		{
// 			RefPtr<SceneNode> pCharMagic = CreateSceneNode();
// 			
// 			SkinMeshComponent* pMeshComp = pCharMagic->CreateComponent<SkinMeshComponent>();
// 			pMeshComp->Load("magician/Body.skn","magician/Body.mtl");
// 			
// 			AnimationComponent* pAnimObj = pCharMagic->CreateComponent<AnimationComponent>();
// 			pAnimObj->Load("magician/Body.Aniset","magician/Body.ske");
// 			pAnimObj->PlayAnimation((UINT)0);
// 
// 			m_pScene->GetRootNode()->AddChild(pCharMagic.get());

			SceneNode* pCharMagic = m_pScene->CreateSceneNode("magician/magician/magician.xml");
			
			//pCharMagic->Right(100);

// 			RefPtr<SceneNode> pPlatform = CreateMeshGameObject("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mtl");
// 
// 			RefPtr<SceneNode> pBox = CreateMeshGameObject("Fbx/Box.skn","Fbx/Box.mtl");
// 
// 			pPlatform->Scale(50);
// 
// 			pBox->Scale(50);
		}

		// Light
		{
			SceneNode* pPointLight = m_pScene->CreateSceneNode(); 
			m_pPointLight = CreatePointLight();
			pPointLight->AddComponent(m_pPointLight.get());
			m_pPointLight->GetSceneNode()->Translate(Vector3(200, 0, 100));
			m_pPointLight->SetLightColor(ColourValue::White);
			m_pPointLight->SetLightIntensity(2.0f);
			m_pPointLight->SetRadius(100);
 
			SceneNode* pDirLight = m_pScene->CreateSceneNode(); 
			m_pDirectLight = CreateDirectonalLight();
			pDirLight->AddComponent(m_pDirectLight.get());
			m_pDirectLight->GetSceneNode()->LookAt(Vector3(10,-10,10),Vector3(0,0,0));
			m_pDirectLight->SetLightColor(ColourValue(1.0f,1.0f,1.0f,1.0f));
		}

		GetLightSystem()->SetAmbientColor(ColourValue::Black);
	}

	void SampleLighting::UnLoad()
	{
		GetLightSystem()->SetAmbientColor(ColourValue::White);	

		m_pDirectLight = NULL;
		m_pPointLight = NULL;
	}

	void SampleLighting::Update()
	{
		if (m_pPointLight)
		{
			float fDegree = 30 * GetTimer()->GetFrameDeltaTime();
			m_pPointLight->GetSceneNode()->RotateAround(Vector3::ZERO,Vector3::UNIT_Y,fDegree);
		}
	}

	void SampleLighting::Render()
	{
		if (m_pPointLight)
		{
			Matrix4 matWS = m_pPointLight->GetSceneNode()->GetMatrixWS();
			GetLineRender()->DrawWireSphere(matWS,m_pPointLight->GetRadius(),ColourValue(1,0,0,0));
		}

		if (m_pDirectLight)
		{
			Vector3 vSrc = Vector3(0,0,0);
			Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetSceneNode()->GetForward();
			GetLineRender()->DrawLine(vSrc,vDir,ColourValue(1,0,0,0));
		}
	}
}


