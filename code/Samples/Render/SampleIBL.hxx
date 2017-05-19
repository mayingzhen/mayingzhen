#include "Samples/Render/SampleIBL.h"

namespace ma
{
	SampleIBL::SampleIBL()
	{
	}


	void SampleIBL::Load()
	{
		GetCamera()->LookAt(Vector3(5, -5, 3), Vector3(0, 0, 0));

		CreateMeshMaterial("FBX/Box.tga","FBX/Box.mtl");

		if (0)
		{
			PrefilterCubeGPU("env.dds","env_filtered.dds");

			GenIntegrateBRDF("BrdfTest.dds");
		}

		/*
		for (uint32 iM = 0; iM < 10; ++iM)
		{
			for (uint32 iR = 0; iR < 10; ++iR)
			{
				RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();

				//float x = iM / -2.0f + j + 2.5f; 
				//float y = spheres_row / -2.0f + i + 2.5f;
				pShpere->SetScale(Vector3(0.4f));
				pShpere->Translate(Vector3(iM + 2.0f,iR + 2.0f,0));

				RefPtr<MeshComponent> pMeshComp = pShpere->CreateComponent<MeshComponent>();
				pMeshComp->Load("Fbx/shpere.skn","Fbx/Box.mtl");

				RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");
				RefPtr<Material> pClone = pMaterial->Clone();
				pMeshComp->SetMaterial(pClone.get());

				SubMaterial* pSubMaterial = pMeshComp->GetSubMaterial(0,0);

				pSubMaterial->GetShadingTechnqiue()->SetShaderMacro("ENVREFLECT",true);

				RefPtr<SamplerState> pEnv = CreateSamplerState("env_filtered.dds",REPEAT,TFO_TRILINEAR,false);
				pSubMaterial->SetParameter("tEnv",Any(pEnv));

				RefPtr<SamplerState> pBRDFTerm = CreateSamplerState("BRDF.dds",CLAMP,TFO_POINT,false);
				pSubMaterial->SetParameter("tBRDFTerm",Any(pBRDFTerm));

				//RefPtr<SamplerState> pMask = CreateSamplerState("zhouyu/body_mask.png");
				//pSubMaterial->SetParameter("tMask",Any(pMask));

				RefPtr<SamplerState> pMask = CreateSamplerState("pistol/Cerberus_RMAC.dds",REPEAT,TFO_TRILINEAR,false);
				//RefPtr<SamplerState> pMask = CreateSamplerState("zhouyu/body_mask.png",REPEAT,TFO_TRILINEAR,false);
				pSubMaterial->SetParameter("tSpecularRMCMap",Any(pMask));


				int nMip = pEnv->GetTexture()->GetMipMapNumber();
				Vector2 u_diff_spec_mip(nMip - 1, nMip - 2);
				//float u_mip_bias = nMip / -2.0f;

				pSubMaterial->SetParameter("u_diff_spec_mip",Any(u_diff_spec_mip));
				//pSubMaterial->SetParameter("u_mip_bias",Any(u_mip_bias));
		
				float u_metalness = iM / 10.0f;
				float u_roughness = iR / 10.0f;
				pSubMaterial->SetParameter("u_metalness",Any(u_metalness));
				pSubMaterial->SetParameter("u_roughness",Any(u_roughness));
			}
		}
		*/

	
		UINT32 spheres_row = 1;
		UINT32 spheres_column = 1; 
		for (UINT32 i = 0; i < spheres_row; ++ i)
		{
			for (UINT32 j = 0; j < spheres_column; ++ j)
			{
				RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();

				float x = spheres_column / -2.0f + j + 2.5f; 
				float y = spheres_row / -2.0f + i + 2.5f;
				pShpere->SetScale(Vector3(0.1f));
				//pShpere->Translate(Vector3(x,y,0));

				RefPtr<MeshComponent> pMeshComp = pShpere->CreateComponent<MeshComponent>();
				pMeshComp->Load("fbx/shpere.skn","pistol/Cerberus.mtl");


				SubMaterial* pSubMaterial = pMeshComp->GetSubMaterial(0,0);

				if (1)
				{
					pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("SPEC",true);
					pSubMaterial->GetShadingTechnqiue()->SetShaderMacroBool("IBL",true);
				}

				if (1)
				{
					RefPtr<SamplerState> pEnv = CreateSamplerState("env_filtered.dds",REPEAT,TFO_TRILINEAR,false);
					pSubMaterial->SetParameter("tEnv",Any(pEnv));

					RefPtr<SamplerState> pBRDFTerm = CreateSamplerState("BRDFTest.dds",CLAMP,TFO_BILINEAR,false);
					pSubMaterial->SetParameter("tBRDF",Any(pBRDFTerm));

					int nMip = pEnv->GetTexture()->GetMipMapNumber();
					Vector2 u_diff_spec_mip(nMip, nMip - 1);

					pSubMaterial->SetParameter("u_diff_spec_mip",Any(u_diff_spec_mip));

					pSubMaterial->SetParameter("u_metalness",Any(1.0f));
					pSubMaterial->SetParameter("u_glossiness",Any(0.5f));
				}
			}
		}
		

		m_pScene->SetAmbientColor(Vector3(0.0,0.0,0.0));
	}

	void SampleIBL::UnLoad()
	{
	}

	void SampleIBL::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;
	}


}



