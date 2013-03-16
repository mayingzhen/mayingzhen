#include "Samples/Physics/SampleRigidBody.h"
#include "BulletPhysics/Module.h"

namespace ma
{
	SampleRigidBody::SampleRigidBody()
	{
	}

	void SampleRigidBody::Init(Application* pApplication)
	{
		BtPhysicsModuleInit();

		SimpleSceneView::Init(pApplication);

		m_fMoveCameraSpeed = 0.20f;
		m_pCamera->SetPositionWS( D3DXVECTOR3(0, 6, 10) );	
	}

	void SampleRigidBody::Shutdown()
	{
		BtPhysicsModuleShutdown();

		SimpleSceneView::Shutdown();
	}

	void SampleRigidBody::Load()
	{
		assert(m_pScene);
		if (m_pScene == NULL)
			return;

		ma::SceneNode* pRootNode = m_pScene->GetRootNode(); 

		m_pScene->GetPhysicsScene()->SetGravity(D3DXVECTOR3(0,-0.98f,0));

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

			m_pRigidBodyComp = new RigidBodyComponent();
			pGameObj->AddComponent(m_pRigidBodyComp);
			m_pRigidBodyComp->SetUseGravity(false);
			m_pRigidBodyComp->SetKinematic(true);
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

		m_pScene->Start();
	}

	void SampleRigidBody::Unload()
	{

	}

	void SampleRigidBody::Tick(float timeElapsed)
	{
		__super::Tick(timeElapsed);

		if (GetInput()->IsKeyReleased(OIS::KC_G))
		{
			m_pRigidBodyComp->SetUseGravity(true);
		}
		if (GetInput()->IsKeyPressed(OIS::KC_K))
		{
			m_pRigidBodyComp->SetKinematic(false);
		}
	}

	void SampleRigidBody::Render()
	{
		__super::Render();
	}

	void SampleRigidBody::OnResize(int w,int h)
	{

	}
}


