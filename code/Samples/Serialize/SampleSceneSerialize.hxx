#include "Samples/Serialize/SampleSceneSerialize.h"

namespace ma
{
	SampleSceneSerialize::SampleSceneSerialize()
	{
	}

	void SampleSceneSerialize::Load()
	{
		float fZoomSpeed = GetCameraControll()->GetZoomSpeed();
		GetCameraControll()->SetZoomSpeed(fZoomSpeed * 0.5);

		GetPhysicsSystem()->SetGravity(Vector3(0,-0.98f,0));
		{
			GameObject* pGameObj = CreateCollisionGameObject("Fbx/Box.skn","Fbx/Box.tga");
			pGameObj->SetName("Test");

			//RigidBodyComponent* pRigidBodyComp = pGameObj->CreateComponent<RigidBodyComponent>();

			int nClone = 5;
			for (int i = 0; i < nClone; ++i)
			{
				std::string pName = pGameObj->GetName();
				pName += std::string("_clone") + StringConverter::ToString(i);
				GameObject* pClone = pGameObj->Clone(pName.c_str());
				pClone->GetSceneNode()->Translate(Vector3(10 * i,0,0));
			}
		}

		std::string strResPath = FileSystem::getResourcePath();
		std::string strScenePath = strResPath + "scene/Test.scene";
		{
			BinaryOutputArchive arOut;
			bool bOpenOK = arOut.Open(strScenePath.c_str());
			ASSERT(bOpenOK);
			GetEntitySystem()->Serialize(arOut);
			arOut.Close();
		}

		{
			BinaryInputArchive arIn;
			bool bOpenOK = arIn.Open(strScenePath.c_str());
			ASSERT(bOpenOK);
			GetEntitySystem()->Serialize(arIn);
			arIn.Close();
		}
	}

	void SampleSceneSerialize::UnLoad()
	{
	}

	void SampleSceneSerialize::Update()
	{
		std::string strResPath = FileSystem::getResourcePath();
		std::string strScenePath = strResPath + "scene/Test.scene.xml";

		if (GetInput()->IsKeyDown(OIS::KC_X))
		{
			{
				XMLOutputArchive arOut;
				bool bOpenOK = arOut.Open(strScenePath.c_str());
				ASSERT(bOpenOK);
				GetEntitySystem()->Serialize(arOut);
				arOut.Close();
			}

			{
				XMLInputArchive arIn;
				bool bOpenOK = arIn.Open(strScenePath.c_str());
				ASSERT(bOpenOK);
				GetEntitySystem()->Serialize(arIn);
				arIn.Close();
			}
		}
	}

}


