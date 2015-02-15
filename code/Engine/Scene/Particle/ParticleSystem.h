#ifndef _ParticleSystem_H_
#define _ParticleSystem_H_


namespace ma
{
	class ParticleEmitter;
	class ParticleThread;
	class ParticleBatch;

	class ParticleSystem
	{

	public:
		ParticleSystem();

		~ParticleSystem();

		void				Init(bool bThread);

		void				Shutdown();

		void				Update();

		void				OnFlushFrame();

		void				AddParticleBatch(ParticleBatch* pParticle);

		//ParticleEmitter*	Create(const char* url);

		//void				Delete(ParticleEmitter* pEmit);
	
	private:
	//	std::vector<ParticleBatch*>		m_arrPaticleEmit;

		ParticleThread*					m_pParticleThread;

	};

	 ParticleSystem*	GetParticleSystem();

	 void				SetParticleSystem(ParticleSystem* pParticleMang);	
}

#endif // _ParticleSystem_H_
