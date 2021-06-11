#include "Samples/Render/SampleMaterial.h"

namespace ma
{
	SampleMaterial::SampleMaterial()
	{
	}

	void SampleMaterial::Load()
	{
		//GetCamera()->LookAt(Vector3(0, 10, 0), Vector3(0, 0, 0));

		GetCamera()->LookAt(Vector3(0, 0, -20), Vector3(0, 0, 0));

		if (1)
		{
			CreateMeshMaterial("FBX/Box.tga", "FBX/Box.mtl");

			CreateMeshMaterial("magician/magician/body.tga", "magician/magician/Body.mtl", "SKIN;LIGHT;BRDF;SPEC");
		}
		
		if (1);
		{
			for (uint32_t i = 1; i < 4; ++i)
			{
				SceneNode* pBox = m_pScene->CreateSceneNode();
				MeshComponent* pBoxMesh = pBox->CreateComponent<MeshComponent>();
				pBoxMesh->SetSuportInstance(true);
				pBoxMesh->Load("Fbx/Box.skn", "Fbx/Box.mtl");
				//pBoxMesh->SetShadowCaster(true);
                float x = (float)i + 2.0f;//Math::RangeRandom(0, 150);
                float y = (float)i + 2.0f;//Math::RangeRandom(0, 150);
				pBox->SetPos(Vector3(x, y,0));
			}
		}

		if (0)
		{
			RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();
			RefPtr<MeshComponent> pShpereMesh = pShpere->CreateComponent<MeshComponent>();
			pShpereMesh->Load("Fbx/Box.skn","Fbx/Box.mtl");
		}

		if (0)
		{
			RefPtr<SceneNode> pCharMagic = m_pScene->CreateSceneNode();
			pCharMagic->SetScale(Vector3(0.01f));
			SkinMeshComponent* pMeshComp = pCharMagic->CreateComponent<SkinMeshComponent>();
			pMeshComp->Load("magician/magician/Body.skn","magician/magician/Body.mtl");

			AnimationComponent* pAnimtionObjectB = pCharMagic->CreateComponent<AnimationComponent>();
			pAnimtionObjectB->Load("magician/magician/body.aniset","magician/magician/Body.ske");
			//pAnimtionObjectB->SetAnimation("100");
		}

		if (0)
		{
			RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();
			RefPtr<MeshComponent> pShpereMesh = pShpere->CreateComponent<MeshComponent>();
			pShpereMesh->Load("Fbx/Box.skn","Fbx/Box.mtl");
			pShpere->Translate(Vector3(2,0,0));

// 			RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");
// 			RefPtr<Material> pClone = pMaterial->Clone();
// 			pShpereMesh->SetMaterial(pClone.get());

			//SubMaterial* pSubMaterial = pClone->GetLodSubByIndex(0,0);
			//pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("DIRLIGHT",true);
			//pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("SPEC",true);
			//pSubMaterial->GetShadingTechnqiue()->SetShaderMacro("BRDF",true);

			//pSubMaterial->GetShadingTechnqiue()->SaveToXML("test.tech");

			//RefPtr<UniformAnimation> pUniform = CreateUniformAnimation();
			//pUniform->AddKeyFrame(0,Any(float(1.0f)));
			//pUniform->AddKeyFrame(50,Any(float(20.0f)));
			//pUniform->BuildFrames();
			//float fSpecPower = 20.0f;

			//pSubMaterial->SetParameter("u_cSpecColor", Any( Vector4(1,1,1,1) ) );
			//pSubMaterial->SetParameter("u_roughness",Any(pUniform));

			//pShpere->Translate(Vector3(2,0,0));

		}

		if (1)
		{
			for (uint32_t i = 0; i < 40; ++i)
			{
				RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();
				RefPtr<MeshComponent> pShpereMesh = pShpere->CreateComponent<MeshComponent>();
				pShpereMesh->Load("Fbx/shpere.skn", "Fbx/Box.mtl");

				RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");
				RefPtr<Material> pClone = pMaterial->Clone();
				pShpereMesh->SetMaterial(pClone.get());

				if (0)
				{
					SubMaterial* pSubMaterial = pClone->GetSubByIndex(0);
					pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("DIRLIGHT", true);
					pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("SPEC", true);
					pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("BRDF", true);

					//pSubMaterial->GetShadingTechnqiue()->SaveToXML("test.tech");

					RefPtr<UniformAnimation> pUniform = CreateUniformAnimation();
					pUniform->AddKeyFrame(0, Any(float(1.0f)));
					pUniform->AddKeyFrame(50, Any(float(20.0f)));
					pUniform->BuildFrames();
					//float fSpecPower = 20.0f;

					pSubMaterial->SetParameter("u_cSpecColor", Any(Vector4(1, 1, 1, 1)));
					pSubMaterial->SetParameter("u_roughness", Any(pUniform));
				}

				pShpere->Translate(Vector3(0, i, i));
			}
		}
		
		m_pScene->GetMainDirLight()->GetSceneNode()->LookAt(Vector3(10,0,10),Vector3(0,0,0));
		m_pScene->GetMainDirLight()->SetLightColor(ColourValue(1.0,1.0,1.0,1.0f));
		m_pScene->SetAmbientColor(ColourValue(0.1,0.1,0.1));
	}

	void SampleMaterial::UnLoad()
	{
	}

	void SampleMaterial::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;

		if (key == Keyboard::KEY_A)
		{
			//RenderScheme* pRenderScheme = m_pScene->GetRenderScheme();
			//pRenderScheme->SetSMAAEnabled( !pRenderScheme->GetSMAAEnabled() );
		}
		else if (key == Keyboard::KEY_D)
		{
			//RenderScheme* pRenderScheme = m_pScene->GetRenderScheme();
			//pRenderScheme->SetDeferredShadingEnabled( !pRenderScheme->GetDeferredShadingEnabled() );
		}
	}


}


