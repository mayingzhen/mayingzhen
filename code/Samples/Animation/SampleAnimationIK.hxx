#include "SampleAnimationIK.h"


namespace ma
{
	SampleAnimationIK::SampleAnimationIK()
	{
	}


	void SampleAnimationIK::Load()
	{	
		m_pCamera->LookAt(Vector3(0, -600, 200), Vector3(0, 0, 0));

		CreateMeshMaterial("magician/magician/body.tga","magician/magician/Body.mtl","SKIN");

		RefPtr<AnimationSet> pAnimSet = CreateAnimationSet();

		RefPtr<SceneNode> pCharMagic = CreateSceneNode();

		SkinMeshComponent* pMeshComp = pCharMagic->CreateComponent<SkinMeshComponent>();
		pMeshComp->Load("magician/magician/Body.skn","magician/magician/Body.mtl");

		//m_pAnimtionObjectB = pCharMagic->CreateComponent<AnimationComponent>();
		//m_pAnimtionObjectB->Load("magician/magician/body.aniset","magician/magician/Body.ske");

	
	}

	void SampleAnimationIK::UnLoad()
	{

	}

	void SampleAnimationIK::Render()
	{
		//m_pAnimtionObjectA->DebugRender();
		//m_pAnimtionObjectB->DebugRender();
	}

	void SampleAnimationIK::Update()
	{
		Input* pInput = GetInput();
		if (pInput == NULL)
			return;

	}

}


