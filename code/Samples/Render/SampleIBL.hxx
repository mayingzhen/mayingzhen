#include "Samples/Render/SampleIBL.h"

namespace ma
{
	SampleIBL::SampleIBL()
	{
	}


	void SampleIBL::Load()
	{
		GetCamera()->LookAt(Vector3(5, -5, 3), Vector3(0, 0, 0));

		CreateMeshMaterial("FBX/Box.tga","FBX/Box.mtl", "IBL");

		if (0)
		{
			PrefilterCubeGPU("env.dds","env_filtered_.dds");

			GenIntegrateBRDF("BrdfTest_.dds");
		}

	
		for (uint32_t iM = 0; iM < 10; ++iM)
		{
			for (uint32_t iR = 0; iR < 10; ++iR)
			{
				RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();


				pShpere->SetScale(Vector3(0.4f));
				pShpere->Translate(Vector3(iM + 2.0f,iR + 2.0f,0));

				RefPtr<MeshComponent> pMeshComp = pShpere->CreateComponent<MeshComponent>();
				pMeshComp->Load("Fbx/shpere.skn","Fbx/Box.mtl");

				RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");

				SubMaterial* pSubMaterial = pMaterial->GetLodSubByIndex(0,0);

				//pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("SPEC", true);
				//pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("IBL", true);

				RefPtr<SamplerState> pBRDFTerm = CreateSamplerState("BRDFTest.dds", CLAMP, TFO_BILINEAR, false);
				pSubMaterial->SetParameter("tBRDF", Any(pBRDFTerm));

				RefPtr<SamplerState> pEnv = CreateSamplerState("env_filtered.dds", REPEAT, TFO_TRILINEAR, false);
				pSubMaterial->SetParameter("tEnv", Any(pEnv));

				int nMip = pEnv->GetTexture()->GetMipMapNumber();
				Vector2 u_diff_spec_mip(nMip, nMip - 1);

				pSubMaterial->SetParameter("u_diff_spec_mip", Any(u_diff_spec_mip));
		
				float u_metalness = (float)iM / 10.0f;
				float u_glossiness = (float)iR / 10.0f;
				pSubMaterial->SetParameter("u_metalness",Any(u_metalness));
				pSubMaterial->SetParameter("u_glossiness",Any(u_glossiness));

				pMeshComp->SetMaterial(pMaterial->Clone().get());
			}
		}
		

		m_pScene->SetAmbientColor(Vector3(0.0,0.0,0.0));
	}

	void SampleIBL::UnLoad()
	{

	}

	void SampleIBL::Render()
	{
		if (0)
		{
			PrefilterCubeGPU("env.dds", "env_filtered_.dds");

			GenIntegrateBRDF("BrdfTest_.dds");
		}
	}

	void SampleIBL::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;
	}


}



