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
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);

		// Render Mesh
		{
			SceneNodePtr pCharMagic = m_pScene->CreateNode("magic");

			RefPtr<MeshComponent> pMeshComp = pCharMagic->CreateComponent<MeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.mat");

			AnimationComponentPtr pAnimObj = pCharMagic->CreateComponent<AnimationComponent>();
			pAnimObj->Load("magician/Body.Aniset","magician/Body.ske");
			pAnimObj->PlayAnimation((UINT)0);

			pCharMagic->Right(100);

			//SceneNodePtr pPlatform = m_pScene->CreateMeshGameObject("Fbx/MovingPlatform.skn","Fbx/MovingPlatform.mat");

			//SceneNodePtr pBox = m_pScene->CreateMeshGameObject("Fbx/Box.skn","Fbx/Box.mat");

			//pPlatform->GetSceneNode()->Scale(50);

			//pBox->GetSceneNode()->Scale(50);
		}

		// Light
		{
			SceneNodePtr pLightObj = m_pScene->CreateNode("Light");
			pLightObj->LookAt(Vector3(10,10,10),Vector3(0,0,0));
			m_pDirectLight = pLightObj->CreateComponent<DirectonalLight>();
			m_pDirectLight->SetLightColor(ColourValue::White);

			m_pDirectLight->SetCreateShadow(true);
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
		Vector3 vSrc = Vector3(0,0,0);
		Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetDirection();
		GetLineRender()->DrawLine(vSrc,vDir,ColourValue(1,0,0,0));
	}

}


