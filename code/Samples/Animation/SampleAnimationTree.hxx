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
		m_pClip602 = NULL;
		m_pClip120 = NULL;
	}

	void SampleAnimationTree::Load()
	{
		m_pCamera->LookAt(Vector3(0, -600, 200), Vector3(0, 0, 0));

		RefPtr<SceneNode> pGameObj = m_pScene->CreateSceneNode();
	
		SkinMeshComponent* pMeshComp = pGameObj->CreateComponent<SkinMeshComponent>();
		pMeshComp->Load("magician/magician/Body.skn","magician/magician/Body.mtl");

		AnimationComponent* pAnimComp = pGameObj->CreateComponent<AnimationComponent>();
		pAnimComp->Load("magician/magician/Body.Aniset","magician/magician/Body.ske");

		m_pAnimator = pAnimComp;
// 		RefPtr<AnimLayerNode> pLayerNode = CreateLayerNode();
// 		m_pClip602 = CreateClipNode("magician/602/bip01.ska","Up602","UpBody");
// 		m_pClip120 = CreateClipNode("magician/120/bip01.ska","Low120","LowerBody");
// 		pLayerNode->AddLayer(m_pClip120.get());
// 		pLayerNode->AddLayer(m_pClip602.get());

		RefPtr<AnimClipNode> pClip602 = CreateClipNode("magician/602/bip01.ska","602");
		RefPtr<AnimClipNode> pClip120 = CreateClipNode("magician/120/bip01.ska","120");

		pAnimComp->GetAnimationSet()->AddSkelAnim(pClip602.get());
		pAnimComp->GetAnimationSet()->AddSkelAnim(pClip120.get());

	
		RefPtr<StateMachineNode> pStateMachineNode = CreateStateMachineNode();
		MachineState* pState602 = pStateMachineNode->AddState("602");
		MachineState* pState120 = pStateMachineNode->AddState("120");
	
		BaseParameter* pParm120 = m_pAnimator->GetParameterNode()->AddParameter("To120", 1.0f);

		MachineTransition* pTrans120 = pState602->AddTransition();
		pTrans120->SetEndState("120");
		pTrans120->SetBlendTime(0.3f);
		pTrans120->AddCondition(Condition::Greater, "To120", 1.0f);

		MachineTransition* pTrans602 = pState120->AddTransition();
		pTrans120->SetEndState("602");
		pTrans120->SetBlendTime(0.0f);
		pTrans602->AddCondition(Condition::Less, "To120", 1.0f);

		pStateMachineNode->SetStartStateId("120");

		//pAnimComp->SetAnimationTree(pStateMachineNode.get(),true);

		pStateMachineNode->SaveToXML("test.xml");

		RefPtr<StateMachineNode> pStateMachineNodeLoad = CreateStateMachineNode();
		pStateMachineNodeLoad->LoadFromXML("test.xml");

		pAnimComp->SetAnimationTree(pStateMachineNodeLoad.get(),true);
	}

	void SampleAnimationTree::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;

		if (key == Keyboard::KEY_F1)
		{
			m_pAnimator->GetParameterNode()->SetParameterValue("To120", 2.0);
			
// 			m_pClip602->SetBoneSet(pszDefaultUpBody);
// 			m_pClip120->SetBoneSet(pszDefaultEmptyBody);
		}
		else if (key == Keyboard::KEY_F2)
		{
			m_pAnimator->GetParameterNode()->SetParameterValue("To120", 0.0);
// 			m_pClip602->SetBoneSet(pszDefaultEmptyBody);
// 			m_pClip120->SetBoneSet(pszDefaultFullBody);
		}
		else if (key == Keyboard::KEY_THREE)
		{
// 			m_pClip602->SetBoneSet(pszDefaultUpBody);
// 			m_pClip120->SetBoneSet(pszDefaultLowerBody);
		}

		return;
	}
}


