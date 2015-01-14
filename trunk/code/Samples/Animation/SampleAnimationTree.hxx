#include "SampleAnimationTree.h"

namespace ma
{
	SampleAnimationTree::SampleAnimationTree()
	{
		m_pClip602 = NULL;
		m_pClip120 = NULL;
	}

	void SampleAnimationTree::UnLoad()
	{
		GetInput()->RemoveKeyListener(this);

		m_pClip602 = NULL;
		m_pClip120 = NULL;
	}

	void SampleAnimationTree::Load()
	{
		GetInput()->AddKeyListener(this);

		SceneNode* pGameObj =  m_pScene->CreateNode("magician");

		MeshComponent* pMeshComp = pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load("magician/Body.skn","magician/Body.mat");

		AnimationComponent* pAnimComp = pGameObj->CreateComponent<AnimationComponent>();
		pAnimComp->Load("magician/Body.Aniset","magician/Body.ske");
		
		AnimationSet* pAnimSet = pAnimComp->GetAnimationSet();
		SkelAnimtion* pSkelAnimtion = pAnimSet->CreateSkelAnim("TestAnim");
		RefPtr<AnimLayerNode> pLayerNode = CreateLayerNode();
		m_pClip602 = CreateClipNode("magician/602/bip01.ska","UpBody");
		m_pClip120 = CreateClipNode("magician/120/bip01.ska","LowerBody");
		pLayerNode->AddLayer(m_pClip120.get());
		pLayerNode->AddLayer(m_pClip602.get());
		pSkelAnimtion->SetTreeNode(pLayerNode.get());

		//pAnimSet->SetSkeleton(pAnimComp->GetSkeleton());
		pAnimComp->OnLoadOver();
			
		pAnimComp->PlayAnimation("TestAnim");
		
	}

	bool SampleAnimationTree::keyPressed(const OIS::KeyEvent &arg)
	{
		if (arg.key == OIS::KC_1)
		{
			m_pClip602->SetBoneSet("FullBody");
			m_pClip120->SetBoneSet("EmptyBody");
		}
		else if (arg.key == OIS::KC_2)
		{
			m_pClip602->SetBoneSet("EmptyBody");
			m_pClip120->SetBoneSet("FullBody");
		}
		else if (arg.key == OIS::KC_3)
		{
			m_pClip602->SetBoneSet("UpBody");
			m_pClip120->SetBoneSet("LowerBody");
		}

		return true;
	}
}


