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
		ParticleBatchQueue& quePaticleBatch = m_quePaticleBatch[index];

		while (true)
		{
			m_csQueue.Lock();
			if ( quePaticleBatch.empty() ) 
			{
				m_csQueue.Unlock();
				break;
			}
			
			ParticleBatch* pParticleBatch = quePaticleBatch.front();
			quePaticleBatch.pop_front();
			m_csQueue.Unlock();
		
			if (pParticleBatch)
			{
				pParticleBatch->Calculate(false);
			}

		}
	}

	void ParticleThread::AddParticleBatch(ParticleBatch* pEmit)
	{
		int index = GetRenderSystem()->CurThreadFill();
		ParticleBatchQueue& quePaticleBatch = m_quePaticleBatch[index];

		m_csQueue.Lock();
		quePaticleBatch.push_back(pEmit);
		m_csQueue.Unlock();	

		m_pEvent->Signal();
	}

	void ParticleThread::OnFlushFrame()
	{
		int index = GetRenderSystem()->CurThreadFill();
		ParticleBatchQueue& quePaticleBatch = m_quePaticleBatch[index];

		m_csQueue.Lock();
		quePaticleBatch.clear();
		m_csQueue.Unlock();
	}

	void ParticleThread::FlushRenderQueue()
	{
		int index = GetRenderSystem()->CurThreadFill();
		ParticleBatchQueue& quePaticleBatch = m_quePaticleBatch[index];

		while (true)
		{
			m_csQueue.Lock();
			if ( quePaticleBatch.empty() ) 
			{
				m_csQueue.Unlock();
				break;
			}

			ParticleBatch* pParticleBatch = quePaticleBatch.front();
			quePaticleBatch.pop_front();
			m_csQueue.Unlock();

			if (pParticleBatch)
			{
				pParticleBatch->Calculate(false);
			}
		}
	}
}