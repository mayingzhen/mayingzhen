#include "ParticleManager.h"

namespace ma
{
	static ParticleManager* gpParticleManager = NULL;

	ParticleManager* GetParticleManager()
	{
		return gpParticleManager;
	}

	void SetParticleManager(ParticleManager* pParticleMang)
	{
		gpParticleManager = pParticleMang;
	}

	ParticleManager::ParticleManager()
	{	
		m_pParticleThread = NULL;
	}

	ParticleManager::~ParticleManager()
	{
	}

	void ParticleManager::Init()
	{
		m_pParticleThread = new ParticleThread();
	}

	void ParticleManager::ShoutDown()
	{
		SAFE_DELETE(m_pParticleThread);
	}

	void ParticleManager::Update()
	{
		for (UINT i = 0; i < m_arrPaticleEmit.size(); ++i)
		{
			ParticleEmitter* pEmit = m_arrPaticleEmit[i];
			
			if (m_pParticleThread)
			{
				m_pParticleThread->AddEmitter(pEmit);
			}
			else
			{
				m_arrPaticleEmit[i]->update(GetTimer()->GetFrameDeltaTime());
			}
		}
	}

	void ParticleManager::Render()
	{
		for (UINT i = 0; i < m_arrPaticleEmit.size(); ++i)
		{
			ParticleEmitter* pEmit = m_arrPaticleEmit[i];

			pEmit->draw(GetRenderSystem()->GetCamera());
		}
	}

	void ParticleManager::OnFlushFrame()
	{

	}

	ParticleEmitter* ParticleManager::Create(const char* url)
	{
		ParticleEmitter* pEmit = ParticleEmitter::create(url);
		m_arrPaticleEmit.push_back(pEmit);
		return pEmit;
	}

	void ParticleManager::Delete(ParticleEmitter* pEmit)
	{
		std::vector<ParticleEmitter*>::iterator it = std::find(m_arrPaticleEmit.begin(),m_arrPaticleEmit.end(),pEmit);
		if (it != m_arrPaticleEmit.end())
		{
			m_arrPaticleEmit.erase(it);
		}
		//SAFE_DELETE(pEmit);
	}


}