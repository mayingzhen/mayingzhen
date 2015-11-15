#include "ParticleEmitter.h"



namespace ma
{
	ParticleEmitter::ParticleEmitter()
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

		MarkDirty();
	}

	void ParticleEmitter::Show(Camera* pCamera) 
	{
		if ( GetParticleSystem() )
			GetParticleSystem()->AddParticleBatch(m_pParticleBatch.get());

		m_pParticleBatch->SetWorldMatrix( m_pSceneNode->GetMatrixWS() );

		m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Trans, m_pParticleBatch.get());
	}

	void ParticleEmitter::Start()
	{
		m_pParticleBatch->Start();

		m_AABB.merge(Vector3::ZERO);
	}

	void ParticleEmitter::Stop()
	{
		m_pParticleBatch->Stop();
	}

	void ParticleEmitter::Serialize(Serializer& sl, const char* pszLable/* = "ParticleEmitter"*/)
	{

	}

}
