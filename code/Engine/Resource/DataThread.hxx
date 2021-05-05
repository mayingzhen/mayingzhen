#include "DataThread.h"

namespace ma
{

	DataThread::DataThread()
	{
	}

	DataThread::~DataThread()
	{
	}

	void DataThread::Start()
	{
		m_thread = std::thread(&DataThread::ThreadLoop,this);
	}

	void DataThread::Stop()
	{
		m_bExit = true;
		m_readEvent.notify_all();
		
		m_thread.join();
	}

	void DataThread::ThreadLoop()
	{
		ProfileOnThreadCreate("DataThread");

		while(!m_bExit)
		{
			DataObjQueue queUnloaded;
			{
				std::lock_guard<std::mutex> locker(m_csRequestQueue);
				std::swap(queUnloaded, m_queUnloaded);
			}

			for (auto resData : queUnloaded)
			{
				resData->LoadFileToMemeory();

				LogInfo("resData->LoadFileToMemeory %s", resData->GetResPath());

				std::lock_guard<std::mutex> locker(m_csLoadedQueue);
				m_queLoaded.push_back(resData);
			}

			std::unique_lock<std::mutex> lock(m_csRequestQueue);
			m_readEvent.wait(lock);
		}
	}

	void DataThread::Process()
	{	
		DataObjQueue queloaded;
		{
			std::lock_guard<std::mutex> locker(m_csLoadedQueue);
			std::swap(queloaded, m_queLoaded);
		}

		while (!queloaded.empty())
		{
			RefPtr<Resource> resData = queloaded.front();
			resData->CreateFromMemeory();
			if (resData->GetResState() == ResReady || resData->GetResState() == ResLoadError)
			{
				queloaded.pop_front();
			}
		}
	}

	void DataThread::PushBackDataObj(Resource* pObj)
	{
		std::unique_lock<std::mutex> lock(m_csRequestQueue);

		m_queUnloaded.push_back(pObj);

		m_readEvent.notify_one();
	}

	DataThread* g_pDataThread = NULL;
	DataThread* GetDataThread()
	{
		return g_pDataThread;
	}
}

