#include "SampleParticle.h"


namespace ma
{
	SampleParticle::SampleParticle()
	{
		m_pEmitter = NULL;
	}


	void SampleParticle::Load()
	{
		Vector3 vEyePos = Vector3(0, 20, 30);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->LookAt(vEyePos,VAtPos,vUp);

		m_pEmitter = GetParticleManager()->Create("Particle/fire.particle");//ParticleEmitter::create("Particle/fire.particle");
		m_pEmitter->start();
	}

	void SampleParticle::UnLoad()
	{
		//SAFE_DELETE(m_pEmitter);
	}


	void SampleParticle::Update()
	{
// 		if (m_pEmitter)
// 		{	
// 			m_pEmitter->update(GetTimer()->GetFrameDeltaTime());
// 		}

		//GetParticleManager()->A
	}

	void SampleParticle::Render()
	{
// 		if (m_pEmitter)
// 		{
// 			m_pEmitter->Render();
// 		}		
	}

}


