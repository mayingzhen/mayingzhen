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
		Game::GetInstance().mkeyEvent.remove(this,&SampleAnimationTree::keyEvent);

		m_pClip602 = NULL;
		m_pClip120 = NULL;
	}

	void SampleAnimationTree::Load()
	{
		m_pCamera->LookAt(Vector3(0, -600, 200), Vector3(0, 0, 0));

		Game::GetInstance().mkeyEvent.notify(this,&SampleAnimationTree::keyEvent);

		RefPtr<SceneNode> pGameObj = m_pScene->CreateSceneNode();
	
		SkinMeshComponent* pMeshComp = pGameObj->CreateComponent<SkinMeshComponent>();
		pMeshComp->Load("magician/magician/Body.skn","magician/magician/Body.mtl");

		AnimationComponent* pAnimComp = pGameObj->CreateComponent<AnimationComponent>();
		pAnimComp->Load("magician/magician/Body.Aniset","magician/magician/Body.ske");


		RefPtr<AnimLayerNode> pLayerNode = CreateLayerNode();
		m_pClip602 = CreateClipNode("magician/602/bip01.ska","Up602","UpBody");
		m_pClip120 = CreateClipNode("magician/120/bip01.ska","Low120","LowerBody");
		pLayerNode->AddLayer(m_pClip120.get());
		pLayerNode->AddLayer(m_pClip602.get());

		pAnimComp->PlayAnimation(pLayerNode.get());
	}

	void SampleAnimationTree::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;

		if (key == Keyboard::KEY_ONE)
		{
			m_pClip602->SetBoneSet(pszDefaultUpBody);
			m_pClip120->SetBoneSet(pszDefaultEmptyBody);
		}
		else if (key == Keyboard::KEY_TWO)
		{
			m_pClip602->SetBoneSet(pszDefaultEmptyBody);
			m_pClip120->SetBoneSet(pszDefaultFullBody);
		}
		else if (key == Keyboard::KEY_THREE)
		{
			m_pClip602->SetBoneSet(pszDefaultUpBody);
			m_pClip120->SetBoneSet(pszDefaultLowerBody);
		}

		return;
	}
}


