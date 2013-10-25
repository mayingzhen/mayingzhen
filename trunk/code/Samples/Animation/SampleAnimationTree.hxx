#include "SampleAnimationTree.h"

namespace ma
{
	SampleAnimationTree::SampleAnimationTree()
	{
		m_pGameObj = NULL;
		m_pClip602 = NULL;
		m_pClip100 = NULL;
	}

	void SampleAnimationTree::UnLoad()
	{
		GetInput()->RemoveKeyListener(this);

		GetEntitySystem()->DeleteGameObject(m_pGameObj);
	}

	void SampleAnimationTree::Load()
	{
		GetInput()->AddKeyListener(this);

		m_pGameObj = CreateAnimationGameObject("magician/Body.skn","magician/Body.tga","magician/Body.ske");

		IAnimationObject* pAnimComp = m_pGameObj->GetTypeComponentFirst<IAnimationObject>();
		IAnimationSet* pAnimSet = pAnimComp->GetAnimationSet();
		IAction*	pAction = pAnimSet->CreateAction("TestAnim");
 		IAnimLayerNode*	pLayerNode = pAction->CreateLayerNode();
		m_pClip602 = pAction->CreateClipNode("magician/602/bip01.ska","UpBody");
		m_pClip100 = pAction->CreateClipNode("gigi/210_run/bip01.ska","LowerBody");
		pLayerNode->AddLayer(m_pClip100);
		pLayerNode->AddLayer(m_pClip602);
		pAction->SetTreeNode(pLayerNode);

		pAnimComp->PlayAnimation("TestAnim");

		m_pGameObj->GetSceneNode()->Translate(Vector3(50,-100,0));
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
// 		else if (arg.key == OIS::KC_4)
// 		{
// 			m_pClip602->SetBoneSet("LowerBody");
// 			m_pClip100->SetBoneSet("UpBody");
// 		}

		return true;
	}

	bool SampleAnimationTree::keyReleased(const OIS::KeyEvent &arg)
	{
		return true;
	}
}


