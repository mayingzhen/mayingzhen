#include "Samples/Render/SampleIBL.h"

namespace ma
{
	SampleIBL::SampleIBL()
	{
	}


	void SampleIBL::Load()
	{
		GetCamera()->LookAt(Vector3(0, -5, 3), Vector3(0, 0, 0));

		CreateMeshMaterial("FBX/Box.tga","FBX/Box.mtl");

		if (1)
		{
			//PrefilterCubeGPU("PaperMill_E_3k.dds","PaperMill_E_3k_filtered.dds");
			PrefilterCubeGPU("env_filtered.dds","env_filtered_filtered.dds");
		}

		UINT32 spheres_row = 1;
		UINT32 spheres_column = 1; 
		for (UINT32 i = 0; i < spheres_row; ++ i)
		{
			for (UINT32 j = 0; j < spheres_column; ++ j)
			{
				RefPtr<SceneNode> pShpere = m_pScene->CreateSceneNode();
				RefPtr<MeshComponent> pShpereMesh = pShpere->CreateComponent<MeshComponent>();
				pShpereMesh->Load("Fbx/shpere.skn","Fbx/Box.mtl");

				RefPtr<Material> pMaterial = CreateMaterial("Fbx/Box.mtl");
				RefPtr<Material> pClone = pMaterial->Clone();
				pShpereMesh->SetMaterial(pClone.get());

				SubMaterial* pSubMaterial = pClone->GetLodSubByIndex(0,0);
				pSubMaterial->GetShadingTechnqiue()->SetShaderMacro("LIGHT",true);
				pSubMaterial->GetShadingTechnqiue()->SetShaderMacro("SPEC",true);
				pSubMaterial->GetShadingTechnqiue()->SetShaderMacro("BRDF",true);
				pSubMaterial->GetShadingTechnqiue()->SetShaderMacro("ENVREFLECT",true);
				
				//pSubMaterial->GetShadingTechnqiue()->SaveToXML("test.tech");

				if (1)
				{
					RefPtr<SamplerState> pEnv = CreateSamplerState("env_filtered.dds",CLAMP,TFO_BILINEAR,false);
					pSubMaterial->SetParameter("tEnv",Any(pEnv));

					int nMip = pEnv->GetTexture()->GetMipMapNumber();
					Vector2 u_diff_spec_mip(nMip - 1, nMip - 2);
					float u_mip_bias = nMip / -2.0f;

					pSubMaterial->SetParameter("u_diff_spec_mip",Any(u_diff_spec_mip));
					pSubMaterial->SetParameter("u_mip_bias",Any(u_mip_bias));

					pSubMaterial->SetParameter("u_roughness",Any(120.f));
				}


				float x = spheres_column / -2.0f + j + 2.5f; 
				float y = spheres_row / -2.0f + i + 2.5f;
				pShpere->SetScale(Vector3(0.4f));
				pShpere->Translate(Vector3(x,y,0));
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



