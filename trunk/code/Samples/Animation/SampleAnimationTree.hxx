#include "SampleAnimationTree.h"
#include "Animation/Module.h"
#include "DXRender/Module.h"

namespace ma
{
	SampleAnimationTree::SampleAnimationTree()
	{
		m_pAnimtionPlay = NULL;

		m_pSkeleton = NULL;

		m_pRenderMesh = NULL;

		m_pRendTexture = NULL;
	}

	void SampleAnimationTree::Init(Application* pApplication)
	{
		SimpleSceneView::Init(pApplication);

		DxRenderModuleInit();
		AnimationModuleInit();

		DxRenderDevice* pDxRenderDevice = (DxRenderDevice*)GetRenderDevice();
		pDxRenderDevice->Init( (HWND)pApplication->GetWindID() );

		DxRender* pDxRender = (DxRender*)GetRender();
		pDxRender->InitDefaultShader();

	}

	void SampleAnimationTree::Shutdown()
	{
		DxRenderModuleShutdown();
		AnimationModuleShutdown();
	}

	void SampleAnimationTree::Load()
	{
// 		FBXImporter fbxImpor;
// 		fbxImpor.Initialize();
// 		MeshData* pMeshData = new MeshData;
// 		SkeletonData* pSkeData = new SkeletonData;
// 		std::vector<AnimationData*> vAnimData;
// 		fbxImpor.LoadScene("../Fbx/TestBull_anim.fbx",pMeshData,pSkeData,vAnimData);

		m_pRenderMesh = new DxRendMesh();
		m_pRenderMesh->InitWithData(pMeshData);

		const char* pTexPath = "../Fbx/TestBull_DM.png";
		m_pRendTexture = new DxRendTexture();
		m_pRendTexture->Load(pTexPath);

		m_pSkeleton = new Skeleton();
		m_pSkeleton->InitWithData(*pSkeData);

		Animation* pAnimation = new Animation;
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

	void SampleAnimationTree::Unload()
	{

	}

	void SampleAnimationTree::Tick(float timeElapsed)
	{
		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		m_pAnimtionPlay->AdvanceTime(fTimeElapsed);

		m_pAnimtionPlay->EvaluateAnimation(1.0f);
	}

	void SampleAnimationTree::Render()
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		pRender->BeginRender();

		pRender->SetViewMatrix(&m_matView);
		pRender->SetProjMatrix(&m_matProj);

		if (!m_pSkeleton || !m_pRenderMesh || !m_pAnimtionPlay)
			return;

		D3DXMATRIX arrSkinMatrix[256];
		UINT nBoneNum = m_pSkeleton->GetBoneNumer();
		NodePose* pAnimPose = m_pAnimtionPlay->GetAnimationPose();
		for (UINT i = 0; i < nBoneNum; ++i)
		{
			if (pAnimPose)
			{
				maMatrixFromTransform(&arrSkinMatrix[i],& pAnimPose->GetTransformOS(i));
				D3DXMatrixMultiply(&arrSkinMatrix[i],& m_pSkeleton->GetBoneMatrixOSInv(i),&arrSkinMatrix[i]);
			}
			else
			{
				D3DXMatrixIdentity(&arrSkinMatrix[i]);
			}
		}

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		pRender->RenderSkelMesh(arrSkinMatrix,nBoneNum,&matWorld,m_pRenderMesh,m_pRendTexture);

		pRender->EndRender();
	}

	void SampleAnimationTree::OnResize(int w,int h)
	{

	}
}


