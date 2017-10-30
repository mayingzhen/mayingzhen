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
				m_csRequestQueue.lock();
				if ( m_queUnloaded.empty() )
				{
					m_csRequestQueue.unlock();
					break;
				}
				RefPtr<Resource> resData = m_queUnloaded.front();
				m_queUnloaded.pop_front();
				m_csRequestQueue.unlock();

				resData->LoadFileToMemeory();

				LogInfo("resData->LoadFileToMemeory %s", resData->GetResPath());

				m_csLoadedQueue.lock();
				m_queLoaded.push_back(resData);
				m_csLoadedQueue.unlock();
			}
		}
	}

	void DataThread::Process()
	{	
		//UINT dTime = StaticFunc::GetTime();
		while (true)
		{
			m_csLoadedQueue.lock();
			if (m_queLoaded.empty())
			{
				m_csLoadedQueue.unlock();
				break;
			}

			RefPtr<Resource> resData = m_queLoaded.front();
			resData->IsReady();
			if (resData->GetResState() == ResReady || resData->GetResState() == ResLoadError)
			{
				m_queLoaded.pop_front();
			}
			m_csLoadedQueue.unlock();	
		}
	}

	void DataThread::PushBackDataObj(Resource* pObj)
	{
		m_csRequestQueue.lock();
		m_queUnloaded.push_back(pObj);
		m_csRequestQueue.unlock();

		m_readEvent.Signal();
	}

	DataThread* g_pDataThread = NULL;
	DataThread* GetDataThread()
	{
		return g_pDataThread;
	}
}

