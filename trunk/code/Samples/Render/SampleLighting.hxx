#include "SampleLighting.h"

namespace ma
{
	SampleLighting::SampleLighting()
	{
	}

	void SampleLighting::Load()
	{
		Vector3 vEyePos = Vector3(0, 400, 600);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);

		// Render Mesh
		{
			SceneNodePtr pCharMagic = m_pScene->CreateNode("magic");

			MeshComponentPtr pMeshComp = pCharMagic->CreateComponent<MeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.mat");

			AnimationComponentPtr pAnimObj = pCharMagic->CreateComponent<AnimationComponent>();
			pAnimObj->Load("magician/Body.Aniset","magician/Body.ske");
			pAnimObj->PlayAnimation((UINT)0);

			pCharMagic->Right(100);

// 			SceneNodePtr pPlatform = CreateMeshGameObject("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mat");
// 
// 			SceneNodePtr pBox = CreateMeshGameObject("Fbx/Box.skn","Fbx/Box.mat");
// 
// 			pPlatform->Scale(50);
// 
// 			pBox->Scale(50);
		}

		// Light
		{
 	
			SceneNodePtr pLightObj1 = m_pScene->CreateNode("Light1");
			pLightObj1->Translate(Vector3(200, 0, 100));
			m_pPointLight=  pLightObj1->CreateComponent<PointLight>();
			m_pPointLight->SetLightColor(ColourValue::White);
			m_pPointLight->SetRadius(300);
 
			SceneNodePtr pLightObj3 = m_pScene->CreateNode("Light2");
			pLightObj3->LookAt(Vector3(10,10,10),Vector3(0,0,0));
			m_pDirectLight = pLightObj3->CreateComponent<DirectonalLight>();
			m_pDirectLight->SetLightColor(ColourValue(0.2f,0.2f,0.2f,1.0f));
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
		float fDegree = 30 * GetTimer()->GetFrameDeltaTime();
		m_pPointLight->GetSceneNode()->RotateAround(Vector3::ZERO,Vector3::UNIT_Z,fDegree);
	}

	void SampleLighting::Render()
	{
		Matrix4 matWS = m_pPointLight->GetSceneNode()->GetWorldMatrix();
		GetLineRender()->DrawWireSphere(matWS,m_pPointLight->GetRadius(),ColourValue(1,0,0,0));

		Vector3 vSrc = Vector3(0,0,0);
		Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetDirection();
		GetLineRender()->DrawLine(vSrc,vDir,ColourValue(1,0,0,0));
		
	}
}


