#include "Samples/Serialize/SampleSceneSerialize.h"
#include "BulletPhysics/Module.h"

namespace ma
{
	SampleSceneSerialize::SampleSceneSerialize()
	{
		m_pScene = NULL;
	}

	void SampleSceneSerialize::Init()
	{
		Sample::Init();

		BtPhysicsModuleInit();
		AnimationModuleInit();

		//Vector3 vEyePos = Vector3(0, 6, 10);
		//m_fMoveCameraSpeed = 0.20f;
		//m_pCamera->LookAt(&vEyePos);
	}

	void SampleSceneSerialize::Shutdown()
	{
		BtPhysicsModuleShutdown();
		AnimationModuleShutdown();

		//Sample::Shutdown();
	}

	void SampleSceneSerialize::Load()
	{
		//ma::SceneNode* pRootNode = m_pScene->GetRootNode();//new SceneNode(NULL,"RootNode");	
		Scene* pScene = new Scene();

		pScene->GetPhysicsScene()->SetGravity(Vector3(0,-0.98f,0));

		{
			GameObject* pGameObj = pScene->CreateGameObject("physics");

			MeshComponent* pMeshComp = pGameObj->CreateComponent<MeshComponent>();
			pMeshComp->Load("Fbx/Box.skn","Fbx/Box.tga");
	
			Vector3 vMin,vMax;
			pMeshComp->GetBoundingAABB(vMin,vMax);
			Vector3 vSize = vMax - vMin;
			Vector3 vCenter = (vMin + vMax) * 0.5f;
			NodeTransform tsf;
			TransformSetIdentity(&tsf);
			tsf.m_vPos = vCenter;

			BoxCollisionComponent* pBoxCollisionShape = pGameObj->CreateComponent<BoxCollisionComponent>();
			pBoxCollisionShape->SetSize(vSize);
			pBoxCollisionShape->SetTransformLS(tsf);

			RigidBodyComponent* pRigidBodyComp = pGameObj->CreateComponent<RigidBodyComponent>();
			pRigidBodyComp->GetRigidBody()->SetUseGravity(false);
			pRigidBodyComp->GetRigidBody()->SetKinematic(true);
			
			int nClone = 5;
			for (int i = 0; i < nClone; ++i)
			{
				char buf[10] = {0};
				itoa(i,buf,10);
				std::string pName = pGameObj->GetName();
				pName += std::string("_clone") + buf;
				GameObject* pClone = pGameObj->Clone(pName.c_str());
				pClone->Translate(Vector3(10 * i,0,0));
			}
			
		}

		{
			GameObject* pGameObj = m_pScene->CreateGameObject("Terrain");

			MeshComponent* pMeshComp = pGameObj->CreateComponent<MeshComponent>();//new MeshComponent();
			pMeshComp->Load("/Fbx/MovingPlatform.skn","/Fbx/PlatformTexture.tga");
			pGameObj->AddComponent(pMeshComp);

			Vector3 vMin,vMax;
			pMeshComp->GetBoundingAABB(vMin,vMax);
			Vector3 vSize = vMax - vMin;
			Vector3 vCenter = (vMin + vMax) * 0.5f;
			NodeTransform tsf;
			TransformSetIdentity(&tsf);
			tsf.m_vPos = vCenter;

			BoxCollisionComponent* pBoxCollisionShape = pGameObj->CreateComponent<BoxCollisionComponent>();//new BoxCollisionComponent();
			pBoxCollisionShape->SetSize(vSize);
			pBoxCollisionShape->SetTransformLS(tsf);
			pGameObj->AddComponent(pBoxCollisionShape);

			pGameObj->Translate(vCenter,TS_WORLD);
		}

		{
			BinaryOutputArchive arOut;
			bool bOpenOK = arOut.Open("../Tesx.scene");
			ASSERT(bOpenOK);
			pScene->Serialize(arOut);
			SAFE_DELETE(pScene);
		}

		{
			BinaryInputArchive arIn;
			bool bOpenOK = arIn.Open("../Tesx.scene");
			ASSERT(bOpenOK);
			//SAFE_DELETE(m_pScene);
			//m_pScene = new Scene(NULL);
			m_pScene->Serialize(arIn);
		}


		m_pScene->Start();
	}

	void SampleSceneSerialize::Unload()
	{

	}

	void SampleSceneSerialize::Tick(float timeElapsed)
	{
		__super::Tick(timeElapsed);

		if (GetInput()->IsKeyReleased(OIS::KC_X))
		{
			{
				XMLOutputArchive arOut;
				bool bOpenOK = arOut.Open("../Tesx.scene.l");
				ASSERT(bOpenOK);
				m_pScene->Serialize(arOut);
				SAFE_DELETE(m_pScene);
			}

			{
				XMLInputArchive arIn;
				bool bOpenOK = arIn.Open("../Tesx.scene.l");
				ASSERT(bOpenOK);
				SAFE_DELETE(m_pScene);
				m_pScene = new Scene();
				m_pScene->Serialize(arIn);
				m_pScene->Start();
			}
		}
	}

	void SampleSceneSerialize::Render()
	{
		__super::Render();
	}

}


