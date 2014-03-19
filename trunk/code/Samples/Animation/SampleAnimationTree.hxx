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

		GameObjectPtr pGameObj =  GetEntitySystem()->CreateGameObject("magician");

		MeshComponentPtr pMeshComp = pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load("magician/Body.skn","magician/Body.mat");

		IAnimationObjectPtr pAnimComp = pGameObj->CreateComponent<IAnimationObject>();
		pAnimComp->Load("magician/Body.Aniset","magician/Body.ske");

// 		ActionData actionData;
// 		AnimLayerNodeData animLayerData;
// 		AnimClipNodeData clip602Data;
// 		clip602Data.m_sClipPath = "magician/602/bip01.ska";
// 		clip602Data.m_sBoneSetName = "UpBody";
// 		AnimClipNodeData clip120Data;
// 		clip120Data.m_sClipPath = "magician/120/bip01.ska";
// 		clip120Data.m_sBoneSetName = "LowerBody";
// 		animLayerData.m_arrAnimNodeData.push_back(&clip120Data);
// 		animLayerData.m_arrAnimNodeData.push_back(&clip602Data);
// 		actionData.m_pAnimNodeData = &animLayerData;

		
		IAnimationSet* pAnimSet = pAnimComp->GetAnimationSet();
		IAction*	pAction = pAnimSet->CreateAction("TestAnim");
		IAnimLayerNode*	pLayerNode = pAction->CreateLayerNode();
		m_pClip602 = pAction->CreateClipNode("magician/602/bip01.ska","UpBody");
		m_pClip120 = pAction->CreateClipNode("magician/120/bip01.ska","LowerBody");
		pLayerNode->AddLayer(m_pClip120);
		pLayerNode->AddLayer(m_pClip602);
		pAction->SetTreeNode(pLayerNode);
			
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


