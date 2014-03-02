#include "DataThread.h"

namespace ma
{

	bool DataThread::IsFree(void)
	{
		return m_bFree;
	}

	DataThread::DataThread()
	{
		m_bFree = true;

		m_pReadEvent = new CMyEvent();
	}

	DataThread::~DataThread()
	{
		SAFE_DELETE(m_pThread);
		SAFE_DELETE(m_pReadEvent);
	}

	void DataThread::Update()
	{
		m_pReadEvent->Wait();

		m_bFree = false;

		while(!m_queUnloaded.empty())
		{	
			// 队列锁
			m_csRequestQueue.Lock();
			Resource* pObj = m_queUnloaded.front();
			m_queUnloaded.pop_front();
			m_csRequestQueue.Unlock();

			pObj->LoadFileToMemeory();

			// 使用单独的锁控制m_queLoaded的操作,使得lock控制的时间段更短.
			m_csLoadedQueue.Lock();
			m_queLoaded.push_back(pObj);
			m_csLoadedQueue.Unlock();
		}

		m_bFree = true;
	}

	Resource* DataThread::PopUpDataObj(void)
	{	
		AutoLock lock(m_csLoadedQueue);
		if(!m_queLoaded.empty())
		{	
			Resource* pObj = m_queLoaded.front();
			m_queLoaded.pop_front();
			return pObj;
		}
		else 
		{
			return NULL;
		}
	}	

	void DataThread::PushBackDataObj(Resource* pObj)
	{	
		m_queUnloadedBuffer.push_back(pObj);
	}


	bool DataThread::Process()
	{	
		while(!m_queLoaded.empty())
		{	
			m_csLoadedQueue.Lock();
			Resource* pObj = m_queLoaded.front();
			m_queLoaded.pop_front();
			m_csLoadedQueue.Unlock();	
			
			if (pObj)
			{
				pObj->CreateFromMemeory();
			}
		}

		// Note: Process是主线程调用的
		if(this->IsFree())
		{	
			// 将m_queUnloadedBuffer转入m_queUnloaded 
			// Note: m_queUnloadedBuffer只被主线程操作.
			// Note: 假设加载线程休停在ProcessLoadRequest的m_bFree = false;之前 
			// 则主线程可能多次调用该此Process
			if(!m_queUnloadedBuffer.empty())
			{	
				m_csRequestQueue.Lock();	// 锁住 m_queUnloaded
				m_queUnloaded.insert(m_queUnloaded.end(), m_queUnloadedBuffer.begin(), m_queUnloadedBuffer.end());
				m_csRequestQueue.Unlock();
				m_queUnloadedBuffer.clear();
				m_pReadEvent->Signal();
			}
		}

		return !m_queLoaded.empty();
	}

}

