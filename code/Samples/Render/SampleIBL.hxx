#include "Samples/Render/SampleIBL.h"

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

		RefPtr<SceneNode> pSkyBoxNode = m_pScene->CreateSceneNode();
		RefPtr<SkyBox> pSkyBox = pSkyBoxNode->CreateComponent<SkyBox>();
		pSkyBox->SetCubeMap("env.dds");

		for (uint32_t iM = 0; iM < 1; ++iM)
		{
			for (uint32_t iR = 0; iR < 1; ++iR)
			{
				RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();

				pShpere->SetScale(Vector3(0.4f));
				pShpere->Translate(Vector3(iM + 0.0f, 0, iR + 0.0f));

				RefPtr<MeshComponent> pMeshComp = pShpere->CreateComponent<MeshComponent>();
				pMeshComp->Load("Fbx/shpere.skn","Fbx/Box.mtl");

				RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");
				RefPtr<Material> pMaterialInst = pMaterial->Clone();

				SubMaterial* pSubMaterial = pMaterialInst->GetSubByIndex(0);

				pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("SPEC", true);
				pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("IBL", true);

				RefPtr<SamplerState> pBRDFTerm = CreateSamplerState("brdf.dds", CLAMP, TFO_POINT, false);
				pSubMaterial->SetParameter("tBRDF", Any(pBRDFTerm));

				//RefPtr<SamplerState> pEnv = CreateSamplerState("env_filtered.dds", REPEAT, TFO_TRILINEAR, false);
				RefPtr<SamplerState> pEnv = CreateSamplerState(gFilterCube.GetOutTexture(), REPEAT, TFO_TRILINEAR, false);
				pSubMaterial->SetParameter("tEnv", Any(pEnv));

				int nMip = pEnv->GetTexture()->GetMipMapNumber();
				Vector2 u_diff_spec_mip(nMip, nMip - 1);

				pSubMaterial->SetParameter("u_diff_spec_mip", Any(u_diff_spec_mip));
		
				float u_metalness = (float)iM / 10.0f;
				float u_glossiness = (float)iR / 10.0f;
				pSubMaterial->SetParameter("u_metalness",Any(u_metalness));
				pSubMaterial->SetParameter("u_glossiness",Any(u_glossiness));

				pMeshComp->SetMaterial(pMaterialInst.get());
			}
		}
		

		m_pScene->SetAmbientColor(Vector3(0.0,0.0,0.0));
	}

	void SampleIBL::UnLoad()
	{

	}

	void SampleIBL::Render()
	{
		if (1)
		{
			gFilterCube.Render();
			//PrefilterCubeGPU("env.dds", "env_filtered_.dds");

			//GenIntegrateBRDF("BrdfTest_.dds");
		}
	}

	void SampleIBL::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;
	}


}



