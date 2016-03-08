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

		Vector3 vEyePos = Vector3(0, -600, 600);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->LookAt(vEyePos,VAtPos);


		RefPtr<Terrain> pTerrain = CreateTerrain("scene/terrain/test.xml");
		m_pScene->GetRootNode()->AddChild(pTerrain.get());

		SceneNode* pCharMagic = m_pScene->CreateSceneNode("magician/magician/magician.xml");
		pCharMagic->SetPos(Vector3(150.0f,200.0f,pTerrain->GetHeight(150.0f,200.0f)));
		SkinMeshComponent* pMeshComp = pCharMagic->GetTypeComponent<SkinMeshComponent>();
		pMeshComp->SetShadowCaster(true);

		m_pBox = m_pScene->CreateSceneNode();
		MeshComponent* pBoxMesh = m_pBox->CreateComponent<MeshComponent>();
		pBoxMesh->SetShadowCaster(true);
		pBoxMesh->Load("Fbx/Box.skn","Fbx/Box.mtl");
		m_pBox->SetScale(Vector3(50));
		m_pBox->SetPos(Vector3(250.0f, 250.0f,pTerrain->GetHeight(250.0f, 250.0f)));

		RefPtr<DirectonalLight> pSunLight = m_pScene->GetDirLight();
		
		pSunLight->GetSceneNode()->LookAt(Vector3(1.f, 0.f, 1.f),Vector3::ZERO); 
		pSunLight->SetLightColor(ColourValue(0.49, 0.65, 0.95, 1.f));
		pSunLight->SetShadowEnabled(true);

	}

	void SampleShadowMap::UnLoad()
	{
		//GetInput()->RemoveKeyListener(this);

		//m_pScene->SetAmbientColor(ColourValue::White);		

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
			Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetSceneNode()->GetForward();
			LineRender::DrawLine(vSrc,vDir,ColourValue(1,0,0,0));
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


