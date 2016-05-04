#include "SampleAnimationIK.h"


namespace ma
{
	SampleAnimationIK::SampleAnimationIK()
	{
	}


	void SampleAnimationIK::Load()
	{	
		m_pCamera->LookAt(Vector3(0, -6, 2), Vector3(0, 0, 0));

		CreateMeshMaterial("hero_17/hero_17/body.tga","hero_17/hero_17/Body.mtl","SKIN");

		RefPtr<AnimationSet> pAnimSet = CreateAnimationSet();

		RefPtr<SceneNode> pChar = m_pScene->CreateSceneNode();
		pChar->SetScale( Vector3(0.01f) );
		SkinMeshComponent* pMeshComp = pChar->CreateComponent<SkinMeshComponent>();
		pMeshComp->Load("hero_17/hero_17/Body.skn","hero_17/hero_17/Body.mtl");
		
		m_pAnimComp = pChar->CreateComponent<AnimationComponent>();
		m_pAnimComp->SetSkeletonPath("hero_17/hero_17/body.ske");
		
		RefPtr<AnimClipNode> pClipNode = CreateClipNode("hero_17/102/bip01.ska","102");
		RefPtr<LookAtModifier> pLookAt = CreateLookAtModifier();
		pLookAt->SetBoneName("Bone12");
		pLookAt->SetUpLs(Vector3::UNIT_Y);
		pLookAt->SetFowardLS(Vector3::UNIT_X);
		pLookAt->SetMaxYaw(Math::HALF_PI);
		pClipNode->AddPoseModifier(pLookAt.get());

		pAnimSet->AddSkelAnim(pClipNode.get());
		pAnimSet->AddSkelAnim( CreateClipNode("hero_17/100/bip01.ska","100").get() );
	
		m_pAnimComp->SetAnimationSet(pAnimSet.get());
		
		m_pAnimComp->PlayAnimation(102);
		//m_pAnimComp->PlayAnimation(pClipNode.get());

	}

	void SampleAnimationIK::UnLoad()
	{
	}

	void SampleAnimationIK::Render()
	{
		//m_pAnimtionObjectA->DebugRender();
		//m_pAnimtionObjectB->DebugRender();
	}

	void SampleAnimationIK::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;

		if (key == Keyboard::KEY_ONE)
		{
			m_pAnimComp->PlayAnimation(100);
		}
		else if (key ==Keyboard::KEY_TWO)
		{
			m_pAnimComp->PlayAnimation(102);
		}
		else if (key == Keyboard::KEY_A)
		{
			RenderScheme* pRenderScheme = GetCamera()->GetScene()->GetRenderScheme();
			pRenderScheme->SetSMAAEnabled( !pRenderScheme->GetSMAAEnabled() );
		}
	}

	void SampleAnimationIK::Update()
	{
		Input* pInput = GetInput();
		if (pInput == NULL)
			return;
		
		m_pAnimComp->SetGoalWorldSpace(m_pCamera->GetEyeNode()->GetPosWS());
	}

}


