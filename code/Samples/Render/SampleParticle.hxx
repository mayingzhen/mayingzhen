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
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);

// 		SceneNodePtr pFire = GetEntitySystem()->CreateGameObject("Fire");
// 		ParticleEmitterPtr pPaticComp = pFire->CreateComponent<ParticleEmitter>();
// 		pPaticComp->Load("Particle/fire.particle");
// 		pPaticComp->Start();
	}

	void SampleParticle::UnLoad()
	{
		//GetParticleSystem()->Delete(m_pEmitter);	
	}


	void SampleParticle::Update()
	{
	}


}


