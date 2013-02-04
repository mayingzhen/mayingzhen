#include "Samples/Physics/SamplePhysics.h"
#include "DXRender/Module.h"
#include "Framework/Module.h"
#include "BulletPhysics/Module.h"

namespace ma
{
	SamplePhysics::SamplePhysics()
	{
	}

	void SamplePhysics::Init(Application* pApplication)
	{
		m_vEyePos = D3DXVECTOR3(0, 6, 10);
		m_fMoveCameraSpeed = 0.20f;
		SimpleSceneView::Init(pApplication);

		DxRenderModuleInit();
		FrameWorkModuleInit();
		BtPhysicsModuleInit();

		DxRenderDevice* pDxRenderDevice = (DxRenderDevice*)GetRenderDevice();
		pDxRenderDevice->Init( (HWND)pApplication->GetWindID() );

		DxRender* pDxRender = (DxRender*)GetRender();
		pDxRender->InitDefaultShader();

		
	}

	void SamplePhysics::Shutdown()
	{
		FrameWorkModuleShutdown();
		DxRenderModuleShutdown();
		BtPhysicsModuleShutdown();
	}

	void SamplePhysics::Load()
	{
		ma::SceneNode* pRootNode = new SceneNode(NULL,"RootNode");
		m_pScene = new Scene(pRootNode);

		m_pScene->GetPhysicsScene()->SetGravity(D3DXVECTOR3(0,-0.98f,0));

		{
			GameObject* pGameObj = new GameObject(m_pScene,"physics");
			pRootNode->AddChildNode(pGameObj);

			MeshComponent* pMeshComp = new MeshComponent();
			pMeshComp->Load("../Fbx/Box.skn","../Fbx/Box.tga");
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
		}

		{
			GameObject* pGameObj = new GameObject(m_pScene,"Terrain");
			pRootNode->AddChildNode(pGameObj);

			MeshComponent* pMeshComp = new MeshComponent();
			pMeshComp->Load("../Fbx/MovingPlatform.skn","../Fbx/PlatformTexture.tga");
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


	void SamplePhysics::Unload()
	{

	}

	void SamplePhysics::Tick(float timeElapsed)
	{
		__super::Tick(timeElapsed);

		if (GetInput()->IsKeyReleased(OIS::KC_G))
		{
			m_pRigidBodyComp->SetUseGravity(true);
		}

		if (m_pScene)
		{
			m_pScene->Update(timeElapsed);
		}
	}

	void SamplePhysics::Render()
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		pRender->BeginRender();

		pRender->SetViewMatrix(&m_matView);
		pRender->SetProjMatrix(&m_matProj);

		if (m_pScene)
		{
			m_pScene->Render();
		}

		pRender->EndRender();
	}

	void SamplePhysics::OnResize(int w,int h)
	{

	}
}


