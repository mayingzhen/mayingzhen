#include "Samples/Serialize/SampleFbxImport.h"
#include "Animation/Module.h"
#include "DXRender/Module.h"
#include "Serialize/Module.h"
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
		
		DxRenderModuleInit();
		AnimationModuleInit();

		DxRenderDevice* pDxRenderDevice = (DxRenderDevice*)GetRenderDevice();
		pDxRenderDevice->Init( (HWND)pApplication->GetWindID() );

		DxRender* pDxRender = (DxRender*)GetRender();
		pDxRender->InitDefaultShader();

	}

	void SampleFbxImport::Shutdown()
	{
		DxRenderModuleShutdown();
		AnimationModuleShutdown();
	}

	void SampleFbxImport::LoadSkelMesh(FBXImporter& fbxImpor)
	{
		MeshData* pMeshData = new MeshData;
		SkeletonData* pSkeData = new SkeletonData;
		AnimationData* pAnimData = new AnimationData;

		fbxImpor.LoadSkeletonMeshData("../Fbx/TestBull_anim.fbx",pMeshData,pSkeData);

		fbxImpor.LoadAnimationData("../Fbx/TestBull_anim.fbx",pAnimData,pSkeData);

		// Save
		SaveMeshToBinaryFile("../Fbx/TestBull.skn",pMeshData);
		SaveSkeletonToBinaryFile("../Fbx/TestBull.ske",pSkeData);
		SaveAnimationToBinaryFile("../Fbx/TestBull.ska",pAnimData);
		SAFE_DELETE(pMeshData);
		SAFE_DELETE(pSkeData);
		SAFE_DELETE(pAnimData);
		///

		pMeshData = LoadMeshFromBinaryFile("../Fbx/TestBull.skn");
		pSkeData = LoadSkeletonFromBinaryFile("../Fbx/TestBull.ske");
		pAnimData = LoadAnimationFromBinaryFile("../Fbx/TestBull.ska");

		m_pRenderMesh = new DxRendMesh();
		m_pRenderMesh->InitWithData(pMeshData);

		const char* pTexPath = "../Fbx/TestBull_DM.png";
		m_pRendTexture = new DxRendTexture();
		m_pRendTexture->Load(pTexPath);

		m_pSkeleton = new Skeleton();
		m_pSkeleton->InitWithData(*pSkeData);

		Animation* pAnimation = new Animation();
		pAnimation->InitWithData(pAnimData);
		pAnimation->ConverteAnimDataParentToLocalSpaceAnimation(m_pSkeleton);
		AnimationInst* pAnimInst = new AnimationInst(pAnimation,m_pSkeleton);
		AnimClipNode* pClipNode = new AnimClipNode(pAnimInst/*,m_pSkeleton->GetBoneSetByName("UpBody")*/);
		AnimationAction* pAction = new AnimationAction();
		pAction->SetTreeNode(pClipNode);

		m_pAnimtionPlay = new AnimationPlay(); 
		m_pAnimtionPlay->SetSkeleton(m_pSkeleton);
		m_pAnimtionPlay->PlayAnimation(pAction);
	}

	void SampleFbxImport::LoadSaticMesh(FBXImporter& fbxImpor)
	{
		MeshData* pMeshData = new MeshData;

		fbxImpor.LoadStaticMeshData("../Fbx/MovingPlatform.fbx",pMeshData);

		SaveMeshToBinaryFile("../Fbx/MovingPlatform.skn",pMeshData);
		SAFE_DELETE(pMeshData);

		pMeshData = LoadMeshFromBinaryFile("../Fbx/MovingPlatform.skn");

		m_pStaticMesh = new DxRendMesh();
		m_pStaticMesh->InitWithData(pMeshData);
		
		const char* pTexPath = "../Fbx/PlatformTexture.tga";
		m_pStatcMeshTexture = new DxRendTexture();
		m_pStatcMeshTexture->Load(pTexPath);
		
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
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		pRender->BeginRender();

		pRender->SetViewMatrix(&m_matView);
		pRender->SetProjMatrix(&m_matProj);

		// render skelMesh
		{
			D3DXMATRIX matWorld;
			D3DXMatrixTranslation(&matWorld,-50,0,0);

			if (m_pAnimtionPlay)
			{
				D3DXMATRIX* skinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
				UINT nNumber = m_pAnimtionPlay->GetSkinMatrixNumber();
				pRender->RenderSkelMesh(skinMatrix,nNumber,&matWorld,m_pRenderMesh,m_pRendTexture);
			}
		}

		// render staticMesh
		{
			D3DXMATRIX matWorld;
			D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale,50,50,50); 
			D3DXMatrixTranslation(&matWorld,250,0,0);
			matWorld = matScale * matWorld;
			pRender->RenderMesh(&matWorld,m_pStaticMesh,m_pStatcMeshTexture);
		}
	

		pRender->EndRender();
	}

	void SampleFbxImport::OnResize(int w,int h)
	{

	}
}


