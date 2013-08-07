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
		GetInput()->RemoveKeyListener(this);
	}

	void SampleAnimationTree::Load()
	{
		GetInput()->AddKeyListener(this);

		m_pRenderMesh = new RenderMesh();
		m_pRenderMesh->Load("magician/Body.skn","magician/Body.tga");

		m_pAnimtionPlay = GetAnimationSystem()->CreateAnimationPlay("magician/Body.ske");
		AnimationSet* pAnimSet = m_pAnimtionPlay->GetAnimationSet();
		Action*	pAction = pAnimSet->CreateAction("TestAnim");
 		AnimLayerNode*	pLayerNode = pAction->CreateRootNode<AnimLayerNode>();
		m_pClip602 = pLayerNode->CreateLayer<AnimClipNode>();
		m_pClip100 = pLayerNode->CreateLayer<AnimClipNode>();
		
		m_pClip602->SetAnimationClip("magician/602/bip01.ska");
		m_pClip602->SetBoneSet("UpBody");

		m_pClip100->SetAnimationClip("gigi/210_run/bip01.ska"/*"magician/100/bip01.ska"*/);
		m_pClip100->SetBoneSet("LowerBody");

		m_pAnimtionPlay->PlayAnimation("TestAnim");


	
	}


	void SampleAnimationTree::Update()
	{
		float fTimeElapsed = GetTimer()->GetFrameDeltaTime();

		m_pAnimtionPlay->AdvanceTime(fTimeElapsed);

		m_pAnimtionPlay->EvaluateAnimation(1.0f);

		Matrix4x4* skinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
		UINT nNumber = m_pAnimtionPlay->GetSkinMatrixNumber();
		m_pRenderMesh->SetSkinMatrix(skinMatrix,nNumber);

		Matrix4x4 matWorld,matRoat;
		MatrixTranslation(&matWorld,50,120,0);
		MatrixRotationYawPitchRoll(&matRoat,0,PI * 1.2,0);
		matWorld = matWorld * matRoat;
		m_pRenderMesh->SetWorldMatrix(matWorld);
	}

	void SampleAnimationTree::Render()
	{
		m_pRenderMesh->Draw();
	}

	bool SampleAnimationTree::keyPressed(const OIS::KeyEvent &arg)
	{
		if (arg.key == OIS::KC_1)
		{
			m_pClip602->SetBoneSet("FullBody");
			m_pClip100->SetBoneSet("EmptyBody");
		}
		else if (arg.key == OIS::KC_2)
		{
			m_pClip602->SetBoneSet("EmptyBody");
			m_pClip100->SetBoneSet("FullBody");
		}
		else if (arg.key == OIS::KC_3)
		{
			m_pClip602->SetBoneSet("UpBody");
			m_pClip100->SetBoneSet("LowerBody");
		}
		else if (arg.key == OIS::KC_4)
		{
			m_pClip602->SetBoneSet("LowerBody");
			m_pClip100->SetBoneSet("UpBody");
		}

		return true;
	}

	bool SampleAnimationTree::keyReleased(const OIS::KeyEvent &arg)
	{
		return true;
	}
}


