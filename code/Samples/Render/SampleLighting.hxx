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
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos,vUp);

		// Render Mesh
		{
			GameObjectPtr pCharMagic = GetEntitySystem()->CreateGameObject("magic");

			MeshComponentPtr pMeshComp = pCharMagic->CreateComponent<MeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.mat");

			IAnimationObjectPtr pAnimObj = pCharMagic->CreateComponent<IAnimationObject>();
			pAnimObj->Load("magician/Body.Aniset","magician/Body.ske");
			pAnimObj->PlayAnimation((UINT)0);

			pCharMagic->GetSceneNode()->Right(100);

			GameObjectPtr pPlatform = CreateMeshGameObject("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mat");

			GameObjectPtr pBox = CreateMeshGameObject("Fbx/Box.skn","Fbx/Box.mat");

			pPlatform->GetSceneNode()->Scale(50);

			pBox->GetSceneNode()->Scale(50);
		}

		// Light
		{
 	
			GameObjectPtr pLightObj1 = GetEntitySystem()->CreateGameObject("Light1");
			pLightObj1->GetSceneNode()->Translate(Vector3(200, 100, 0));
			m_pPointLight=  pLightObj1->CreateComponent<PointLight>();
			m_pPointLight->SetLightColor(Vector4(1.0f,1.0f,1.0f,1.0f));
			m_pPointLight->SetRadius(300);
 
			GameObjectPtr pLightObj3 = GetEntitySystem()->CreateGameObject("Light2");
			pLightObj3->GetSceneNode()->LookAt(Vector3(10,10,10),Vector3(0,0,0),Vector3(0,1,0));
			m_pDirectLight = pLightObj3->CreateComponent<DirectonalLight>();
			m_pDirectLight->SetLightColor(Vector4(0.2f,0.2f,0.2f,1.0f));
		}

		GetLightSystem()->SetAmbientColor(Vector4(0.0f,0.0f,0.0f,0.0f));
	}

	void SampleLighting::UnLoad()
	{
		GetLightSystem()->SetAmbientColor(Vector4(1.0f,1.0f,1.0f,1.0f));	
	}

	void SampleLighting::Update()
	{
		float fDegree = 30 * GetTimer()->GetFrameDeltaTime();
		m_pPointLight->GetSceneNode()->RotateYAxisLS(fDegree);
	}

	void SampleLighting::Render()
	{
		Matrix4x4 matWS = m_pPointLight->GetSceneNode()->GetWorldMatrix();
		GetLineRender()->DrawWireSphere(matWS,m_pPointLight->GetRadius(),Color(1,0,0,0));

		Vector3 vSrc = Vector3(0,0,0);
		Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetDirection();
		GetLineRender()->DrawLine(vSrc,vDir,Color(1,0,0,0));
		
	}
}


