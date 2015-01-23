#include "ParticleSystem.h"

namespace ma
{
	static ParticleSystem* gpParticleSystem = NULL;

	ParticleSystem* GetParticleSystem()
	{
		return gpParticleSystem;
	}

	void SetParticleSystem(ParticleSystem* pParticleMang)
	{
		gpParticleSystem = pParticleMang;
	}

	ParticleSystem::ParticleSystem()
	{	
		m_pParticleThread = NULL;
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::Init(bool bThread)
	{
		if (bThread)
		{
			m_pParticleThread = new ParticleThread();
			m_pParticleThread->Start();
		}
	}

	void ParticleSystem::Shutdown()
	{
		if (m_pParticleThread)
		{
			m_pParticleThread->Stop();
			SAFE_DELETE(m_pParticleThread);
		}
	}

	void ParticleSystem::OnFlushFrame()
	{
		if (m_pParticleThread)
			m_pParticleThread->OnFlushFrame();
	}

	void ParticleSystem::Update()
	{
		//if (m_pParticleThread)
		//	m_pParticleThread->FlushRenderQueue();
	}

	void ParticleSystem::AddParticleBatch(ParticleBatch* pParticle)
	{
		pParticle->SetUpdate(false);
		
		if (m_pParticleThread)
		{	
			m_pParticleThread->AddParticleBatch(pParticle);
		}
	}

// 	ParticleEmitter* ParticleSystem::Create(const char* url)
// 	{
// 		ParticleEmitter* pEmit = new ParticleEmitter(NULL);
// 		pEmit->Load(url);
// 		m_arrPaticleEmit.push_back(pEmit);
// 		return pEmit;
// 	}

// 	void ParticleSystem::Delete(ParticleEmitter* pEmit)
// 	{
// 		std::vector<ParticleEmitter*>::iterator it = std::find(m_arrPaticleEmit.begin(),m_arrPaticleEmit.end(),pEmit);
// 		if (it != m_arrPaticleEmit.end())
// 		{
// 			m_arrPaticleEmit.erase(it);
// 		}
// 		//SAFE_DELETE(pEmit);
// 	}


}