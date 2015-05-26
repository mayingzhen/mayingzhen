#include "SampleLighting.h"

namespace ma
{
	SampleLighting::SampleLighting()
	{
	}

	void SampleLighting::Load()
	{
		Vector3 vEyePos = Vector3(0, 600, 600);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);

		// Render Mesh
		{
// 			RefPtr<SceneNode> pCharMagic = CreateSceneNode();
// 			
// 			SkinMeshComponent* pMeshComp = pCharMagic->CreateComponent<SkinMeshComponent>();
// 			pMeshComp->Load("magician/Body.skn","magician/Body.mat");
// 			
// 			AnimationComponent* pAnimObj = pCharMagic->CreateComponent<AnimationComponent>();
// 			pAnimObj->Load("magician/Body.Aniset","magician/Body.ske");
// 			pAnimObj->PlayAnimation((UINT)0);
// 
// 			m_pScene->GetRootNode()->AddChild(pCharMagic.get());

			SceneNode* pCharMagic = m_pScene->CreateNode("magician/magician.xml");
			
			//pCharMagic->Right(100);

// 			RefPtr<SceneNode> pPlatform = CreateMeshGameObject("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mat");
// 
// 			RefPtr<SceneNode> pBox = CreateMeshGameObject("Fbx/Box.skn","Fbx/Box.mat");
// 
// 			pPlatform->Scale(50);
// 
// 			pBox->Scale(50);
		}

		// Light
		{
 	
			RefPtr<SceneNode> pLightObj1 = CreateSceneNode();
			pLightObj1->Translate(Vector3(200, 0, 100));
			m_pPointLight=  pLightObj1->CreateComponent<PointLight>();
			m_pPointLight->SetLightColor(ColourValue::White);
			m_pPointLight->SetLightIntensity(2.0f);
			m_pPointLight->SetRadius(300);
			m_pScene->GetRootNode()->AddChild(pLightObj1.get());
 
// 			RefPtr<SceneNode> pLightObj2 = m_pScene->CreateNode("Light2");
// 			pLightObj2->LookAt(Vector3(10,10,10),Vector3(0,0,0));
// 			m_pDirectLight = pLightObj2->CreateComponent<DirectonalLight>();
// 			m_pDirectLight->SetLightColor(ColourValue(0.5f,0.5f,0.5f,1.0f));
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
			//m_pPointLight->GetSceneNode()->RotateAround(Vector3::ZERO,Vector3::UNIT_Z,fDegree);
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
			Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetDirection();
			GetLineRender()->DrawLine(vSrc,vDir,ColourValue(1,0,0,0));
		}
	}
}


