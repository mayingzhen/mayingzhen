#include "ParticleThread.h"
#include "Engine/Thread/Thread.h"

namespace ma
{
// 	static ParticleThread* gpParticleThread;
// 
// 	ParticleThread* GetParticleThread()
// 	{
// 		return gpParticleThread;
// 	}
// 
// 	void SetParticleThread(ParticleThread* pParticleThread)
// 	{
// 		gpParticleThread = ParticleThread;
// 	}

	void ParticleThread::Run()
	{
		while (1)
		{
			m_pEvent->Wait();

			while (!m_quePaticleEmit.empty())
			{
				ParticleEmitter* pEmit = m_quePaticleEmit.front();
				m_quePaticleEmit.pop_front();

				pEmit->update(GetTimer()->GetFrameDeltaTime());
			}
		}
	}

	


	ParticleThread::ParticleThread()
	{
		m_pEvent = NULL;
	}

	ParticleThread::~ParticleThread()
	{

	}

	void ParticleThread::Init()
	{
		m_pEvent = new CMyEvent();
	}

	void ParticleThread::ShoutDown()
	{
		SAFE_DELETE(m_pEvent);
	}

	void ParticleThread::AddEmitter(ParticleEmitter* pEmit)
	{
		m_quePaticleEmit.push_back(pEmit);

		m_pEvent->Signal();
	}
}