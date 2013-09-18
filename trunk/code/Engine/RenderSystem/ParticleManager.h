#ifndef _ParticleManager_H_
#define _ParticleManager_H_


namespace ma
{
	class ParticleEmitter;
	class ParticleThread;

	class ENGINE_API ParticleManager
	{

	public:
		ParticleManager();

		~ParticleManager();

		void				Init();

		void				ShoutDown();

		void				Update();

		void				Render();

		void				OnFlushFrame();

		ParticleEmitter*	Create(const char* url);

		void				Delete(ParticleEmitter* pEmit);
	
	private:
		std::vector<ParticleEmitter*>	m_arrPaticleEmit;

		ParticleThread*					m_pParticleThread;

	};

	ENGINE_API ParticleManager*	GetParticleManager();

	ENGINE_API void				SetParticleManager(ParticleManager* pParticleMang);	
}

#endif // _ParticleManager_H_

