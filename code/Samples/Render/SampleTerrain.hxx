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

			MaterialData testMaterial;
			{
				SubMaterialData& subMatData = testMaterial.AddSubMatData();
				subMatData.SetShaderName("terrain");
				subMatData.SetShderMacro("LAYER 2");
				//subMatData.GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;

				subMatData.AddParameter("tDetailMap0", Any( CreateTexture("scene/terrain/chess.dds") ) );
				subMatData.AddParameter("tDetailMap1", Any( CreateTexture("scene/terrain/diban_zhuanshi.dds") )  );
				subMatData.AddParameter("tBlendingMap",Any( CreateTexture("scene/terrain/test_b0.dds") ) );
				subMatData.AddParameter("uDetailScale",Any(Vector2(0.01f, 0.01f)));
				subMatData.AddParameter("uDetailOffSet",Any(Vector4::ZERO));
			}

			{
				SubMaterialData& subMatData = testMaterial.AddSubMatData();
				subMatData.SetShaderName("terrain");
				subMatData.SetShderMacro("LAYER 1");
				//subMatData.GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;

				subMatData.AddParameter("tDetailMap0", Any( CreateTexture("scene/terrain/diban_tu.dds") ) );
				subMatData.AddParameter("uDetailScale",Any(Vector2(0.01f, 0.01f)));
				subMatData.AddParameter("uDetailOffSet",Any(Vector4::ZERO));
			}
			testMaterial.SaveToXML("scene/terrain/test.mat");

			pTerrain->SetMaterialPath("scene/terrain/test.mat");
			pTerrain->SetHeightMap("scene/terrain/test.dds");
			pTerrain->SetMaterialMap("scene/terrain/test_m.dds");
			pTerrain->SetBlendMap("scene/terrain/test_b0.dds");
			pTerrain->OnLoadOver();

			pTerrain->SaveToXML("scene/terrain/test.xml");

			m_pScene->AddChild(pTerrain.get());
		}
		else
		{
			RefPtr<Terrain> pTerrain = CreateTerrain("scene/terrain/test.xml");

			m_pScene->AddChild(pTerrain.get());
		}
	}

	void SampleTerrain::UnLoad()
	{
		//m_pTerrain = NULL;		
	}


}


