#include "SampleTerrain.h"


namespace ma
{
	SampleTerrain::SampleTerrain()
	{
	}


	void SampleTerrain::Load()
	{
 		Vector3 lookAtPos(100,0,0);
 		Vector3 eyePos = Vector3(100, 360, -60);
 		GetCamera()->LookAt(eyePos,lookAtPos);

		//GetResourceSystem()->SetDataThreadEnable(true);

		//RenderScheme* pRenderScheme = m_pScene->GetRenderScheme();
		//pRenderScheme->SetDeferredShadingEnabled(true);

		if (1)
		{
			RefPtr<Terrain> pTerrain = CreateTerrain();

			pTerrain->SetTrunkSize(64);
			pTerrain->SetHeightSpcing(50);
			pTerrain->SetStartPoint(Vector3(0, 0, 0));

			//RefPtr<SamplerState> pEnv = CreateSamplerState("env_filtered.dds",CLAMP,TFO_BILINEAR,false);
			
			//int nMip = pEnv->GetTexture()->GetMipMapNumber();
			//Vector2 u_diff_spec_mip(nMip - 1, nMip - 2);
			//float u_mip_bias = nMip / -2.0f;

			if(1)
			{
                VertexElement element[3];
                element[0] = VertexElement(0, 0, DT_SHORT4N, DU_POSITION, 0);
                element[1] = VertexElement(0, 8, DT_SHORT2N, DU_TEXCOORD, 0);
                element[2] = VertexElement(0, 12, DT_UBYTE4N, DU_TANGENT, 0);
                RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 3);
                
				ShaderCreateInfo info;
				info.m_strVSFile = "terrain.hlsl:vs_main";
				info.m_strPSFile = "terrain.hlsl:ps_main";
				info.m_pVertexDecl = pDeclaration;
				RefPtr<Technique> pShadingTech = CreateTechnique(info);

				pShadingTech->SaveToXML("shader/terrain.tech");
			}

			RefPtr<Material> testMaterial = CreateMaterial();
			{
				RefPtr<SubMaterial> subMaterial = CreateSubMaterial();
				testMaterial->AddSubMaterial(subMaterial.get());

				RefPtr<Technique> pShadingTech = CreateTechnique("shader/terrain.tech", "LAYER 2;DIRLIGHT",nullptr);

				subMaterial->SetShadingTechnqiue(pShadingTech.get());
				
				subMaterial->SetParameter("tDetailMap0", Any( CreateSamplerState("scene/terrain/chess.dds") ) );
				subMaterial->SetParameter("tDetailMap1", Any( CreateSamplerState("scene/terrain/diban_zhuanshi.dds") ) );
				//subMaterial->SetParameter("tBlendingMap", Any( CreateSamplerState("scene/terrain/test_b0.dds",CLAMP,TFO_POINT,false) ) );
				subMaterial->SetParameter("uDetailScale", Any( Vector2(0.1f, 0.1f) ) );
				subMaterial->SetParameter("uDetailOffSet", Any( Vector4::ZERO) );
				subMaterial->SetParameter("u_cDiffuseColor", Any( Vector4::ONE ) );
				subMaterial->SetParameter("u_cSpecColor", Any( Vector4::ZERO ));
				subMaterial->SetParameter("u_roughness",Any(0.0f));

				//subMaterial->SetParameter("tEnv",Any(pEnv));
				//subMaterial->SetParameter("u_diff_spec_mip",Any(u_diff_spec_mip));
				//subMaterial->SetParameter("u_mip_bias",Any(u_mip_bias));
			}

			{
				RefPtr<SubMaterial> subMaterial = CreateSubMaterial();
				testMaterial->AddSubMaterial(subMaterial.get());

				//subMaterial->SetShadingTechnqiue("terrain","LAYER 1");

				RefPtr<Technique> pNewShadingTech = CreateTechnique("shader/terrain.tech", "LAYER 1", nullptr);

				subMaterial->SetShadingTechnqiue(pNewShadingTech.get());

				Technique* pShadingTech = subMaterial->GetShadingTechnqiue();
				pShadingTech->SetShaderMacroBool("DIRLIGHT",true);
				pShadingTech->SetShaderMacroBool("SPEC",true);
				pShadingTech->SetShaderMacroBool("BRDF",true);
				pShadingTech->SetShaderMacroBool("BUMPMAP",true);
				pShadingTech->SetShaderMacroBool("PARALLAXMAPPING",true);
				

				subMaterial->SetParameter("tDetailMap0", Any( CreateSamplerState("scene/terrain/wall.jpg") ) );
				subMaterial->SetParameter("tBumpMap0", Any( CreateSamplerState("scene/terrain/wall_NM_height.tga",REPEAT,TFO_TRILINEAR,false) ) );
				subMaterial->SetParameter("uDetailScale", Any(Vector2(0.1f, 0.1f) ) );
				subMaterial->SetParameter("uDetailOffSet", Any(Vector4::ZERO) );
				subMaterial->SetParameter("u_cDiffuseColor", Any( Vector4::ONE ) );
				subMaterial->SetParameter("u_cSpecColor", Any( Vector4::ONE ) );
				subMaterial->SetParameter("u_roughness",Any(00.0f));
			}

			{
				RefPtr<SubMaterial> subMaterial = CreateSubMaterial();
				testMaterial->AddSubMaterial(subMaterial.get());

				//subMaterial->SetShadingTechnqiue("terrain","LAYER 1");

				RefPtr<Technique> pShadingTech = CreateTechnique("shader/terrain.tech", "LAYER 1",nullptr);

				subMaterial->SetShadingTechnqiue(pShadingTech.get());

				subMaterial->SetParameter("tDetailMap0", Any( CreateSamplerState("scene/terrain/diban_tu.dds") ) );
				subMaterial->SetParameter("uDetailScale", Any(Vector2(0.01f, 0.01f) ) );
				subMaterial->SetParameter("uDetailOffSet", Any(Vector4::ZERO) );
				subMaterial->SetParameter("u_cSpecColor", Any(Vector4::ZERO) );
			}

			for (uint32_t i = 0; i < testMaterial->GetSubNumber(); ++i)
			{
				SubMaterial* subMaterial = testMaterial->GetSubByIndex(i);
				subMaterial->SetParameter("tBlendingMap", Any(CreateSamplerState("scene/terrain/test_b0.dds", CLAMP, TFO_POINT, false)));
			}

			testMaterial->SaveToFile("scene/terrain/test.mtl");

			pTerrain->SetMaterialPath("scene/terrain/test.mtl");
			pTerrain->SetHeightMap("scene/terrain/test.dds");
			pTerrain->SetMaterialMap("scene/terrain/test_m.dds");
			pTerrain->SetBlendMap("scene/terrain/test_b0.dds");
			pTerrain->IsReady();

			pTerrain->SaveToXML("scene/terrain/test.xml");

			//m_pScene->GetRootNode()->AddChild(pTerrain.get());
		}
	
		{
			RefPtr<Terrain> pTerrain = CreateTerrain("scene/terrain/test.xml");

			m_pScene->GetRootNode()->AddChild(pTerrain.get());
		}

		m_pScene->GetMainDirLight()->GetSceneNode()->LookAt(Vector3(10,10,10),Vector3(0,0,0));
		m_pScene->GetMainDirLight()->SetLightColor(ColourValue(1.0,1.0,1.0,1.0f));
		m_pScene->SetAmbientColor(Vector3(0.0,0.0,0.0));
	}

	void SampleTerrain::UnLoad()
	{
		//m_pTerrain = NULL;		
	}
}


