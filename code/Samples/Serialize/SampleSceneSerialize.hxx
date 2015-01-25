#include "Samples/Serialize/SampleSceneSerialize.h"

namespace ma
{
	SampleSceneSerialize::SampleSceneSerialize()
	{
	}

	void SampleSceneSerialize::Load()
	{
		GetCamera()->GetSceneNode()->LookAt(Vector3(0, 20, 15), Vector3(0, 0, 0));

		GetInput()->AddKeyListener(this);

		//GetResourceSystem()->SetDataThreadEnable(false);

		std::string strScenePath = "scene/Test.scene.xml";

		if (1)
		{
			MaterialData matData;

			SubMaterialData& subMatData = matData.AddSubMatData();
			subMatData.SetShaderName("default");
			subMatData.SetShderMacro("DIFFUSE;DIFFUSECOLOR");
			//subMatData.GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;

			RefPtr<SamplerState> pDiff = CreateSamplerState();
			pDiff->SetTexturePath("FBX/Box.tga");

			subMatData.AddParameter("u_texture", Any(pDiff) );
			subMatData.AddParameter("u_cDiffuseColor", Any( Vector4(1,0,0,0) ) );

			matData.SaveToXML("Fbx/Box.mat");

			RefPtr<Scene> pScene = CreateScene();

			SceneNodePtr pGameObj = pScene->CreateNode("Test");
			MeshComponentPtr pMehBox = pGameObj->CreateComponent<MeshComponent>();
			pMehBox->Load("Fbx/Box.skn","Fbx/box.mat");
			
			//pMehBox->GetSubMaterial(0)->GetShadingTechnqiue()->SetParameter("u_cDiffuseColor",Any( Vector4(0,1,0,0) ));
			
			int nClone = 5;
			for (int i = 0; i < nClone; ++i)
			{
				std::string pName = pGameObj->GetName();
				pName += std::string("_clone"); // + StringConverter::toString(i);
				SceneNodePtr pClone = pGameObj->Clone(pName.c_str());
				pClone->Translate(Vector3(2 * (i + 1),0,0));
			}

			pScene->SaveToXML(strScenePath.c_str());
		}
		
		m_pScene->LoadFromXML(strScenePath.c_str());

	}

	void SampleSceneSerialize::UnLoad()
	{
		GetInput()->RemoveKeyListener(this);
	}

	bool SampleSceneSerialize::keyPressed(const OIS::KeyEvent &arg)
	{
		std::string strScenePath = "scene/Test.scene";

		if (arg.key == OIS::KC_X)
		{
			{
// 				XMLOutputSerializer arOut;
// 				m_pScene->Serialize(arOut);
// 				arOut.Save(strScenePath.c_str());
				m_pScene->SaveToFile(strScenePath.c_str());
			}

			{
// 				XMLInputSerializer arIn;
// 				bool bOpenOK = arIn.Open(strScenePath.c_str());
// 				ASSERT(bOpenOK);
// 				m_pScene->Serialize(arIn);
// 				arIn.Close();
				m_pScene->LoadFromFile(strScenePath.c_str());
			}
		}

		return true;
	}
}


