#include "SampleTerrain.h"


namespace ma
{
	SampleTerrain::SampleTerrain()
	{
	}


	void SampleTerrain::Load()
	{
 		Vector3 lookAtPos(0,0,0);
 		Vector3 eyePos = Vector3(0,600,600);
 		GetCamera()->GetSceneNode()->LookAt(eyePos,lookAtPos);

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
				testMaterial->AddSubMaterial(subMatData.get());

				subMatData->SetShadingTechnqiue("terrain","LAYER 2");
				
				subMatData->SetParameter("tDetailMap0", Any( CreateTexture("scene/terrain/chess.dds") ) );
				subMatData->SetParameter("tDetailMap1", Any( CreateTexture("scene/terrain/diban_zhuanshi.dds") ) );
				subMatData->SetParameter("tBlendingMap", Any( CreateTexture("scene/terrain/test_b0.dds") ) );
				subMatData->SetParameter("uDetailScale", Any( Vector2(0.01f, 0.01f) ) );
				subMatData->SetParameter("uDetailOffSet", Any( Vector4::ZERO) );
				subMatData->SetParameter("u_cSpecColor", Any( Vector4::ZERO) );
			}

			{
				RefPtr<SubMaterial> subMatData = CreateSubMaterial();
				testMaterial->AddSubMaterial(subMatData.get());

				subMatData->SetShadingTechnqiue("terrain","LAYER 1");

				subMatData->SetParameter("tDetailMap0", Any( CreateTexture("scene/terrain/diban_tu.dds") ) );
				subMatData->SetParameter("uDetailScale", Any(Vector2(0.01f, 0.01f) ) );
				subMatData->SetParameter("uDetailOffSet", Any(Vector4::ZERO) );
				subMatData->SetParameter("u_cSpecColor", Any(Vector4::ZERO) );
			}
			testMaterial->SaveToXML("scene/terrain/test.mat");

			pTerrain->SetMaterialPath("scene/terrain/test.mat");
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
	}

	void SampleTerrain::UnLoad()
	{
		//m_pTerrain = NULL;		
	}


}


