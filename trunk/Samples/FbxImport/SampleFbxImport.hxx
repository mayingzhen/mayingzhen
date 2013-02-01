#include "Samples/FbxImport/SampleFbxImport.h"
#include "Animation/Module.h"
#include "DXRender/Module.h"
#include "Serialize/Module.h"

namespace ma
{
	SampleFbxImport::SampleFbxImport()
	{
		m_pAnimtionPlay = NULL;

		m_pSkeleton = NULL;

		m_pRenderMesh = NULL;

		m_pRendTexture = NULL;
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

	void SampleFbxImport::Load()
	{
		FBXImporter fbxImpor;
		fbxImpor.Initialize();
		MeshData* pMeshData = new MeshData;
		SkeletonData* pSkeData = new SkeletonData;
		std::vector<AnimationData*> vAnimData;
		fbxImpor.LoadScene("../Fbx/TestBull_anim.fbx",pMeshData,pSkeData,vAnimData);

		m_pRenderMesh = new DxRendMesh();
		m_pRenderMesh->InitWithData(pMeshData);

		const char* pTexPath = "../Fbx/TestBull_DM.png";
		m_pRendTexture = new DxRendTexture();
		m_pRendTexture->Load(pTexPath);

		m_pSkeleton = new Skeleton();
		m_pSkeleton->InitWithData(*pSkeData);

		Animation* pAnimation = new Animation();
		pAnimation->InitWithData(vAnimData[0]);
		pAnimation->ConverteAnimDataParentToLocalSpaceAnimation(m_pSkeleton);
		AnimationInst* pAnimInst = new AnimationInst(pAnimation,m_pSkeleton);
		AnimClipNode* pClipNode = new AnimClipNode(pAnimInst/*,m_pSkeleton->GetBoneSetByName("UpBody")*/);
		AnimationAction* pAction = new AnimationAction();
		pAction->SetTreeNode(pClipNode);

		m_pAnimtionPlay = new AnimationPlay(); 
		m_pAnimtionPlay->SetSkeleton(m_pSkeleton);
		m_pAnimtionPlay->PlayAnimation(pAction);
	}

	void SampleFbxImport::Unload()
	{

	}

	void SampleFbxImport::Tick(float timeElapsed)
	{
		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		m_pAnimtionPlay->AdvanceTime(fTimeElapsed);

		m_pAnimtionPlay->EvaluateAnimation(1.0f);
	}

	void SampleFbxImport::Render()
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		pRender->BeginRender();

		pRender->SetViewMatrix(&m_matView);
		pRender->SetProjMatrix(&m_matProj);

		if (m_pAnimtionPlay == NULL)
			return;

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		D3DXMATRIX* skinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
		UINT nSkinMatrixNumber = m_pAnimtionPlay->GetSkinMatrixNumber();
		pRender->RenderSkelMesh(skinMatrix,nSkinMatrixNumber,&matWorld,m_pRenderMesh,m_pRendTexture);

		pRender->EndRender();
	}

	void SampleFbxImport::OnResize(int w,int h)
	{

	}
}


