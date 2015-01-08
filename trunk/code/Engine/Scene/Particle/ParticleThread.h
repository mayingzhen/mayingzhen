#ifndef _ParticleThread_H_
#define _ParticleThread_H_


namespace ma
{
	class ParticleEmitter;
	class Thread;
	class CMyEvent;

	class ParticleThread : public Thread
	{

	public:
		ParticleThread();

		~ParticleThread();

		void				ThreadUpdate();

		void				AddParticleBatch(ParticleBatch* pEmit);

		void				FlushRenderQueue();
	
		void				OnFlushFrame();


	private:
		typedef std::deque<ParticleBatch*> ParticleBatchQueue;
		ParticleBatchQueue				m_quePaticleBatch[2];

		CMyEvent*						m_pEvent; 

		CriticalSection					m_csQueue;	
	};
}

#endif // _ParticleThread_H_

