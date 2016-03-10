#include "SampleAnimationRetarget.h"


namespace ma
{
	SampleAnimationRetarget::SampleAnimationRetarget()
	{
	}


	void SampleAnimationRetarget::Load()
	{	
		m_pCamera->LookAt(Vector3(0, -600, 200), Vector3(0, 0, 0));

		// character A MeshData & skeleton & Animation
		if (1)
		{
 			CreateMeshMaterial("gigi/gigi/body_b.tga","gigi/gigi/body_b.mtl","SKIN");
 			CreateMeshMaterial("gigi/gigi/body_f.tga","gigi/gigi/body_f.mtl","SKIN");
 			CreateMeshMaterial("gigi/gigi/body_h.tga","gigi/gigi/body_h.mtl","SKIN");

			RefPtr<AnimationSet> pAnimSet = CreateAnimationSet();
 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/100_stand/bip01.ska","100",NULL,"gigi/gigi/body.ske").get() );
 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/210_run/bip01.ska","200",NULL,"gigi/gigi/body.ske").get() );
 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/281_run_jump_start/bip01.ska","300",NULL,"gigi/gigi/body.ske").get() );
			pAnimSet->AddSkelAnim( CreateClipNode("gigi/282_jump_twostage/bip01.ska","400",NULL,"gigi/gigi/body.ske").get() );
 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/283_run_jumping/bip01.ska","500",NULL,"gigi/gigi/body.ske").get() );
 			pAnimSet->AddSkelAnim( CreateClipNode("gigi/285_run_jump_stop/bip01.ska","600",NULL,"gigi/gigi/body.ske").get() );
			pAnimSet->SaveToXML("gigi/gigi/body.aniset");

			// MeshData B (b f h)
			RefPtr<SceneNode> pChargigi = CreateSceneNode();
			SkinMeshComponent* pMesCompb = pChargigi->CreateComponent<SkinMeshComponent>();
			pMesCompb->Load("gigi/gigi/body_b.skn","gigi/gigi/body_b.mtl");

			SkinMeshComponent* pMesComph = pChargigi->CreateComponent<SkinMeshComponent>();
			pMesComph->Load("gigi/gigi/body_h.skn","gigi/gigi/body_h.mtl");

			SkinMeshComponent* pMesCompf = pChargigi->CreateComponent<SkinMeshComponent>();
			pMesCompf->Load("gigi/gigi/body_f.skn","gigi/gigi/body_f.mtl");
			
			m_pAnimtionObjectA = pChargigi->CreateComponent<AnimationComponent>();
			m_pAnimtionObjectA->Load("gigi/gigi/body.aniset","gigi/gigi/body.ske");

			pChargigi->SaveToXML("gigi/gigi/gigi.xml");
		}

		// character B MeshData & skeleton & Animation
		if (1)
		{
			m_pScene->GetDirLight()->GetSceneNode()->LookAt(Vector3(0,0,1),Vector3(0,0,0));
			m_pScene->GetDirLight()->SetLightColor(ColourValue(0.5,0.5,0.5,1.0f));
			m_pScene->SetAmbientColor(Vector3(0.5,0.5,0.5));


			CreateMeshMaterial("magician/magician/body.tga","magician/magician/Body.mtl","SKIN;LIGHT;BRDF;SPEC");

			RefPtr<AnimationSet> pAnimSet = CreateAnimationSet();
			pAnimSet->AddSkelAnim( CreateClipNode("magician/100/bip01.ska","100",NULL,"magician/magician/Body.ske").get() );
			pAnimSet->AddSkelAnim( CreateClipNode("magician/120/bip01.ska","200",NULL,"magician/magician/Body.ske").get() );
			pAnimSet->AddSkelAnim( CreateClipNode("magician/602/bip01.ska","300",NULL,"magician/magician/Body.ske").get() );
			pAnimSet->SaveToXML("magician/magician/body.aniset");

			RefPtr<SceneNode> pCharMagic = CreateSceneNode();

			SkinMeshComponent* pMeshComp = pCharMagic->CreateComponent<SkinMeshComponent>();
			pMeshComp->Load("magician/magician/Body.skn","magician/magician/Body.mtl");

			m_pAnimtionObjectB = pCharMagic->CreateComponent<AnimationComponent>();
			m_pAnimtionObjectB->Load("magician/magician/body.aniset","magician/magician/Body.ske");

			pCharMagic->SaveToXML("magician/magician/magician.xml");
		}
		
		if (1)
		{
			SceneNode* pChargigi = m_pScene->CreateSceneNode("gigi/gigi/gigi.xml");
			
			pChargigi->Right(-50.0f);

			m_pAnimtionObjectA = pChargigi->GetTypeComponent<AnimationComponent>();

			//m_pAnimtionObjectA->SetAnimSetPath("magician/magician/body.aniset");

		}

		if (1)
		{
			SceneNode* pCharMagic = m_pScene->CreateSceneNode("magician/magician/magician.xml");

			pCharMagic->Right(50.0f);

			m_pAnimtionObjectB = pCharMagic->GetTypeComponent<AnimationComponent>();

			m_pAnimtionObjectB->SetAnimSetPath("gigi/gigi/body.aniset");
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
		Input* pInput = GetInput();
		if (pInput == NULL)
			return;

		if (pInput->IsKeyDown(Keyboard::KEY_ONE))
		{
			m_pAnimtionObjectA->PlayAnimation("100");
			m_pAnimtionObjectB->PlayAnimation("100");
		}
		else if (pInput->IsKeyDown(Keyboard::KEY_TWO))
		{
			m_pAnimtionObjectA->PlayAnimation("200");
			m_pAnimtionObjectB->PlayAnimation("200");
		}
		else if (pInput->IsKeyDown(Keyboard::KEY_THREE))
		{
			m_pAnimtionObjectA->PlayAnimation("300");
			m_pAnimtionObjectB->PlayAnimation("300");
		}
		else if (pInput->IsKeyDown(Keyboard::KEY_FOUR))
		{
			m_pAnimtionObjectA->PlayAnimation("400");
			m_pAnimtionObjectB->PlayAnimation("400");
		}

	}

}


