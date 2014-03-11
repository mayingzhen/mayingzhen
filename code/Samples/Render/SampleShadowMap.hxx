#include "SampleShadowMap.h"

namespace ma
{
	SampleShadowMap::SampleShadowMap()
	{
		m_pDirectLight = NULL;
	}

	void SampleShadowMap::Load()
	{
		Vector3 vEyePos = Vector3(0, 400, 600);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos,vUp);

		// Render Mesh
		{
			GameObjectPtr pCharMagic = GetEntitySystem()->CreateGameObject("magic");

			MeshComponent* pMeshComp = pCharMagic->CreateComponent<MeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.mat");

			IAnimationObject* pAnimObj = pCharMagic->CreateComponent<IAnimationObject>();
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
			GameObjectPtr pLightObj = GetEntitySystem()->CreateGameObject("Light");
			pLightObj->GetSceneNode()->LookAt(Vector3(10,10,10),Vector3(0,0,0),Vector3(0,1,0));
			m_pDirectLight = pLightObj->CreateComponent<DirectonalLight>();
			m_pDirectLight->SetLightColor(Vector4(1.0f,1.0f,1.0f,1.0f));

			m_pDirectLight->SetCreateShadow(true);
		}

		GetLightSystem()->SetAmbientColor(Vector4(0.2f,0.2f,0.2f,0.0f));

	}

	void SampleShadowMap::UnLoad()
	{
		GetLightSystem()->SetAmbientColor(Vector4(1.0f,1.0f,1.0f,1.0f));			
	}

	void SampleShadowMap::Update()
	{
// 		float fDegree = 30 * GetTimer()->GetFrameDeltaTime();
// 		m_pDirectLight->GetSceneNode()->RotateYAxisLS(fDegree);
	}

	void SampleShadowMap::Render()
	{
		Vector3 vSrc = Vector3(0,0,0);
		Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetDirection();
		GetLineRender()->DrawLine(vSrc,vDir,Color(1,0,0,0));
	}

}


