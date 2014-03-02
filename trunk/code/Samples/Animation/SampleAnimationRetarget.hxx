#include "SampleAnimationRetarget.h"

namespace ma
{
	SampleAnimationRetarget::SampleAnimationRetarget()
	{
	}


	void SampleAnimationRetarget::Load()
	{	

		// character A MeshData & skeleton & Animation
		{
			// MeshData B (b f h)
			m_pChargigi = GetEntitySystem()->CreateGameObject("gigi");
			MeshComponent* pMesCompb = m_pChargigi->CreateComponent<MeshComponent>();
			pMesCompb->Load("gigi/gigi/body_b.skn","gigi/gigi/body_b.tga");

			MeshComponent* pMesComph = m_pChargigi->CreateComponent<MeshComponent>();
			pMesComph->Load("gigi/gigi/body_h.skn","gigi/gigi/body_h.tga");

			MeshComponent* pMesCompf = m_pChargigi->CreateComponent<MeshComponent>();
			pMesCompf->Load("gigi/gigi/body_f.skn","gigi/gigi/body_f.tga");
			
			m_pAnimtionObjectA = m_pChargigi->CreateComponent<IAnimationObject>();
			m_pAnimtionObjectA->Load(NULL,"gigi/gigi/body.ske");

// 			IAnimationSet* pAnimSet = m_pAnimtionObjectA->GetAnimationSet();
// 
// 			pAnimSet->AddAnimClip("gigi/210_run/bip01.ska","gigi_Run");
// 			pAnimSet->AddAnimClip("gigi/281_run_jump_start/bip01.ska","gigi_jump");
// 			pAnimSet->AddAnimClip("magician/602/bip01.ska","Mag602");
// 			pAnimSet->AddAnimClip("magician/100/bip01.ska","mag100");
// 
// 			m_pAnimtionObjectA->PlayAnimation((UINT)0);	

			m_pChargigi->GetSceneNode()->Translate(Vector3(-50,-100,0));
		}

		// character B MeshData & skeleton & Animation
		{
// 			m_pCharMagic = GetEntitySystem()->CreateGameObject("magic");
// 
// 			MeshComponent* pMeshComp = m_pCharMagic->CreateComponent<MeshComponent>();
// 			pMeshComp->Load("magician/Body.skn","magician/Body.tga");
// 
// 			AnimComponent* pAnimComp = m_pCharMagic->CreateComponent<AnimComponent>();
// 			pAnimComp->Load(NULL,"magician/Body.ske");
// 
// 			m_pAnimtionObjectB = pAnimComp->GetAnimObject();
// 
// 			IAnimationSet* pAnimSet = m_pAnimtionObjectB->GetAnimationSet();
// 			
// 			pAnimSet->AddAnimClip("gigi/210_run/bip01.ska","gigi_Run");
// 			pAnimSet->AddAnimClip("gigi/281_run_jump_start/bip01.ska","gigi_jump");
// 			pAnimSet->AddAnimClip("magician/602/bip01.ska","Mag602");
// 			pAnimSet->AddAnimClip("magician/100/bip01.ska","mag100");
// 
// 			m_pAnimtionObjectB->PlayAnimation((UINT)0);
// 
//  			m_pCharMagic->GetSceneNode()->Translate(Vector3(50,-100,0));
		}
	}

	void SampleAnimationRetarget::UnLoad()
	{
		GetEntitySystem()->DeleteGameObject(m_pChargigi);
		GetEntitySystem()->DeleteGameObject(m_pCharMagic);
	}

	void SampleAnimationRetarget::Update()
	{
		Input* pInput = GetInput();
		if (pInput == NULL)
			return;

		if (pInput->IsKeyDown(OIS::KC_1))
		{
			m_pAnimtionObjectA->PlayAnimation((UINT)0);
			m_pAnimtionObjectB->PlayAnimation((UINT)0);
		}
		else if (pInput->IsKeyDown(OIS::KC_2))
		{
			m_pAnimtionObjectA->PlayAnimation(1);
			m_pAnimtionObjectB->PlayAnimation(1);
		}
		else if (pInput->IsKeyDown(OIS::KC_3))
		{
			m_pAnimtionObjectA->PlayAnimation(2);
			m_pAnimtionObjectB->PlayAnimation(2);
		}
		else if (pInput->IsKeyDown(OIS::KC_4))
		{
			m_pAnimtionObjectA->PlayAnimation(3);
			m_pAnimtionObjectB->PlayAnimation(3);
		}

	}

}


