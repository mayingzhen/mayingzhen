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
		//GetInput()->RemoveKeyListener(this);

		m_pClip602 = NULL;
		m_pClip120 = NULL;
	}

	void SampleAnimationTree::Load()
	{
		m_pCamera->LookAt(Vector3(0, 200, 600), Vector3(0, 0, 0));

		//GetInput()->AddKeyListener(this);

// 		RefPtr<SceneNode> pGameObj = CreateSceneNode();
// 		m_pScene->GetRootNode()->AddChild(pGameObj.get());
// 
// 		SkinMeshComponent* pMeshComp = pGameObj->CreateComponent<SkinMeshComponent>();
// 		pMeshComp->Load("magician/Body.skn","magician/Body.mtl");
// 
// 		AnimationComponent* pAnimComp = pGameObj->CreateComponent<AnimationComponent>();
// 		pAnimComp->Load("magician/Body.Aniset","magician/Body.ske");


		SceneNode* pCharMagic = m_pScene->CreateSceneNode("magician/magician.xml");

		AnimationComponent* pAnimComp = pCharMagic->GetTypeComponent<AnimationComponent>();
		
		RefPtr<AnimLayerNode> pLayerNode = CreateLayerNode();
		m_pClip602 = CreateClipNode("magician/602/bip01.ska","Up602","UpBody");
		m_pClip120 = CreateClipNode("magician/120/bip01.ska","Low120","LowerBody");
		pLayerNode->AddLayer(m_pClip120.get());
		pLayerNode->AddLayer(m_pClip602.get());

		pAnimComp->PlayAnimation(pLayerNode.get());
	}

// 	bool SampleAnimationTree::keyPressed(const OIS::KeyEvent &arg)
// 	{
// 		if (arg.key == OIS::KC_1)
// 		{
// 			m_pClip602->SetBoneSet("FullBody");
// 			m_pClip120->SetBoneSet("EmptyBody");
// 		}
// 		else if (arg.key == OIS::KC_2)
// 		{
// 			m_pClip602->SetBoneSet("EmptyBody");
// 			m_pClip120->SetBoneSet("FullBody");
// 		}
// 		else if (arg.key == OIS::KC_3)
// 		{
// 			m_pClip602->SetBoneSet("UpBody");
// 			m_pClip120->SetBoneSet("LowerBody");
// 		}
// 
// 		return true;
// 	}
}


