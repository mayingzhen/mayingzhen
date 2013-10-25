#include "SampleParticle.h"


namespace ma
{
	SampleParticle::SampleParticle()
	{
	}


	void SampleParticle::Load()
	{
		Vector3 vEyePos = Vector3(0, 20, 30);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->LookAt(vEyePos,VAtPos,vUp);

		GameObject* pFire = GetEntitySystem()->CreateGameObject("Fire");
		ParticleEmitter* pPaticComp = pFire->CreateComponent<ParticleEmitter>();
		pPaticComp->Load("Particle/fire.particle");
		pPaticComp->start();
	}

	void SampleParticle::UnLoad()
	{
		//GetParticleSystem()->Delete(m_pEmitter);	
	}


	void SampleParticle::Update()
	{
	}

	void SampleParticle::Render()
	{	
	}

}


