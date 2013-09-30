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

		void				AddEmitter(ParticleEmitter* pEmit);

		void				FlushRenderQueue();
	
		void				OnFlushFrame();

	private:
		void				Run();
	

	private:
		typedef std::deque<ParticleEmitter*> ParticleEmitterQueue;
		ParticleEmitterQueue			m_quePaticleEmit[2];

		CMyEvent*						m_pEvent; 

		CriticalSection					m_csQueue;	
	};
}

#endif // _ParticleThread_H_

