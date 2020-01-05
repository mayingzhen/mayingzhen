#include "Samples/Render/SampleIBL.h"
#include "Application/ui.h"
//#include "nativefiledialog/src/include/nfd.h"

namespace ma
{
	SampleIBL::SampleIBL()
	{
	}

	PrefilterCube gFilterCube;

	void SampleIBL::Load()
	{

		GetCamera()->LookAt(Vector3(5, 3, -5), Vector3(0, 0, 0));

		//CreateMeshMaterial("FBX/Box.tga","FBX/Box.mtl", "IBL");

		//gFilterCube.Init("cube02.dds");

		{
			PostProcessPipeline* pPostProcess = GetPostProcess();
			PostProcess* pHDR = pPostProcess->GetPostProcess("HDR");

			PostProcessStep* pStepTonemap = pHDR->GetStep("ToneMap");
			SubMaterial* pToneMapMaterial = pStepTonemap->GetMaterial();
			RefPtr<MethodBinding> pMethodExpos = new MethodFunBinding<float>([this](Renderable*) { return m_fExporse; });
			pToneMapMaterial->SetParameter("gExposure", Any(pMethodExpos));

			PostProcessStep* pStepPrefiter = pHDR->GetStep("BloomPrefilter");
			SubMaterial* pFilterMaterial = pStepPrefiter->GetMaterial();
			RefPtr<MethodBinding> pMethodParam = new MethodFunBinding<Vector4>([this](Renderable*) { return Vector4(m_fParam); });
			pFilterMaterial->SetParameter("_Threshold", Any(pMethodParam));
		}


		RefPtr<SceneNode> pSkyBoxNode = m_pScene->CreateSceneNode();
		RefPtr<SkyBox> pSkyBox = pSkyBoxNode->CreateComponent<SkyBox>();
		pSkyBox->SetCubeMap("cube02.dds");

		RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();

		pShpere->SetScale(Vector3(0.4f));
		pShpere->Translate(Vector3(0.0f, 0.0f, 0.0f));

		RefPtr<MeshComponent> pMeshComp = pShpere->CreateComponent<MeshComponent>();
		pMeshComp->Load("Fbx/shpere.skn","Fbx/Box.mtl");

		SubMaterial* pSubMaterial = pMeshComp->GetSubMaterial(0);

		Technique* pTech = pSubMaterial->GetShadingTechnqiue();

		pTech->SetShaderMacroBool("SPEC", true);
		pTech->SetShaderMacroBool("IBL", true);
		pTech->ReLoad();

		RefPtr<SamplerState> pBRDFTerm = CreateSamplerState("brdf.dds", CLAMP, TFO_POINT, false);
		pSubMaterial->SetParameter("tBRDF", Any(pBRDFTerm));

		RefPtr<SamplerState> pEnv = CreateSamplerState("env_filtered.dds", REPEAT, TFO_TRILINEAR, false);
		//RefPtr<SamplerState> pEnv = CreateSamplerState(gFilterCube.GetOutTexture(), REPEAT, TFO_TRILINEAR, false);
		pSubMaterial->SetParameter("tEnv", Any(pEnv));

		int nMip = pEnv->GetTexture()->GetMipMapNumber();
		Vector2 u_diff_spec_mip(nMip, nMip - 1);
		pSubMaterial->SetParameter("u_diff_spec_mip", Any(u_diff_spec_mip));
		
		RefPtr<MethodBinding> pMethodMetallness = new MethodFunBinding<float>([this](Renderable*) { return m_fMetalness; });
		pSubMaterial->SetParameter("u_metalness",Any(pMethodMetallness) );

		RefPtr<MethodBinding> pMethodGlossness = new MethodFunBinding<float>([this](Renderable*) { return m_fGlossiness; });
		pSubMaterial->SetParameter("u_glossiness", Any(pMethodGlossness) );

		RefPtr<MethodBinding> pMethodDiffuseColor = new MethodFunBinding<Vector4>([this](Renderable*) { return m_cColor; });
		pSubMaterial->SetParameter("u_cDiffuseColor", Any(pMethodDiffuseColor));

		const Any& anyValue = pSubMaterial->GetParameter("u_texture")->GetValue();
		m_pDiffuseTexture = any_cast< RefPtr<SamplerState> >(anyValue);

		RefPtr<MethodBinding> pMethodDiffuseTexture = new MethodFunBinding< SamplerState* >([this](Renderable*) {return m_pDiffuseTexture.get();});
		pSubMaterial->SetParameter("u_texture", Any(pMethodDiffuseTexture));

		m_pScene->SetAmbientColor(ColourValue(0.0,0.0,0.0));
		m_pScene->GetMainDirLight()->GetSceneNode()->LookAt(Vector3(5, 3, -5), Vector3(0, 0, 0));
	}

	void SampleIBL::UnLoad()
	{

	}

	std::string PathAbsoluteToRelative(std::string strPathIn)
	{
		std::string strDir = GetArchiveMananger()->GetArchiveByIndex(0)->GetName();
		StaticFunc::StrFormatPath(strDir, false);

		StaticFunc::StrFormatPath(strPathIn, false);

		size_t nFind = strPathIn.find(strDir);
		if (nFind != std::string::npos)
		{
			return strPathIn.substr(nFind + strDir.length());
		}
		else
		{
			return "";
		}
	}

	void SampleIBL::Update()
	{
		GetUI()->Begin("Material"); 
		GetUI()->SliderFloat("Metalness", &m_fMetalness, 0.0f, 1.0f);           
		GetUI()->SliderFloat("Glossiness", &m_fGlossiness, 0.0f, 1.0f);
		GetUI()->ColorEdit4("diffuseColor", (float*)&m_cColor); // Edit 4 floats representing a color

		/*
		if (GetUI()->Button("diffuseTexutrePath:"))
		{
			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
			if (result == NFD_OKAY) 
			{
				std::string strTexture = PathAbsoluteToRelative(outPath);
				free(outPath);

				if (!strTexture.empty())
				{
					m_pDiffuseTexture = CreateSamplerState(strTexture.c_str());
				}
	
			}
		}
		*/

		GetUI()->End();

		GetUI()->Begin("HDR");
		GetUI()->SliderFloat("Exporse", &m_fExporse, 0.0f, 10.0f);
		GetUI()->SliderFloat("Param", &m_fParam, 0.0f, 20.0f);
		GetUI()->End();

	}

	void SampleIBL::PreRender()
	{
		if (0)
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



