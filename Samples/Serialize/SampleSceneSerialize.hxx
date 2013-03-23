#include "Samples/Serialize/SampleSceneSerialize.h"
#include "BulletPhysics/Module.h"

namespace ma
{
	SampleSceneSerialize::SampleSceneSerialize()
	{
		m_pScene = NULL;
	}

	void SampleSceneSerialize::Init(Application* pApplication)
	{
		BtPhysicsModuleInit();

		SimpleSceneView::Init(pApplication);

		//D3DXVECTOR3 vEyePos = D3DXVECTOR3(0, 6, 10);
		m_fMoveCameraSpeed = 0.20f;
		//m_pCamera->LookAt(&vEyePos);
	}

	void SampleSceneSerialize::Shutdown()
	{
		BtPhysicsModuleShutdown();

		SimpleSceneView::Shutdown();
	}

	void SampleSceneSerialize::Load()
	{
		//ma::SceneNode* pRootNode = m_pScene->GetRootNode();//new SceneNode(NULL,"RootNode");
		ma::Scene* pScene = new Scene();
		ma::SceneNode* pRootNode = pScene->GetRootNode();

		pScene->GetPhysicsScene()->SetGravity(D3DXVECTOR3(0,-0.98f,0));

		{
			GameObject* pGameObj = new GameObject(m_pScene,"physics");
			pRootNode->AddChildNode(pGameObj);

			MeshComponent* pMeshComp = new MeshComponent();
			pMeshComp->Load("../Data/Fbx/Box.skn","../Data/Fbx/Box.tga");
			pGameObj->AddComponent(pMeshComp);

			D3DXVECTOR3 vMin,vMax;
			pMeshComp->GetBoundingAABB(vMin,vMax);
			D3DXVECTOR3 vSize = vMax - vMin;
			D3DXVECTOR3 vCenter = (vMin + vMax) * 0.5f;
			maNodeTransform tsf;
			maTransformSetIdentity(&tsf);
			tsf.m_vPos = vCenter;

			BoxCollisionComponent* pBoxCollisionShape = new BoxCollisionComponent();
			pBoxCollisionShape->SetSize(vSize);
			pBoxCollisionShape->SetTransformLS(tsf);
			pGameObj->AddComponent(pBoxCollisionShape);

			RigidBodyComponent* pRigidBodyComp = new RigidBodyComponent();
			pGameObj->AddComponent(pRigidBodyComp);
			pRigidBodyComp->SetUseGravity(false);
			pRigidBodyComp->SetKinematic(true);
		}

		{
			GameObject* pGameObj = new GameObject(m_pScene,"Terrain");
			pRootNode->AddChildNode(pGameObj);

			MeshComponent* pMeshComp = new MeshComponent();
			pMeshComp->Load("../Data/Fbx/MovingPlatform.skn","../Data/Fbx/PlatformTexture.tga");
			pGameObj->AddComponent(pMeshComp);

			D3DXVECTOR3 vMin,vMax;
			pMeshComp->GetBoundingAABB(vMin,vMax);
			D3DXVECTOR3 vSize = vMax - vMin;
			D3DXVECTOR3 vCenter = (vMin + vMax) * 0.5f;
			maNodeTransform tsf;
			maTransformSetIdentity(&tsf);
			tsf.m_vPos = vCenter;

			BoxCollisionComponent* pBoxCollisionShape = new BoxCollisionComponent();
			pBoxCollisionShape->SetSize(vSize);
			pBoxCollisionShape->SetTransformLS(tsf);
			pGameObj->AddComponent(pBoxCollisionShape);

			pGameObj->TranslateWS(D3DXVECTOR3(0,-3,0));
		}

		{
			BinaryOutputArchive arOut;
			bool bOpenOK = arOut.Open("../Tesx.scene");
			assert(bOpenOK);
			pScene->Serialize(arOut);
			SAFE_DELETE(pScene);
		}

		{
			BinaryInputArchive arIn;
			bool bOpenOK = arIn.Open("../Tesx.scene");
			assert(bOpenOK);
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
				bool bOpenOK = arOut.Open("../Tesx.scene.xml");
				assert(bOpenOK);
				m_pScene->Serialize(arOut);
				SAFE_DELETE(m_pScene);
			}

			{
				XMLInputArchive arIn;
				bool bOpenOK = arIn.Open("../Tesx.scene.xml");
				assert(bOpenOK);
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

	void SampleSceneSerialize::OnResize(int w,int h)
	{

	}
}


