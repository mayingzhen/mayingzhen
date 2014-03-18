#include "ParticleEmitter.h"



namespace ma
{

	IMPL_OBJECT(ParticleEmitter,RenderComponent)

	ParticleEmitter::ParticleEmitter(GameObject* pGameObj)
		:RenderComponent(pGameObj)
	{
		m_pParticleBatch = NULL;
	}

	ParticleEmitter::~ParticleEmitter()
	{
	}

	void ParticleEmitter::Load(const char* pParticleEmitPath)
	{
		m_pParticleBatch = new ParticleBatch();
		m_pParticleBatch->Load(pParticleEmitPath);
	}

	void ParticleEmitter::Show(Camera* pCamera) 
	{
		if ( GetParticleSystem() )
			GetParticleSystem()->AddParticleBatch(m_pParticleBatch.get());

		m_pParticleBatch->SetWorldMatrix( m_pSceneNode->GetWorldMatrix() );

		GetRenderSystem()->GetRenderQueue()->AddRenderObj(RL_Trans,m_pParticleBatch);
	}

	void ParticleEmitter::Start()
	{
		m_pParticleBatch->Start();
	}

	void ParticleEmitter::Stop()
	{
		m_pParticleBatch->Stop();
	}

	void ParticleEmitter::Serialize(Serializer& sl, const char* pszLable/* = "ParticleEmitter"*/)
	{

	}

}
