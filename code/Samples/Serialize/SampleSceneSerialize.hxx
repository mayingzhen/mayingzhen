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

		if (0)
		{
			CreateDefaultMaterial("FBX/Box.tga","Fbx/Box.mat");

			RefPtr<SceneNode> pSceneNode = CreateSceneNode();

			RefPtr<SceneNode> pTestSceneNode = CreateSceneNode();
			RefPtr<MeshComponent> pMehBox = pTestSceneNode->CreateComponent<MeshComponent>();
			pMehBox->Load("Fbx/Box.skn","Fbx/box.mat");
		
// 			int nClone = 5;
// 			for (int i = 0; i < nClone; ++i)
// 			{
// 				std::string pName = pTestSceneNode->GetName();
// 				pName += std::string("_clone"); // + StringConverter::toString(i);
// 				RefPtr<SceneNode> pClone = pTestSceneNode->Clone(pName.c_str());
// 				pClone->Translate(Vector3(2 * (i + 1),0,0));
// 				pSceneNode->AddChild(pClone.get());
// 			}

			pTestSceneNode->SaveToXML(strScenePath.c_str());
		}
		
		RefPtr<SceneNode> pSceneNode = CreateSceneNode(strScenePath.c_str());
		m_pScene->GetRootNode()->AddChild(pSceneNode.get());

		//m_pScene->GetSunShaow()->SetEnabled(true);
		m_pScene->GetSunShaow()->GetSceneNode()->LookAt(Vector3(1.f, 1.f, 0.f),Vector3::ZERO); 
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
				//m_pScene->GetRootNode()->SaveToFile(strScenePath.c_str());
			}

			{
				//m_pScene->GetRootNode()->LoadFromFile(strScenePath.c_str());
			}
		}

		return true;
	}
}


