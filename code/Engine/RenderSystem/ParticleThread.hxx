#include "ParticleThread.h"
#include "Engine/Thread/Thread.h"

namespace ma
{
	void ParticleThread::Run()
	{
		while (1)
		{
			m_pEvent->Wait();

			int index = GetRenderThread()->CurThreadFill();
			ParticleEmitterQueue& quePaticleEmit = m_quePaticleEmit[index];
			while (!quePaticleEmit.empty())
			{
				m_csQueue.Lock();
				ParticleEmitter* pEmit = quePaticleEmit.front();
				quePaticleEmit.pop_front();
				m_csQueue.Unlock();

				pEmit->Update();
			}
		}
	}

	ParticleThread::ParticleThread()
	{
		m_pEvent = new CMyEvent();
	}

	ParticleThread::~ParticleThread()
	{
		SAFE_DELETE(m_pEvent);
	}

	void ParticleThread::AddEmitter(ParticleEmitter* pEmit)
	{
		int index = GetRenderThread()->GetThreadList();
		ParticleEmitterQueue& quePaticleEmit = m_quePaticleEmit[index];

		m_csQueue.Lock();
		quePaticleEmit.push_back(pEmit);
		m_csQueue.Unlock();	

		m_pEvent->Signal();
	}

	void ParticleThread::OnFlushFrame()
	{
		int index = GetRenderThread()->CurThreadFill();
		ParticleEmitterQueue& quePaticleEmit = m_quePaticleEmit[index];

		m_csQueue.Lock();
		quePaticleEmit.clear();
		m_csQueue.Unlock();
	}

	void ParticleThread::FlushRenderQueue()
	{
		int index = GetRenderThread()->m_nCurThreadProcess;
		ParticleEmitterQueue& quePaticleEmit = m_quePaticleEmit[index];

		while (!quePaticleEmit.empty())
		{
			m_csQueue.Lock();
			ParticleEmitter* pEmit = quePaticleEmit.front();
			quePaticleEmit.pop_front();
			m_csQueue.Unlock();

			pEmit->Update();
		}
	}
}