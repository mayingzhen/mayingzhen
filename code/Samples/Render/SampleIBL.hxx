#include "Samples/Render/SampleIBL.h"
#include "Application/ui.h"

namespace ma
{
	SampleIBL::SampleIBL()
	{
	}

	PrefilterCube gFilterCube;

	void SampleIBL::Load()
	{

		GetCamera()->LookAt(Vector3(5, 3, -5), Vector3(0, 0, 0));
		GetCameraControll()->Init();

		CreateMeshMaterial("FBX/Box.tga","FBX/Box.mtl", "IBL");

		gFilterCube.Init("env.dds");

		//RefPtr<SceneNode> pSkyBoxNode = m_pScene->CreateSceneNode();
		//RefPtr<SkyBox> pSkyBox = pSkyBoxNode->CreateComponent<SkyBox>();
		//pSkyBox->SetCubeMap("env.dds");

		RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();

		pShpere->SetScale(Vector3(0.4f));
		pShpere->Translate(Vector3(0.0f, 0.0f, 0.0f));

		RefPtr<MeshComponent> pMeshComp = pShpere->CreateComponent<MeshComponent>();
		pMeshComp->Load("Fbx/shpere.skn","Fbx/Box.mtl");

		RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");
		RefPtr<Material> pMaterialInst = pMaterial->Clone();

		SubMaterial* pSubMaterial = pMaterialInst->GetSubByIndex(0);

		Technique* pTech = pSubMaterial->GetShadingTechnqiue();

		pTech->SetShaderMacroBool("SPEC", true);
		pTech->SetShaderMacroBool("IBL", true);

		RefPtr<SamplerState> pBRDFTerm = CreateSamplerState("brdf.dds", CLAMP, TFO_POINT, false);
		pSubMaterial->SetParameter("tBRDF", Any(pBRDFTerm));

		//RefPtr<SamplerState> pEnv = CreateSamplerState("env_filtered.dds", REPEAT, TFO_TRILINEAR, false);
		RefPtr<SamplerState> pEnv = CreateSamplerState(gFilterCube.GetOutTexture(), REPEAT, TFO_TRILINEAR, false);
		pSubMaterial->SetParameter("tEnv", Any(pEnv));

		int nMip = pEnv->GetTexture()->GetMipMapNumber();
		Vector2 u_diff_spec_mip(nMip, nMip - 1);

		pSubMaterial->SetParameter("u_diff_spec_mip", Any(u_diff_spec_mip));
		
		Uniform* pUniformMetal = pTech->GetUniform("u_metalness");
		pUniformMetal->SetMethodBinding( new MethodFunBinding<float>( [this](Renderable*){ return m_fMetalness; } ) );

		Uniform* pUniformGloss = pTech->GetUniform("u_glossiness");
		pUniformGloss->SetMethodBinding(new MethodFunBinding<float>( [this](Renderable*){ return m_fGlossiness; } ) );

		pMeshComp->SetMaterial(pMaterialInst.get());

		m_pScene->SetAmbientColor(Vector3(0.0,0.0,0.0));
	}

	void SampleIBL::UnLoad()
	{

	}

	void SampleIBL::Update()
	{
		GetUI()->Begin("Material"); 
		GetUI()->SliderFloat("Metalness", &m_fMetalness, 0.0f, 1.0f);           
		GetUI()->SliderFloat("Glossiness", &m_fGlossiness, 0.0f, 1.0f);
		//GetUI()->ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		GetUI()->End();

// 		m_pMaterial->SetParameter("u_metalness", Any(m_fMetalness));
// 		m_pMaterial->SetParameter("u_glossiness", Any(m_fGlossiness));
	}

	void SampleIBL::PreRender()
	{
		if (1)
		{
			gFilterCube.Render();
			//PrefilterCubeGPU("env.dds", "env_filtered_.dds");

			//GenIntegrateBRDF("BrdfTest_.dds");
		}
	}

	void SampleIBL::PostRender()
	{
	}

	void SampleIBL::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;
	}


}



