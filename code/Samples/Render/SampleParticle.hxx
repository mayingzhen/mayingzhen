#include "SampleParticle.h"


namespace ma
{
	SampleParticle::SampleParticle()
	{
	}


	void SampleParticle::Load()
	{
		Vector3 vEyePos = Vector3(0, 30, 20);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->LookAt(vEyePos,VAtPos);

		RefPtr<SceneNode> pFire = CreateSceneNode();
		m_pScene->GetRootNode()->AddChild(pFire.get());
		//ParticleEmitter* pPaticComp = pFire->CreateComponent<ParticleEmitter>();
		//pPaticComp->Load("Particle/fire.particle");
		//pPaticComp->Start();
	}

	void SampleParticle::UnLoad()
	{
	}


	void SampleParticle::Update()
	{
	}


}


