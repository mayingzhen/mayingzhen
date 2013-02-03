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

		GameObject* pGameObj = new GameObject(m_pScene,"physics");
		pRootNode->AddChildNode(pGameObj);

		MeshComponent* pMeshComp = new MeshComponent();
		pMeshComp->Load("../Fbx/TestBull.skn","../Fbx/TestBull_DM.png");
		pGameObj->AddComponent(pMeshComp);

		BoxCollisionComponent* pBoxCollisionShape = new BoxCollisionComponent();
 		pBoxCollisionShape->SetSize(D3DXVECTOR3(100,100,100));
 		pGameObj->AddComponent(pBoxCollisionShape);

		RigidBodyComponent* pRigidBodyComp = new RigidBodyComponent();
 		pGameObj->AddComponent(pRigidBodyComp);

	
		// Stone
// 		{
// 			GameObject* pGameObj = new GameObject(m_pScene,"physics");
// 			pRootNode->AddChildNode(pGameObj);
// 			pGameObj->RotateLS(3.14f,0,0);
// 
// 			MeshComponent* pMeshComp = new MeshComponent();
// 			pMeshComp->Load("../TrineGame/map/stone.skn","../TrineGame/map/stone.tga");
// 			pGameObj->AddComponent(pMeshComp);
// 
// 			BoxCollisionComponent* pBoxCollisionShape = new BoxCollisionComponent();
// 			pBoxCollisionShape->SetSize(D3DXVECTOR3(100,100,100));
// 			pGameObj->AddComponent(pBoxCollisionShape);
// 
// 			RigidBodyComponent* pRigidBodyComp = new RigidBodyComponent();
// 			pGameObj->AddComponent(pRigidBodyComp);
// 		}
// 
// 		// Terrain
// 		{
// 			GameObject* pTerrain = new GameObject(m_pScene,"Terrain");
// 			pRootNode->AddChildNode(pTerrain);
// 			pTerrain->RotateLS(3.14f,0,0);
// 
// 			MeshComponent* pTerrainMesh = new MeshComponent();
// 			pTerrainMesh->Load("../TrineGame/map/terrain.skn","../TrineGame/map/terrain.tga");
// 			pTerrain->AddComponent(pTerrainMesh);
// 
// 			 BoxCollisionComponent* pTerrainBoxColl = new BoxCollisionComponent();
// 			 pTerrainBoxColl->SetSize(D3DXVECTOR3(1000,1000,1000));
// 			 pTerrain->AddComponent(pTerrainBoxColl);
// 		}

		m_pScene->Start();
	}


	void SamplePhysics::Unload()
	{

	}

	void SamplePhysics::Tick(float timeElapsed)
	{
		__super::Tick(timeElapsed);

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


