#include "ParticleThread.h"
#include "Engine/Thread/Thread.h"

namespace ma
{

	ParticleThread::ParticleThread()
	{
		m_pEvent = new CMyEvent();
	}

	ParticleThread::~ParticleThread()
	{
		SAFE_DELETE(m_pEvent);
	}

	void ParticleThread::ThreadUpdate()
	{
		m_pEvent->Wait();

		int index = GetRenderSystem()->CurThreadFill();
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

	void ParticleThread::AddEmitter(ParticleEmitter* pEmit)
	{
		int index = GetRenderSystem()->GetThreadList();
		ParticleEmitterQueue& quePaticleEmit = m_quePaticleEmit[index];

		m_csQueue.Lock();
		quePaticleEmit.push_back(pEmit);
		m_csQueue.Unlock();	

		m_pEvent->Signal();
	}

	void ParticleThread::OnFlushFrame()
	{
		int index = GetRenderSystem()->CurThreadFill();
		ParticleEmitterQueue& quePaticleEmit = m_quePaticleEmit[index];

		m_csQueue.Lock();
		quePaticleEmit.clear();
		m_csQueue.Unlock();
	}

	void ParticleThread::FlushRenderQueue()
	{
		int index = GetRenderSystem()->CurThreadProcess();
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