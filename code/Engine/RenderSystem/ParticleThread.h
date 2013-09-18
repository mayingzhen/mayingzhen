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

		void				Init();

		void				ShoutDown();

		void				AddEmitter(ParticleEmitter* pEmit);

	private:
		void				Run();
	

	private:
		std::deque<ParticleEmitter*>	m_quePaticleEmit;

		CMyEvent*						m_pEvent; 
	};

	//ParticleThread*	GetParticleThread();

	//void				SetParticleThread(ParticleThread* pParticleThread);	
}

#endif // _ParticleThread_H_

