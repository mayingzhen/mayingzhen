#include "SampleAnimationTree.h"

namespace ma
{
	SampleAnimationTree::SampleAnimationTree()
	{
		m_pAnimtionPlay = NULL;

		m_pRenderMesh = NULL;
	}

	void SampleAnimationTree::Init()
	{
		Sample::Init();

		Load();
	}

	void SampleAnimationTree::Shutdown()
	{
	}

	void SampleAnimationTree::Load()
	{
		m_pRenderMesh = new RenderMesh();
		m_pRenderMesh->Load("magician/Body.skn","magician/Body.tga");

		m_pAnimtionPlay =  GetAnimationDevice()->CreateAnimationPlay();
		m_pAnimtionPlay->CreateSkeleton("magician/Body.ske");


// 		Animation* pAnimation = new Animation;
// 		pAnimation->InitWithData(vAnimData[0]);
// 		pAnimation->ConverteAnimDataParentToLocalSpaceAnimation(m_pSkeleton);
// 		AnimationInst* pAnimInst = new AnimationInst(pAnimation,m_pSkeleton);
// 		AnimClipNode* pClipNode = new AnimClipNode(pAnimInst/*,m_pSkeleton->GetBoneSetByName("UpBody")*/);
// 		AnimationAction* pAction = new AnimationAction();
// 		pAction->SetTreeNode(pClipNode);
// 
// 		m_pAnimtionPlay = new AnimationPlay(); 
// 		m_pAnimtionPlay->SetSkeleton(m_pSkeleton);
// 		m_pAnimtionPlay->PlayAnimation(pAction);
	}

	void SampleAnimationTree::Unload()
	{

	}

	void SampleAnimationTree::Update()
	{
		Sample::Update();

		float fTimeElapsed = GetTimer()->GetFrameDeltaTime();

		m_pAnimtionPlay->AdvanceTime(fTimeElapsed);

		m_pAnimtionPlay->EvaluateAnimation(1.0f);

		Matrix4x4* skinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
		UINT nNumber = m_pAnimtionPlay->GetSkinMatrixNumber();
		m_pRenderMesh->SetSkinMatrix(skinMatrix,nNumber);
	}

	void SampleAnimationTree::Render()
	{
		m_pRenderMesh->Draw();
	}
}


