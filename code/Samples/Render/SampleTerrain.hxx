#include "SampleTerrain.h"


namespace ma
{
	SampleTerrain::SampleTerrain()
	{
	}


	void SampleTerrain::Load()
	{
 		Vector3 lookAtPos(0,0,0);
 		Vector3 eyePos = Vector3(0,-60,60);
 		GetCamera()->LookAt(eyePos,lookAtPos);

		//GetResourceSystem()->SetDataThreadEnable(true);

		if (1)
		{
			RefPtr<Terrain> pTerrain = CreateTerrain();

			pTerrain->SetTrunkSize(32);
			pTerrain->SetHeightSpcing(50);
			pTerrain->SetStartPoint(Vector3(0, 0, 0));

			RefPtr<Material> testMaterial = CreateMaterial();
			{
				RefPtr<SubMaterial> subMatData = CreateSubMaterial();
				testMaterial->AddSubMaterial(0,subMatData.get());

				subMatData->SetShadingTechnqiue("terrain","LAYER 2;LIGHTING");
				
				subMatData->SetParameter("tDetailMap0", Any( CreateSamplerState("scene/terrain/chess.dds") ) );
				subMatData->SetParameter("tDetailMap1", Any( CreateSamplerState("scene/terrain/diban_zhuanshi.dds") ) );
				subMatData->SetParameter("tBlendingMap", Any( CreateSamplerState("scene/terrain/test_b0.dds",CLAMP,TFO_POINT,false) ) );
				subMatData->SetParameter("uDetailScale", Any( Vector2(0.1f, 0.1f) ) );
				subMatData->SetParameter("uDetailOffSet", Any( Vector4::ZERO) );
				subMatData->SetParameter("u_cSpecColor", Any( Vector4::ZERO) );
			}

			{
				RefPtr<SubMaterial> subMatData = CreateSubMaterial();
				testMaterial->AddSubMaterial(0,subMatData.get());

				subMatData->SetShadingTechnqiue("terrain","LAYER 1;LIGHTING;BUMPMAP;PARALLAXMAPPING");

				subMatData->SetParameter("tDetailMap0", Any( CreateSamplerState("scene/terrain/wall.jpg") ) );
				subMatData->SetParameter("tBumpMap0", Any( CreateSamplerState("scene/terrain/wall_NM_height.tga",REPEAT,TFO_TRILINEAR,false) ) );
				subMatData->SetParameter("uDetailScale", Any(Vector2(0.1f, 0.1f) ) );
				subMatData->SetParameter("uDetailOffSet", Any(Vector4::ZERO) );
				subMatData->SetParameter("u_cSpecColor", Any(Vector4::ZERO) );
			}

			{
				RefPtr<SubMaterial> subMatData = CreateSubMaterial();
				testMaterial->AddSubMaterial(0,subMatData.get());

				subMatData->SetShadingTechnqiue("terrain","LAYER 1");

				subMatData->SetParameter("tDetailMap0", Any( CreateSamplerState("scene/terrain/diban_tu.dds") ) );
				subMatData->SetParameter("uDetailScale", Any(Vector2(0.01f, 0.01f) ) );
				subMatData->SetParameter("uDetailOffSet", Any(Vector4::ZERO) );
				subMatData->SetParameter("u_cSpecColor", Any(Vector4::ZERO) );
			}

			testMaterial->SaveToFile("scene/terrain/test.mtl");

			pTerrain->SetMaterialPath("scene/terrain/test.mtl");
			pTerrain->SetHeightMap("scene/terrain/test.dds");
			pTerrain->SetMaterialMap("scene/terrain/test_m.dds");
			pTerrain->SetBlendMap("scene/terrain/test_b0.dds");
			pTerrain->IsReady();

			pTerrain->SaveToXML("scene/terrain/test.xml");

			m_pScene->GetRootNode()->AddChild(pTerrain.get());
		}
		else
		{
			RefPtr<Terrain> pTerrain = CreateTerrain("scene/terrain/test.xml");

			m_pScene->GetRootNode()->AddChild(pTerrain.get());
		}

		m_pScene->GetDirLight()->GetSceneNode()->LookAt(Vector3(10,10,10),Vector3(0,0,0));
		m_pScene->GetDirLight()->SetLightColor(ColourValue(1.0,1.0,1.0,1.0f));
		m_pScene->SetAmbientColor(Vector3(0.0,0.0,0.0));
	}

	void SampleTerrain::UnLoad()
	{
		//m_pTerrain = NULL;		
	}


}


