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
		m_pParticleThread = new ParticleThread();
	}

	ParticleSystem::~ParticleSystem()
	{
		SAFE_DELETE(m_pParticleThread);
	}

	void ParticleSystem::OnFlushFrame()
	{
		m_pParticleThread->OnFlushFrame();
	}

	void ParticleSystem::Update()
	{
		m_pParticleThread->FlushRenderQueue();
	}

	void ParticleSystem::AddParticleEmitter(ParticleEmitter* pParticle)
	{
		pParticle->SetUpdate(false);
		m_pParticleThread->AddEmitter(pParticle);
	}

	ParticleEmitter* ParticleSystem::Create(const char* url)
	{
		ParticleEmitter* pEmit = new ParticleEmitter(NULL);
		pEmit->Load(url);
		m_arrPaticleEmit.push_back(pEmit);
		return pEmit;
	}

	void ParticleSystem::Delete(ParticleEmitter* pEmit)
	{
		std::vector<ParticleEmitter*>::iterator it = std::find(m_arrPaticleEmit.begin(),m_arrPaticleEmit.end(),pEmit);
		if (it != m_arrPaticleEmit.end())
		{
			m_arrPaticleEmit.erase(it);
		}
		//SAFE_DELETE(pEmit);
	}


}