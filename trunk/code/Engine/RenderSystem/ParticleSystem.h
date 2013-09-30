#ifndef _ParticleSystem_H_
#define _ParticleSystem_H_


namespace ma
{
	class ParticleEmitter;
	class ParticleThread;

	class ENGINE_API ParticleSystem
	{

	public:
		ParticleSystem();

		~ParticleSystem();

		void				Init();

		void				ShoutDown();

		void				Update();

		void				OnFlushFrame();

		void				AddParticleEmitter(ParticleEmitter* pParticle);

		ParticleEmitter*	Create(const char* url);

		void				Delete(ParticleEmitter* pEmit);
	
	private:
		std::vector<ParticleEmitter*>	m_arrPaticleEmit;

		ParticleThread*					m_pParticleThread;

	};

	ENGINE_API ParticleSystem*	GetParticleSystem();

	ENGINE_API void				SetParticleSystem(ParticleSystem* pParticleMang);	
}

#endif // _ParticleSystem_H_

