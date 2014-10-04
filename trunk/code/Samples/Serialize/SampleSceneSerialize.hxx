#include "Samples/Serialize/SampleSceneSerialize.h"

namespace ma
{
	SampleSceneSerialize::SampleSceneSerialize()
	{
	}

	void SampleSceneSerialize::Load()
	{
		Vector3 vEyePos = Vector3(0, 15, 20);
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
				pClone->Translate(Vector3(1 * i,0,0));
			}
		}

		std::string strScenePath = "scene/Test.scene.xml";
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

		SceneNode* pCamaerNode = m_pScene->FindNode("defaultCamera");
		ASSERT(pCamaerNode);
		if (pCamaerNode == NULL)
			 return;

		Camera* pCamera = pCamaerNode->GetTypeComponent<Camera>();
		ASSERT(pCamera);
		GetCameraControll()->SetCamera(pCamera);
		
		m_pCamera = pCamera;
		GetRenderSystem()->GetView(0)->SetCamera(pCamera);
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


