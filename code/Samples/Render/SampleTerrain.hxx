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

				subMatData->SetShaderName("terrain");
				subMatData->SetShderMacro("LAYER 2");
				
				subMatData->AddParameter("tDetailMap0", Any( CreateSamplerState("scene/terrain/chess.dds") ) );
				subMatData->AddParameter("tDetailMap1", Any( CreateSamplerState("scene/terrain/diban_zhuanshi.dds") ) );
				subMatData->AddParameter("tBlendingMap", Any( CreateSamplerState("scene/terrain/test_b0.dds") ) );
				subMatData->AddParameter("uDetailScale", Any( Vector2(0.01f, 0.01f) ) );
				subMatData->AddParameter("uDetailOffSet", Any( Vector4::ZERO) );
				subMatData->AddParameter("u_cSpecColor", Any( Vector4::ZERO) );
			}

			{
				RefPtr<SubMaterial> subMatData = CreateSubMaterial();
				testMaterial->AddSubMaterial(subMatData.get());

				subMatData->SetShaderName("terrain");
				subMatData->SetShderMacro("LAYER 1");

				subMatData->AddParameter("tDetailMap0", Any( CreateSamplerState("scene/terrain/diban_tu.dds") ) );
				subMatData->AddParameter("uDetailScale", Any(Vector2(0.01f, 0.01f) ) );
				subMatData->AddParameter("uDetailOffSet", Any(Vector4::ZERO) );
				subMatData->AddParameter("u_cSpecColor", Any(Vector4::ZERO) );
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


