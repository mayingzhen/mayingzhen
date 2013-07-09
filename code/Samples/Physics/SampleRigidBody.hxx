#include "Samples/Physics/SampleRigidBody.h"
#include "Framwork/Module.h"
#include "BulletPhysics/Module.h"

namespace ma
{
	SampleRigidBody::SampleRigidBody()
	{
	}

	void SampleRigidBody::Init(const Platform* pPlatform)
	{
		Sample::Init(pPlatform);

		FramworkModuleInit();
		BtPhysicsModuleInit();

		//m_fMoveCameraSpeed = 0.20f;
		Vector3 vEyePos = Vector3(0, 40, 40);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		m_pCamera->LookAt(vEyePos, VAtPos, vUp);

		Load();
	}

	void SampleRigidBody::Shutdown()
	{
		BtPhysicsModuleShutdown();
		FramworkModuleShutdown();

		//Sample::Shutdown();
	}

	void SampleRigidBody::Load()
	{
		m_pScene = new Scene();

		//ma::SceneNode* pRootNode = m_pScene->GetRootNode(); 

		m_pScene->GetPhysicsScene()->SetGravity(Vector3(0,-0.98f,0));

		{
			GameObject* pGameObj = m_pScene->CreateGameObject("physics");

			MeshComponent* pMeshComp = pGameObj->CreateComponent<MeshComponent>();
			pMeshComp->Load("/Fbx/Box.skn","/Fbx/Box.tga");

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

			m_pRigidBodyComp = pGameObj->CreateComponent<RigidBodyComponent>();
			IRigidBody* pRigidBody = m_pRigidBodyComp->GetRigidBody();
			pRigidBody->SetUseGravity(false);
			pRigidBody->SetKinematic(true);
		}

		{
			GameObject* pGameObj = m_pScene->CreateGameObject("Terrain");
	
			MeshComponent* pMeshComp = pGameObj->CreateComponent<MeshComponent>();//new MeshComponent();
			pMeshComp->Load("/Fbx/MovingPlatform.skn","/Fbx/PlatformTexture.tga");

			Vector3 vMin,vMax;
			pMeshComp->GetBoundingAABB(vMin,vMax);
			Vector3 vSize = vMax - vMin;
			Vector3 vCenter = (vMin + vMax) * 0.5f;
			NodeTransform tsf;
			TransformSetIdentity(&tsf);
			tsf.m_vPos = vCenter;

			BoxCollisionComponent* pBoxCollisionShape = pGameObj->CreateComponent<BoxCollisionComponent>();//new BoxCollisionComponent();
			pBoxCollisionShape->SetSize(vSize);
			//pBoxCollisionShape->SetTransformLS(tsf);

			pGameObj->GetSceneNode()->Translate(vCenter,TS_WORLD);
		}

		{
			XMLOutputArchive xmlOutAr;
			xmlOutAr.Open("../../Text.scene.xml");
			m_pScene->Serialize(xmlOutAr);
			xmlOutAr.Close();
		}

		{
			XMLInputArchive xmlInAr;
			xmlInAr.Open("../../Text.scene.xml");

			SAFE_DELETE(m_pScene);
			m_pScene = new Scene();
			m_pScene->Serialize(xmlInAr);
		}


		m_pScene->Start();
	}

	void SampleRigidBody::Unload()
	{

	}

	void SampleRigidBody::Update()
	{
		Sample::Update();

		m_pScene->Update(GetTimer()->GetFrameDeltaTime());

// 		if (GetInput()->IsKeyReleased(OIS::KC_G))
// 		{
// 			m_pRigidBodyComp->SetUseGravity(true);
// 		}
// 		if (GetInput()->IsKeyPressed(OIS::KC_K))
// 		{
// 			m_pRigidBodyComp->SetKinematic(false);
// 		}
	}

	void SampleRigidBody::Render()
	{
		Sample::Render();

		m_pScene->Render(m_pCamera);
	}

}


