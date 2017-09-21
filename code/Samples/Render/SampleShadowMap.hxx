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

		Vector3 vEyePos = Vector3(0, -60, 60);
		Vector3 VAtPos = Vector3(0, 0, 0);
		GetCamera()->LookAt(vEyePos, VAtPos);

		m_pScene->GetDirLight()->GetSceneNode()->LookAt(Vector3(10, 10, 10), Vector3(0, 0, 0));
		m_pScene->GetDirLight()->SetLightColor(ColourValue(1.0, 1.0, 1.0, 1.0f));
		m_pScene->GetDirLight()->SetSplitPosParam(Vector4(500.0f));
		m_pScene->GetDirLight()->SetShadowEnabled(true);
		m_pScene->SetAmbientColor(Vector3(0.0, 0.0, 0.0));

		m_pTerrain = CreateTerrain("scene/terrain/test.xml");
		m_pScene->GetRootNode()->AddChild(m_pTerrain.get());

		for (uint32 i = 0; i < 2; ++i)
		{
			SceneNode* pCharMagic = m_pScene->CreateSceneNode("magician/magician/magician.xml");
			pCharMagic->SetScale(Vector3(0.01f));
			float x = Math::RangeRandom(0, 10);
			float y = Math::RangeRandom(0, 10);
			pCharMagic->SetPos(Vector3(x, y, m_pTerrain->GetHeight(x, y)));
			pCharMagic->SetPos(Vector3(1.5f, 2.0f, m_pTerrain->GetHeight(1.50f, 2.0f)));
			SkinMeshComponent* pMeshComp = pCharMagic->GetTypeComponent<SkinMeshComponent>();
			pMeshComp->SetShadowCaster(true);
			AnimationComponent* pAnimComp = pCharMagic->GetTypeComponent<AnimationComponent>();
			pAnimComp->SetAnimation(100);
		}

		for (uint32 i = 0; i < 2; ++i)
		{
			SceneNode* pBox = m_pScene->CreateSceneNode();
			MeshComponent* pBoxMesh = pBox->CreateComponent<MeshComponent>();
			pBoxMesh->Load("Fbx/Box.skn", "Fbx/Box.mtl");
			pBoxMesh->SetShadowCaster(true);
			float x = Math::RangeRandom(0, 10);
			float y = Math::RangeRandom(0, 10);
			pBox->SetPos(Vector3(x, y, m_pTerrain->GetHeight(x, y)));
		}

		m_pTerrain->mLoadOverEvent.notify(this, &SampleShadowMap::OnTerrainLoadOver);
	}

	void SampleShadowMap::OnTerrainLoadOver()
	{
		for (uint32 i = 0; i < 200; ++i)
		{
			SceneNode* pCharMagic = m_pScene->CreateSceneNode("magician/magician/magician.xml");
			pCharMagic->SetScale(Vector3(0.01f));
			float x = Math::RangeRandom(0, 150);
			float y = Math::RangeRandom(0, 150);
			pCharMagic->SetPos(Vector3(x, y, m_pTerrain->GetHeight(x, y)));
			pCharMagic->SetPos(Vector3(1.5f, 2.0f, m_pTerrain->GetHeight(1.50f, 2.0f)));
// 			SkinMeshComponent* pMeshComp = pCharMagic->GetTypeComponent<SkinMeshComponent>();
// 			pMeshComp->SetShadowCaster(true);
// 			AnimationComponent* pAnimComp = pCharMagic->GetTypeComponent<AnimationComponent>();
// 			pAnimComp->SetAnimation(100);
		}

		for (uint32 i = 0; i < 200; ++i)
		{
			SceneNode* pBox = m_pScene->CreateSceneNode();
			MeshComponent* pBoxMesh = pBox->CreateComponent<MeshComponent>();
			pBoxMesh->SetShadowCaster(true);
			pBoxMesh->Load("Fbx/Box.skn", "Fbx/Box.mtl");
			float x = Math::RangeRandom(0, 150);
			float y = Math::RangeRandom(0, 150);
			pBox->SetPos(Vector3(x, y, m_pTerrain->GetHeight(x, y)));
		}


		RefPtr<DirectonalLight> pSunLight = m_pScene->GetDirLight();

		pSunLight->GetSceneNode()->LookAt(Vector3(10, 10, 10), Vector3(0, 0, 0));
		pSunLight->SetLightColor(ColourValue(1.0, 1.0, 1.0, 1.0f));
		//pSunLight->SetShadowEnabled(true);
		//pSunLight->SetMaxSplitCount(4);
		//m_pScene->SetAmbientColor(Vector3(0.0,0.0,0.0));

		//m_pScene->GetRenderScheme()->SetDeferredShadingEnabled(true);
		//m_pScene->GetRenderScheme()->SetDeferredShadowEnabled(true);
	}

	void SampleShadowMap::UnLoad()
	{
		//GetInput()->RemoveKeyListener(this);

		//m_pScene->SetAmbientColor(ColourValue::White);

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
			Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetSceneNode()->GetForward();
			LineRender::DrawLine(vSrc,vDir,ColourValue(1,0,0,0));
		}
	}

	void SampleShadowMap::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;

		if (key == Keyboard::KEY_S)
		{
			RefPtr<DirectonalLight> pSunLight = m_pScene->GetDirLight();

			pSunLight->SetShadowEnabled( !pSunLight->GetShadowEnabled() );
		}
	}

}


