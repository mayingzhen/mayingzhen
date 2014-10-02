#include "Samples/Serialize/SampleSceneSerialize.h"

namespace ma
{
	SampleSceneSerialize::SampleSceneSerialize()
	{
	}

	void SampleSceneSerialize::Load()
	{
		Vector3 vEyePos = Vector3(0, 150, 200);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);

		GetInput()->AddKeyListener(this);

		{
			SceneNodePtr pGameObj = m_pScene->CreateNode("Test");  
			MeshComponentPtr pMehBox = pGameObj->CreateComponent<MeshComponent>();
			pMehBox->Load("Fbx/Box.skn","Fbx/box.mat");

			int nClone = 5;
			for (int i = 0; i < nClone; ++i)
			{
				std::string pName = pGameObj->GetName();
				pName += std::string("_clone") + StringConverter::toString(i);
				SceneNodePtr pClone = pGameObj->Clone(pName.c_str());
				pClone->Translate(Vector3(10 * i,0,0));
			}
		}

		std::string strScenePath = "scene/Test.scene";
		{
			BinaryOutputSerializer arOut;
			bool bOpenOK = arOut.Open(strScenePath.c_str());
			ASSERT(bOpenOK);
			m_pScene->Serialize(arOut);
			arOut.Close();
		}

		{
			BinaryInputSerializer arIn;
			bool bOpenOK = arIn.Open(strScenePath.c_str());
			ASSERT(bOpenOK);
			m_pScene->Serialize(arIn);
			arIn.Close();
		}
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
				XMLOutputSerializer arOut;
				bool bOpenOK = arOut.Open(strScenePath.c_str());
				ASSERT(bOpenOK);
				m_pScene->Serialize(arOut);
				arOut.Close();
			}

			{
				XMLInputSerializer arIn;
				bool bOpenOK = arIn.Open(strScenePath.c_str());
				ASSERT(bOpenOK);
				m_pScene->Serialize(arIn);
				arIn.Close();
			}
		}

		return true;
	}
}


