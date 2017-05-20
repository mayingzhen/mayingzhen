#include "DataThread.h"

namespace ma
{

	DataThread::DataThread():Thread("DataThread")
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

	void DataThread::ThreadLoop()
	{
		while(!m_bExit)
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

				LogInfo("resData->LoadFileToMemeory %s", resData->GetResPath());

				m_csLoadedQueue.Lock();
				m_queLoaded.push_back(resData);
				m_csLoadedQueue.Unlock();
			}
		}
	}

	void DataThread::Process()
	{	
		UINT dTime = StaticFunc::GetTime();
		while (true)
		{
			m_csLoadedQueue.Lock();
			if (m_queLoaded.empty())
			{
				m_csLoadedQueue.Unlock();
				break;
			}

			RefPtr<Resource> resData = m_queLoaded.front();
			resData->IsReady();
			if (resData->GetResState() == ResReady || resData->GetResState() == ResLoadError)
			{
				m_queLoaded.pop_front();
			}
			m_csLoadedQueue.Unlock();	
		}
	}

	void DataThread::PushBackDataObj(Resource* pObj)
	{
		m_csRequestQueue.Lock();
		m_queUnloaded.push_back(pObj);
		m_csRequestQueue.Unlock();

		m_readEvent.Signal();
	}

	DataThread* g_pDataThread = NULL;
	DataThread* GetDataThread()
	{
		return g_pDataThread;
	}
}

