#include "Samples/Serialize/SampleFbxImport.h"
#include "Animation/Module.h"
#include "D3D9Render/Module.h"
#include "Plugin/FbxImport/Module.h"

namespace ma
{
	SampleFbxImport::SampleFbxImport()
	{
		m_pAnimtionPlay = NULL;
		m_pSkeleton = NULL;
		m_pRenderMesh = NULL;
		m_pRendTexture = NULL;

		m_pStaticMesh = NULL;
		m_pStatcMeshTexture = NULL;
	}

	void SampleFbxImport::Init(Application* pApplication)
	{
		SimpleSceneView::Init(pApplication);
		
		D3D9RenderModuleInit();
		AnimationModuleInit();

		//D3D9RenderDevice* pDxRenderDevice = (D3D9RenderDevice*)GetRenderDevice();
		//pDxRenderDevice->Init( (HWND)pApplication->GetWindID() );
		GetRenderDevice()->Init(pApplication->GetWindID());
	}

	void SampleFbxImport::Shutdown()
	{
		D3D9RenderModuleShutdown();
		AnimationModuleShutdown();
	}

	void SampleFbxImport::LoadSkelMesh(FBXImporter& fbxImpor)
	{
		{
			MeshData* pMeshData = new MeshData;
			fbxImpor.LoadStaticMeshData("../Data/Fbx/Box.fbx",pMeshData);
			pMeshData->Save("../Data/Fbx/Box.skn");
		}
		

		MeshData* pMeshData = new MeshData;
		SkeletonData* pSkeData = new SkeletonData;
		AnimationData* pAnimData = new AnimationData;

		fbxImpor.LoadSkeletonMeshData("../Data/Fbx/TestBull_anim.fbx",pMeshData,pSkeData);

		fbxImpor.LoadAnimationData("../Data/Fbx/TestBull_anim.fbx",pAnimData,pSkeData);

		// Save
		SaveMeshToBinaryFile("../Data/Fbx/TestBull.skn",pMeshData);
		SaveSkeletonToBinaryFile("../Data/Fbx/TestBull.ske",pSkeData);
		SaveAnimationToBinaryFile("../Data/Fbx/TestBull.ska",pAnimData);
		SAFE_DELETE(pMeshData);
		SAFE_DELETE(pSkeData);
		SAFE_DELETE(pAnimData);
		///

		pMeshData = LoadMeshFromBinaryFile("../Data/Fbx/TestBull.skn");
		pSkeData = LoadSkeletonFromBinaryFile("../Data/Fbx/TestBull.ske");
		//pAnimData = LoadAnimationFromBinaryFile("../Data/Fbx/TestBull.ska");

		m_pRenderMesh = new D3D9RendMesh();
		m_pRenderMesh->InitWithData(pMeshData);

		const char* pTexPath = "../Data/Fbx/TestBull_DM.png";
		m_pRendTexture = new D3D9RendTexture();
		m_pRendTexture->SetResPath(pTexPath);
		m_pRendTexture->Load();

		m_pSkeleton = new Skeleton();
		m_pSkeleton->InitWithData(pSkeData);

		m_pAnimtionPlay = new AnimationPlay(m_pSkeleton);
		m_pAnimtionPlay->AddAction("../Data/Fbx/TestBull.ska","TestAction");
		m_pAnimtionPlay->PlayAnimation("TestAction");
	}

	void SampleFbxImport::LoadSaticMesh(FBXImporter& fbxImpor)
	{
		MeshData* pMeshData = new MeshData;

		fbxImpor.LoadStaticMeshData("../Data/Fbx/MovingPlatform.fbx",pMeshData);

		SaveMeshToBinaryFile("../Data/Fbx/MovingPlatform.skn",pMeshData);
		SAFE_DELETE(pMeshData);

		pMeshData = LoadMeshFromBinaryFile("../Data/Fbx/MovingPlatform.skn");

		m_pStaticMesh = new D3D9RendMesh();
		m_pStaticMesh->InitWithData(pMeshData);
		
		const char* pTexPath = "../Data/Fbx/PlatformTexture.tga";
		m_pStatcMeshTexture = new D3D9RendTexture();
		m_pStatcMeshTexture->SetResPath(pTexPath);
		m_pStatcMeshTexture->Load();
		
	}

	void SampleFbxImport::Load()
	{
		FBXImporter fbxImpor;
		fbxImpor.Initialize();
		
		LoadSaticMesh(fbxImpor);

		LoadSkelMesh(fbxImpor);
	}

	void SampleFbxImport::Unload()
	{

	}

	void SampleFbxImport::Tick(float timeElapsed)
	{
		__super::Tick(timeElapsed);

		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		if (m_pAnimtionPlay)
		{
			m_pAnimtionPlay->AdvanceTime(fTimeElapsed);

			m_pAnimtionPlay->EvaluateAnimation(1.0f);
		}
	}

	void SampleFbxImport::Render()
	{
		if (GetRenderDevice() == NULL)
			return;

		GetRenderDevice()->BeginRender();

		// render skelMesh
		{
			Matrix4x4 matWorld;
			MatrixTranslation(&matWorld,-50,0,0);

			if (m_pAnimtionPlay)
			{
				Matrix4x4* skinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
				UINT nNumber = m_pAnimtionPlay->GetSkinMatrixNumber();
				//pRender->RenderSkelMesh(skinMatrix,nNumber,&matWorld,m_pRenderMesh,m_pRendTexture);
			}
		}

		// render staticMesh
		{
			Matrix4x4 matWorld;
			Matrix4x4 matScale;
			MatrixScaling(&matScale,50,50,50); 
			MatrixTranslation(&matWorld,250,0,0);
			matWorld = matScale * matWorld;
			//pRender->RenderMesh(&matWorld,m_pStaticMesh,m_pStatcMeshTexture);
		}
	

		GetRenderDevice()->EndRender();
	}

}


