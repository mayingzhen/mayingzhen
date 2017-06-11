#include "Samples/Render/SampleMaterial.h"

namespace ma
{
	SampleMaterial::SampleMaterial()
	{
	}

	void SampleMaterial::Load()
	{
		GetCamera()->LookAt(Vector3(0, 10, 0), Vector3(0, 0, 0));

		//CreateMeshMaterial("FBX/Box.tga","FBX/Box.mtl");
		
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
		}

		if (1)
		{
			RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();
			RefPtr<MeshComponent> pShpereMesh = pShpere->CreateComponent<MeshComponent>();
			pShpereMesh->Load("Fbx/shpere.skn","Fbx/Box.mtl");

			RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");
			RefPtr<Material> pClone = pMaterial->Clone();
			pShpereMesh->SetMaterial(pClone.get());

			SubMaterial* pSubMaterial = pClone->GetLodSubByIndex(0,0);
			pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("DIRLIGHT",true);
			pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("SPEC",true);
			//pSubMaterial->GetShadingTechnqiue()->SetShaderMacro("BRDF",true);

			//pSubMaterial->GetShadingTechnqiue()->SaveToXML("test.tech");

			RefPtr<UniformAnimation> pUniform = CreateUniformAnimation();
			pUniform->AddKeyFrame(0,Any(float(1.0f)));
			pUniform->AddKeyFrame(50,Any(float(20.0f)));
			pUniform->BuildFrames();
			float fSpecPower = 20.0f;

			pSubMaterial->SetParameter("u_cSpecColor", Any( Vector4(1,1,1,1) ) );
			pSubMaterial->SetParameter("u_roughness",Any(pUniform));

			//pShpere->Translate(Vector3(2,0,0));

		}

		if (0)
		{
			RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();
			RefPtr<MeshComponent> pShpereMesh = pShpere->CreateComponent<MeshComponent>();
			pShpereMesh->Load("Fbx/shpere.skn","Fbx/Box.mtl");

			RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");
			RefPtr<Material> pClone = pMaterial->Clone();
			pShpereMesh->SetMaterial(pClone.get());

			SubMaterial* pSubMaterial = pClone->GetLodSubByIndex(0,0);
			pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("DIRLIGHT",true);
			pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("SPEC",true);
			pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("BRDF",true);

			//pSubMaterial->GetShadingTechnqiue()->SaveToXML("test.tech");
			
			RefPtr<UniformAnimation> pUniform = CreateUniformAnimation();
			pUniform->AddKeyFrame(0,Any(float(1.0f)));
			pUniform->AddKeyFrame(50,Any(float(20.0f)));
			pUniform->BuildFrames();
			//float fSpecPower = 20.0f;
			
			pSubMaterial->SetParameter("u_cSpecColor", Any( Vector4(1,1,1,1) ) );
			pSubMaterial->SetParameter("u_roughness",Any(pUniform));

			pShpere->Translate(Vector3(2,0,0));

		}
		
		m_pScene->GetDirLight()->GetSceneNode()->LookAt(Vector3(10,0,10),Vector3(0,0,0));
		m_pScene->GetDirLight()->SetLightColor(ColourValue(1.0,1.0,1.0,1.0f));
		m_pScene->SetAmbientColor(Vector3(0.0,0.0,0.0));
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
			RenderScheme* pRenderScheme = m_pScene->GetRenderScheme();
			pRenderScheme->SetSMAAEnabled( !pRenderScheme->GetSMAAEnabled() );
		}
		else if (key == Keyboard::KEY_D)
		{
			RenderScheme* pRenderScheme = m_pScene->GetRenderScheme();
			pRenderScheme->SetDeferredShadingEnabled( !pRenderScheme->GetDeferredShadingEnabled() );
		}
	}


}


