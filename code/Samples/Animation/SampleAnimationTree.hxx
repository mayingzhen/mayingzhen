#include "SampleAnimationTree.h"

namespace ma
{
	SampleAnimationTree::SampleAnimationTree()
	{
		m_pAnimtionPlay = NULL;

		m_pRenderMesh = NULL;
	}

	void SampleAnimationTree::UnLoad()
	{
	}

	void SampleAnimationTree::Load()
	{
		m_pRenderMesh = new RenderMesh();
		m_pRenderMesh->Load("magician/Body.skn","magician/Body.tga");

		m_pAnimtionPlay = GetAnimationSystem()->CreateAnimationPlay("magician/Body.ske");
		AnimationSet* pAnimSet = m_pAnimtionPlay->GetAnimationSet();
		Action*	pAction = pAnimSet->CreateAction("TestAnim");
		//AnimLayerNode*	pLayerNode = pAction->CreateTreeNode<AnimLayerNode>();
		//m_pClip602 = pLayerNode->AddLayer<AnimClipNode>();
		//m_pClip602->SetAnimationClip("magician/602/bip01.ska");
		//m_pClip602->SetBoneSet("UpBody");

		//m_pClip100 = pLayerNode->AddLayer<AnimClipNode>();
		//m_pClip100->SetAnimationClip("magician/100/bip01.ska");
		//m_pClip100->SetBoneSet("LowerBody");

		m_pAnimtionPlay->PlayAnimation("TestAnim");
	}


	void SampleAnimationTree::Update()
	{
// 		float fTimeElapsed = GetTimer()->GetFrameDeltaTime();
// 
// 		m_pAnimtionPlay->AdvanceTime(fTimeElapsed);
// 
// 		m_pAnimtionPlay->EvaluateAnimation(1.0f);
// 
// 		Matrix4x4* skinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
// 		UINT nNumber = m_pAnimtionPlay->GetSkinMatrixNumber();
// 		m_pRenderMesh->SetSkinMatrix(skinMatrix,nNumber);
// 
// 		if (GetInput()->IsKeyDown(OIS::KC_1))
// 		{
// 			m_pClip602->SetBoneSet("FullBody");
// 			m_pClip100->SetBoneSet("EmptyBody");
// 		}
	}

	void SampleAnimationTree::Render()
	{
		m_pRenderMesh->Draw();
	}
}


