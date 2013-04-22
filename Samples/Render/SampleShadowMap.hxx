#include "SampleShadowMap.h"
#include "Plugin/FbxImport/Module.h"

namespace ma
{
	SampleShadowMap::SampleShadowMap()
	{
		m_pLigt = NULL;
	}

	void SampleShadowMap::Init(Application* pApplication)
	{
		SimpleSceneView::Init(pApplication);

		m_fMoveCameraSpeed = 1.0f;
		
		//m_pCamera->LookAt( &Vector3(-7, 2, -3) );	
	}

	void SampleShadowMap::Shutdown()
	{
		SimpleSceneView::Shutdown();
	}

	void SampleShadowMap::Load()
	{
		assert(m_pScene);
		if (m_pScene == NULL)
			return;

		FBXImporter fbxImpor;
		fbxImpor.Initialize();

		MeshData* pMeshDataPlat = new MeshData;
		fbxImpor.LoadStaticMeshData("../Data/Fbx/MovingPlatform.fbx",pMeshDataPlat);
		SaveMeshToBinaryFile("../Data/Fbx/MovingPlatform.skn",pMeshDataPlat);
		SAFE_DELETE(pMeshDataPlat);

		MeshData* pMeshDataBox = new MeshData;
		fbxImpor.LoadStaticMeshData("../Data/Fbx/Box.fbx",pMeshDataBox);
		SaveMeshToBinaryFile("../Data/Fbx/Box.skn",pMeshDataBox);
		SAFE_DELETE(pMeshDataBox);

		SceneNode* pRootNode = m_pScene->GetRootNode(); 

		{
			GameObject* pGameObj = new GameObject(m_pScene,"Box");
			pRootNode->AddChildNode(pGameObj);

			MeshComponent* pMeshComp = new MeshComponent();
			pMeshComp->Load("../Data/Fbx/Box.skn","../Data/Fbx/Box.tga");
			pGameObj->AddComponent(pMeshComp);
		}

		{
			GameObject* pGameObj = new GameObject(m_pScene,"Platform");
			pRootNode->AddChildNode(pGameObj);

			MeshComponent* pMeshComp = new MeshComponent();
			pMeshComp->Load("../Data/Fbx/MovingPlatform.skn","../Data/Fbx/PlatformTexture.tga");
			pGameObj->AddComponent(pMeshComp);
			pGameObj->TranslateWS(Vector3(0,-1,0));
		}

		//light
		{
			m_pLigt = new Light(m_pScene,"Light");
			m_pLigt->SetCreateShadow(true);
			m_pLigt->SetLigtType(LIGHT_DIRECTIONAL);
			pRootNode->AddChildNode(m_pLigt);
			//m_pLigt->LookAt(&Vector3(2, 1, 0));
			m_pLigt->LookAt(&Vector3(-1.6, 10, -3));
			
			GetRenderDevice()->AddLight(m_pLigt);
		}


		m_pScene->Start();
	}

	void SampleShadowMap::Unload()
	{
		SAFE_DELETE(m_pLigt);	
	}

	void SampleShadowMap::Tick(float timeElapsed)
	{
		SimpleSceneView::Tick(timeElapsed);

		Input* pInput = GetInput();
		if (pInput == NULL)
			return;

		float fMoveSpeed = 5;
		float fMoveDis = fMoveSpeed * timeElapsed;

		Vector3 vLightPos = m_pLigt->GetPositionWS();
		if ( pInput->IsKeyDown(OIS::KC_W) )
		{
			vLightPos.y += fMoveDis;
		}
		if ( pInput->IsKeyDown(OIS::KC_S) )
		{
			vLightPos.y -= fMoveDis;
		}
		if ( pInput->IsKeyDown(OIS::KC_A) )
		{
			vLightPos.x -= fMoveDis;
		}
		if ( pInput->IsKeyDown(OIS::KC_D) )
		{
			vLightPos.x += fMoveDis;
		}
		m_pLigt->LookAt(&vLightPos);

	}

	void SampleShadowMap::Render()
	{
		__super::Render();

		Matrix4x4 matWS = m_pLigt->GetWorldMatrix();
		GetRenderDevice()->DrawWireSphere(matWS,0.2,COLOR_RGBA(255,0,0,255));
	}

}


