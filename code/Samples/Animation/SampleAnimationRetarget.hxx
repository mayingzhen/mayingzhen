#include "SampleAnimationRetarget.h"

namespace ma
{
	SampleAnimationRetarget::SampleAnimationRetarget()
	{
	}

	void CreateMaterialData_(const char* pszTexture,const char* pMatPath)
	{
		MaterialData matData;

		SubMaterialData& subMatData = matData.AddSubMatData();
		subMatData.SetShaderName("default");
		subMatData.SetShderMacro("DIFFUSE;SKIN");

		RefPtr<SamplerState> pDiff = CreateSamplerState();
		pDiff->SetTexturePath(pszTexture);

		subMatData.AddParameter("u_texture", Any(pDiff) );

		matData.SaveToXML(pMatPath);
	}

	void SampleAnimationRetarget::Load()
	{	
		m_pCamera->GetSceneNode()->LookAt(Vector3(0, 200, 600), Vector3(0, 0, 0));

		CreateMaterialData_("gigi/gigi/body_b.tga","gigi/gigi/body_b.mat");
		CreateMaterialData_("gigi/gigi/body_f.tga","gigi/gigi/body_f.mat");
		CreateMaterialData_("gigi/gigi/body_h.tga","gigi/gigi/body_h.mat");

		CreateMaterialData_("magician/body.tga","magician/Body.mat");

		{
			AnimationSet animSetData;
			animSetData.AddAnimClip("gigi/100_stand/bip01.ska","100_stand");
			animSetData.AddAnimClip("gigi/210_run/bip01.ska","210_run");
			animSetData.AddAnimClip("gigi/281_run_jump_start/bip01.ska","281_run_jump_start");
			animSetData.AddAnimClip("gigi/282_jump_twostage/bip01.ska","282_jump_twostage");
			animSetData.AddAnimClip("gigi/283_run_jumping/bip01.ska","283_run_jumping");
			animSetData.AddAnimClip("gigi/285_run_jump_stop/bip01.ska","285_run_jump_stop");
			animSetData.SaveToXML("gigi/gigi/body.aniset");
		}
	
		{
			AnimationSet animSetData;
 			animSetData.AddAnimClip("magician/100/bip01.ska","100");
 			animSetData.AddAnimClip("magician/120/bip01.ska","120");
 			animSetData.AddAnimClip("magician/602/bip01.ska","602");
 			animSetData.SaveToXML("magician/body.aniset");
		}


		// character A MeshData & skeleton & Animation
		{
			// MeshData B (b f h)
			SceneNodePtr pChargigi = m_pScene->CreateNode("gigi");
			MeshComponentPtr pMesCompb = pChargigi->CreateComponent<MeshComponent>();
			pMesCompb->Load("gigi/gigi/body_b.skn","gigi/gigi/body_b.mat");

			MeshComponentPtr pMesComph = pChargigi->CreateComponent<MeshComponent>();
			pMesComph->Load("gigi/gigi/body_h.skn","gigi/gigi/body_h.mat");

			MeshComponentPtr pMesCompf = pChargigi->CreateComponent<MeshComponent>();
			pMesCompf->Load("gigi/gigi/body_f.skn","gigi/gigi/body_f.mat");
			
			m_pAnimtionObjectA = pChargigi->CreateComponent<AnimationComponent>();
			m_pAnimtionObjectA->Load("gigi/gigi/body.aniset","gigi/gigi/body.ske");
 
 			m_pAnimtionObjectA->PlayAnimation((UINT)1);	

			pChargigi->RotateAround(Vector3::ZERO, Vector3::UNIT_X, -90);

			pChargigi->Right(-50.0f);
		}

		// character B MeshData & skeleton & Animation
		{
			SceneNodePtr pCharMagic = m_pScene->CreateNode("magic");

			MeshComponentPtr pMeshComp = pCharMagic->CreateComponent<MeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.mat");

			m_pAnimtionObjectB = pCharMagic->CreateComponent<AnimationComponent>();
			m_pAnimtionObjectB->Load("gigi/gigi/body.aniset","magician/Body.ske");

			m_pAnimtionObjectB->PlayAnimation((UINT)0);
			
			pCharMagic->RotateAround(Vector3::ZERO, Vector3::UNIT_X, -90);

			pCharMagic->Right(50.0f);

			
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


