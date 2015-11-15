#include "SampleAnimationRetarget.h"

namespace ma
{
	SampleAnimationRetarget::SampleAnimationRetarget()
	{
	}


	void SampleAnimationRetarget::Load()
	{	
		m_pCamera->LookAt(Vector3(0, 200, 600), Vector3(0, 0, 0));


		// character A MeshData & skeleton & Animation
		if (1)
		{
			//CreateDefaultMaterial("gigi/gigi/body_b.tga","gigi/gigi/body_b.mtl","SKIN");
			//CreateDefaultMaterial("gigi/gigi/body_f.tga","gigi/gigi/body_f.mtl","SKIN");
			//CreateDefaultMaterial("gigi/gigi/body_h.tga","gigi/gigi/body_h.mtl","SKIN");

			RefPtr<AnimationSet> pAnimSet = CreateAnimationSet();
 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/100_stand/bip01.ska","100_stand").get() );
// 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/210_run/bip01.ska","210_run").get() );
// 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/281_run_jump_start/bip01.ska","281_run_jump_start").get() );
// 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/282_jump_twostage/bip01.ska","282_jump_twostage").get() );
// 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/283_run_jumping/bip01.ska","283_run_jumping").get() );
// 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/285_run_jump_stop/bip01.ska","285_run_jump_stop").get() );
			pAnimSet->SaveToXML("gigi/gigi/body.aniset");

			// MeshData B (b f h)
			RefPtr<SceneNode> pChargigi = CreateSceneNode();
			SkinMeshComponent* pMesCompb = pChargigi->CreateComponent<SkinMeshComponent>();
			pMesCompb->Load("gigi/gigi/body_b.skn","gigi/gigi/body_b.mat");

			SkinMeshComponent* pMesComph = pChargigi->CreateComponent<SkinMeshComponent>();
			pMesComph->Load("gigi/gigi/body_h.skn","gigi/gigi/body_h.mat");

			SkinMeshComponent* pMesCompf = pChargigi->CreateComponent<SkinMeshComponent>();
			pMesCompf->Load("gigi/gigi/body_f.skn","gigi/gigi/body_f.mat");
			
			m_pAnimtionObjectA = pChargigi->CreateComponent<AnimationComponent>();
			m_pAnimtionObjectA->Load("gigi/gigi/body.aniset","gigi/gigi/body.ske");
 
			pChargigi->RotateAround(Vector3::ZERO, Vector3::UNIT_X, -90);

			pChargigi->SaveToXML("gigi/gigi/gigi.xml");
		}

		// character B MeshData & skeleton & Animation
		if (1)
		{
			CreateDefaultMaterial("magician/body.tga","magician/Body.mat","SKIN");

			RefPtr<AnimationSet> pAnimSet = CreateAnimationSet();
			pAnimSet->AddSkelAnim( CreateClipNode("magician/100/bip01.ska","100").get() );
			pAnimSet->AddSkelAnim( CreateClipNode("magician/120/bip01.ska","120").get() );
			pAnimSet->AddSkelAnim( CreateClipNode("magician/602/bip01.ska","602").get() );
			pAnimSet->SaveToXML("magician/body.aniset");

			RefPtr<SceneNode> pCharMagic = CreateSceneNode();

			SkinMeshComponent* pMeshComp = pCharMagic->CreateComponent<SkinMeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.mat");

			m_pAnimtionObjectB = pCharMagic->CreateComponent<AnimationComponent>();
			//m_pAnimtionObjectB->Load("gigi/gigi/body.aniset","magician/Body.ske");
			m_pAnimtionObjectB->Load("magician/body.aniset","magician/Body.ske");

			pCharMagic->RotateAround(Vector3::ZERO, Vector3::UNIT_X, -90);

			pCharMagic->SaveToXML("magician/magician.xml");
		}
		
		if (1)
		{
			SceneNode* pChargigi = m_pScene->CreateNode("gigi/gigi/gigi.xml");
			
			pChargigi->Right(-50.0f);

			m_pAnimtionObjectA = pChargigi->GetTypeComponent<AnimationComponent>();
			m_pAnimtionObjectA->PlayAnimation((UINT)1);
		}

		if (1)
		{
			SceneNode* pCharMagic = m_pScene->CreateNode("magician/magician.xml");

			pCharMagic->Right(50.0f);

			m_pAnimtionObjectB = pCharMagic->GetTypeComponent<AnimationComponent>();
			m_pAnimtionObjectB->SetAnimSetPath("gigi/gigi/body.aniset");
			m_pAnimtionObjectB->PlayAnimation((UINT)1);
		}

	}

	void SampleAnimationRetarget::UnLoad()
	{
		m_pAnimtionObjectA = NULL;
		m_pAnimtionObjectB = NULL;
	}

	void SampleAnimationRetarget::Render()
	{
		//m_pAnimtionObjectA->DebugRender();
		//m_pAnimtionObjectB->DebugRender();
	}

	void SampleAnimationRetarget::Update()
	{
		//m_pAnimtionObjectA->SetFrame(5);

// 		Input* pInput = GetInput();
// 		if (pInput == NULL)
// 			return;

// 		if (pInput->IsKeyDown(OIS::KC_1))
// 		{
// 			m_pAnimtionObjectA->PlayAnimation((UINT)0);
// 			m_pAnimtionObjectB->PlayAnimation((UINT)0);
// 		}
// 		else if (pInput->IsKeyDown(OIS::KC_2))
// 		{
// 			m_pAnimtionObjectA->PlayAnimation(1);
// 			m_pAnimtionObjectB->PlayAnimation(1);
// 		}
// 		else if (pInput->IsKeyDown(OIS::KC_3))
// 		{
// 			m_pAnimtionObjectA->PlayAnimation(2);
// 			m_pAnimtionObjectB->PlayAnimation(2);
// 		}
// 		else if (pInput->IsKeyDown(OIS::KC_4))
// 		{
// 			m_pAnimtionObjectA->PlayAnimation(3);
// 			m_pAnimtionObjectB->PlayAnimation(3);
// 		}

	}

}


