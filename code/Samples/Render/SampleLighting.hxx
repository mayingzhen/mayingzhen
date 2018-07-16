#include "SampleLighting.h"

namespace ma
{
	SampleLighting::SampleLighting()
	{
	}

	void SampleLighting::Load()
	{
		Vector3 vEyePos = Vector3(0, -6, 2);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->LookAt(vEyePos,VAtPos);
	

		RenderScheme* pRenderScheme = GetCamera()->GetScene()->GetRenderScheme();
		pRenderScheme->SetDeferredShadingEnabled(!pRenderScheme->GetDeferredShadingEnabled());

		//RefPtr<Terrain> pTerrain = CreateTerrain("scene/terrain/test.xml");
		//m_pScene->GetRootNode()->AddChild(pTerrain.get());

		RefPtr<SceneNode> pSceneNode = CreateSceneNode("magician/magician/magician.xml");
		pSceneNode->SetScale(Vector3(0.01f));
		m_pScene->GetRootNode()->AddChild(pSceneNode.get());
		
		m_pScene->GetMainDirLight()->GetSceneNode()->LookAt(Vector3(0,10,0),Vector3(0,0,0));
		m_pScene->GetMainDirLight()->SetLightColor(ColourValue(1.0f,1.0f,1.0f,1.0f));

		// Light
		if (1)
		{
			SceneNode* pPointLight = m_pScene->CreateSceneNode(); 
			m_pPointLight = CreatePointLight();
			pPointLight->AddComponent(m_pPointLight.get());
			m_pPointLight->GetSceneNode()->Translate(Vector3(2, 0, 1));
			m_pPointLight->SetLightColor(ColourValue::White);
			m_pPointLight->SetLightIntensity(2.0f);
			m_pPointLight->SetRadius(10);
 
			SceneNode* pDirLight = m_pScene->CreateSceneNode(); 
			m_pDirectLight = CreateDirectonalLight();
			pDirLight->AddComponent(m_pDirectLight.get());
			m_pDirectLight->GetSceneNode()->LookAt(Vector3(0,0,10),Vector3(0,0,0));
			m_pDirectLight->SetLightColor(ColourValue(1.0f,1.0f,1.0f,1.0f));
		}

		m_pScene->SetAmbientColor(Vector3::ZERO);
	}

	void SampleLighting::UnLoad()
	{
		m_pScene->SetAmbientColor(Vector3::UNIT_SCALE);	

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
			LineRender::DrawWireSphere(matWS,m_pPointLight->GetRadius(),ColourValue(1,0,0,0));
		}

		if (m_pDirectLight)
		{
			Vector3 vSrc = Vector3(0,0,0);
			Vector3 vDir = vSrc - 1000 * m_pDirectLight->GetSceneNode()->GetForward();
			LineRender::DrawLine(vSrc,vDir,ColourValue(1,0,0,0));
		}
	}

	void SampleLighting::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;

		if (key == Keyboard::KEY_D)
		{
			RenderScheme* pRenderScheme = GetCamera()->GetScene()->GetRenderScheme();
			pRenderScheme->SetDeferredShadingEnabled(!pRenderScheme->GetDeferredShadingEnabled());
		}
	}
}


