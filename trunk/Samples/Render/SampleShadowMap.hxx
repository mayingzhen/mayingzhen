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

		m_fMoveCameraSpeed = 0.20f;
		m_pCamera->SetPositionWS( D3DXVECTOR3(0, 2, -3) );	
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
			//pGameObj->TranslateWS(D3DXVECTOR3(0,0,0));
		}

		//light
		{
			m_pLigt = new Light(m_pScene,"Light");
			m_pLigt->SetCeateShadow(true);
			m_pLigt->SetLigtType(TYPE_ORTHOGRAPHIC);
			pRootNode->AddChildNode(m_pLigt);
			m_pLigt->SetPositionWS(D3DXVECTOR3(2, 1, 0));
			
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
		__super::Tick(timeElapsed);

		Input* pInput = GetInput();
		if (pInput == NULL)
			return;

		float fMoveSpeed = 5;
		float fMoveDis = fMoveSpeed * timeElapsed;

		D3DXVECTOR3 vLightPos = m_pLigt->GetPositionWS();
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
		m_pLigt->SetPositionWS(vLightPos);

	}

	void SampleShadowMap::Render()
	{
		__super::Render();

		D3DXMATRIX matWS = m_pLigt->GetWorldMatrix();
		GetRenderDevice()->DrawWireSphere(matWS,0.2,D3DCOLOR_RGBA(255,0,0,255));
	}

	void SampleShadowMap::OnResize(int w,int h)
	{

	}
}


