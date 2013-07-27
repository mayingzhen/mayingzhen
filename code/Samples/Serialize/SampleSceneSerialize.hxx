#include "Samples/Serialize/SampleSceneSerialize.h"
#include "BulletPhysics/Module.h"
#include "Framwork/Module.h"

namespace ma
{
	SampleSceneSerialize::SampleSceneSerialize()
	{
		m_pScene = NULL;
	}

	void SampleSceneSerialize::Init()
	{
		Sample::Init();

		m_fZoomSpeed *= 0.5; 

		Load();
	}


	void SampleSceneSerialize::Load()
	{	
		Scene* pScene = new Scene();

		pScene->GetPhysicsScene()->SetGravity(Vector3(0,-0.98f,0));

		{
			GameObject* pGameObj = CreateGameObjectWithCollision(pScene,"Fbx/Box.skn","Fbx/Box.tga");
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


		{
			BinaryOutputArchive arOut;
			bool bOpenOK = arOut.Open("../Tesx.scene");
			ASSERT(bOpenOK);
			pScene->Serialize(arOut);
			SAFE_DELETE(pScene);
			arOut.Close();
		}

		{
			BinaryInputArchive arIn;
			bool bOpenOK = arIn.Open("../Tesx.scene");
			ASSERT(bOpenOK);
			SAFE_DELETE(m_pScene);
			m_pScene = new Scene();
			m_pScene->Serialize(arIn);
			arIn.Close();
		}


		m_pScene->Start();
	}


	void SampleSceneSerialize::Update()
	{
		Sample::Update();

		if (GetInput()->IsKeyDown(OIS::KC_X))
		{
			{
				XMLOutputArchive arOut;
				bool bOpenOK = arOut.Open("../Tesx.scene.xml");
				ASSERT(bOpenOK);
				m_pScene->Serialize(arOut);
				SAFE_DELETE(m_pScene);
				arOut.Close();
			}

			{
				XMLInputArchive arIn;
				bool bOpenOK = arIn.Open("../Tesx.scene.xml");
				ASSERT(bOpenOK);
				SAFE_DELETE(m_pScene);
				m_pScene = new Scene();
				m_pScene->Serialize(arIn);
				arIn.Close();
				m_pScene->Start();
			}
		}

		m_pScene->Update(GetTimer()->GetFrameDeltaTime());
	}

	void SampleSceneSerialize::Render()
	{
		m_pScene->Render(m_pCamera);
	}

}


