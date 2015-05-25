#include "DataThread.h"

namespace ma
{

	DataThread::DataThread()
	{
	}

	DataThread::~DataThread()
	{
	}

	void DataThread::Stop()
	{
		m_bExit = true;
		m_readEvent.Signal();
		
		Thread::Stop();
	}

	void DataThread::ThreadUpdate()
	{
		m_readEvent.Wait();
		
		while (true)
		{
			m_csRequestQueue.Lock();
			if ( m_queUnloaded.empty() )
			{
				m_csRequestQueue.Unlock();
				break;
			}
			RefPtr<Resource> resData = m_queUnloaded.front();
			m_queUnloaded.pop_front();
			m_csRequestQueue.Unlock();

			resData->LoadFileToMemeory();

			m_csLoadedQueue.Lock();
			m_queLoaded.push_back(resData);
			m_csLoadedQueue.Unlock();
		}
	}

	void DataThread::Process()
	{	
		while (true)
		{
			m_csLoadedQueue.Lock();
			if (m_queLoaded.empty())
			{
				m_csLoadedQueue.Unlock();
				break;
			}
			RefPtr<Resource> resData = m_queLoaded.front();
			m_queLoaded.pop_front();
			m_csLoadedQueue.Unlock();	

			resData->CreateFromMemeory();	
		}
	}

	void DataThread::PushBackDataObj(Resource* pObj)
	{
		m_csRequestQueue.Lock();
		m_queUnloaded.push_back(pObj);
		m_csRequestQueue.Unlock();

		m_readEvent.Signal();
	}
}

