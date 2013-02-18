#include "Samples/Serialize/SampleSceneSerialize.h"
#include "DXRender/Module.h"
#include "Framework/Module.h"
#include "BulletPhysics/Module.h"

namespace ma
{
	SampleSceneSerialize::SampleSceneSerialize()
	{
		m_pScene = NULL;
	}

	void SampleSceneSerialize::Init(Application* pApplication)
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

	void SampleSceneSerialize::Shutdown()
	{
		FrameWorkModuleShutdown();
		DxRenderModuleShutdown();
		BtPhysicsModuleShutdown();
	}

	void SampleSceneSerialize::Load()
	{
		ma::SceneNode* pRootNode = new SceneNode(NULL,"RootNode");
		ma::Scene* pScene = new Scene(pRootNode);

		pScene->GetPhysicsScene()->SetGravity(D3DXVECTOR3(0,-0.98f,0));

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

			RigidBodyComponent* pRigidBodyComp = new RigidBodyComponent();
			pGameObj->AddComponent(pRigidBodyComp);
			pRigidBodyComp->SetUseGravity(false);
			pRigidBodyComp->SetKinematic(true);
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

		{
			BinaryOutputArchive arOut;
			bool bOpenOK = arOut.Open("../Tesx.scene");
			assert(bOpenOK);
			pScene->Serialize(arOut);
		}

		{
			BinaryInputArchive arIn;
			bool bOpenOK = arIn.Open("../Tesx.scene");
			assert(bOpenOK);
			SAFE_DELETE(m_pScene);
			m_pScene = new Scene(NULL);
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
			}

			{
				XMLInputArchive arIn;
				bool bOpenOK = arIn.Open("../Tesx.scene.xml");
				assert(bOpenOK);
				SAFE_DELETE(m_pScene);
				m_pScene = new Scene(NULL);
				m_pScene->Serialize(arIn);
				m_pScene->Start();
			}
		}

		if (m_pScene)
		{
			m_pScene->Update(timeElapsed);
		}
	}

	void SampleSceneSerialize::Render()
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

	void SampleSceneSerialize::OnResize(int w,int h)
	{

	}
}


