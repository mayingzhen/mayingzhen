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
			GameObjectPtr pChargigi = GetEntitySystem()->CreateGameObject("gigi");
			MeshComponentPtr pMesCompb = pChargigi->CreateComponent<MeshComponent>();
			pMesCompb->Load("gigi/gigi/body_b.skn","gigi/gigi/body_b.mat");

			MeshComponentPtr pMesComph = pChargigi->CreateComponent<MeshComponent>();
			pMesComph->Load("gigi/gigi/body_h.skn","gigi/gigi/body_h.mat");

			MeshComponentPtr pMesCompf = pChargigi->CreateComponent<MeshComponent>();
			pMesCompf->Load("gigi/gigi/body_f.skn","gigi/gigi/body_f.mat");
			
			m_pAnimtionObjectA = pChargigi->CreateComponent<AnimationComponent>();
			m_pAnimtionObjectA->Load("gigi/gigi/body.aniset","gigi/gigi/body.ske");
 
 			m_pAnimtionObjectA->PlayAnimation((UINT)0);	

			pChargigi->GetSceneNode()->Translate(Vector3(-50,-100,0));
		}

		// character B MeshData & skeleton & Animation
		{
			GameObjectPtr pCharMagic = GetEntitySystem()->CreateGameObject("magic");

			MeshComponentPtr pMeshComp = pCharMagic->CreateComponent<MeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.mat");

			m_pAnimtionObjectB = pCharMagic->CreateComponent<AnimationComponent>();
			m_pAnimtionObjectB->Load("gigi/gigi/body.aniset","magician/Body.ske");

			m_pAnimtionObjectB->PlayAnimation((UINT)0);

 			pCharMagic->GetSceneNode()->Translate(Vector3(50,-100,0));
		}
	}

	void SampleAnimationRetarget::UnLoad()
	{
		m_pAnimtionObjectA = NULL;
		m_pAnimtionObjectB = NULL;
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

