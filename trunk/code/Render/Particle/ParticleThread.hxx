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

	void ParticleThread::Update()
	{
		m_pEvent->Wait();

		int index = GetRenderThread() ? GetRenderThread()->CurThreadFill() : 0;
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
		int index = GetRenderThread() ? GetRenderThread()->GetThreadList() : 0;
		ParticleEmitterQueue& quePaticleEmit = m_quePaticleEmit[index];

		m_csQueue.Lock();
		quePaticleEmit.push_back(pEmit);
		m_csQueue.Unlock();	

		m_pEvent->Signal();
	}

	void ParticleThread::OnFlushFrame()
	{
		int index = GetRenderThread() ? GetRenderThread()->CurThreadFill() : 0;
		ParticleEmitterQueue& quePaticleEmit = m_quePaticleEmit[index];

		m_csQueue.Lock();
		quePaticleEmit.clear();
		m_csQueue.Unlock();
	}

	void ParticleThread::FlushRenderQueue()
	{
		int index = GetRenderThread()->CurThreadProcess();
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