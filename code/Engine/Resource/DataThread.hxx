#include "DataThread.h"

namespace ma
{

	void DataThread::Run()
	{	
		// loop waiting for player events. If the exit event is signaled
		// the thread will exit
		while(!m_bExit)
		{	
			m_pReadEvent->Wait();

			if(m_bExit) 
				break;

			m_bFree = false;
			while(!m_queUnloaded.empty())
			{	
				// 队列锁
				m_csRequestQueue.Lock();
				IDataObj* pObj = m_queUnloaded.front();
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

		m_bFree = true;
	} 

	bool DataThread::IsFree(void)
	{
		return m_bFree;
	}

	DataThread::DataThread()
	{
		m_bFree = true;
		m_bExit = false;
		m_bImmediate = false;

		m_pReadEvent = new CMyEvent();

		//m_pThread = new Thread(MyDataThread,this);
	}

	DataThread::~DataThread()
	{
		SAFE_DELETE(m_pThread);
		SAFE_DELETE(m_pReadEvent);
	}

	void DataThread::Start()
	{

	}

	void DataThread::Stop()
	{

	}

	IDataObj* DataThread::PopUpDataObj(void)
	{	
		AutoLock lock(m_csLoadedQueue);
		if(!m_queLoaded.empty())
		{	
			IDataObj* pObj = m_queLoaded.front();
			m_queLoaded.pop_front();
			return pObj;
		}
		else 
		{
			return NULL;
		}
	}	

	IDataObj* DataThread::PushBackDataObj(IDataObj* pObj)
	{	
		if(NULL == pObj)
			return NULL;

		// 加载线程没有使用m_queUnloadedBuffer!
		//if(pObj->GetExigence() == DATA_THREAD_EXIGENCE_IMMEDIATE || m_bImmediate)
		//{
		//	pObj->Load();
		//	return pObj;
		//}
		//else
		{
			// m_queUnloadedBuffer不被背景加载线程使用,不需要同步.
			m_queUnloadedBuffer.push_back(pObj);
		}
		return NULL;
	}


	bool DataThread::Process()
	{	
		while(!m_queLoaded.empty())
		{	
			m_csLoadedQueue.Lock();
			IDataObj* pObj = m_queLoaded.front();
			m_queLoaded.pop_front();
			m_csLoadedQueue.Unlock();	
			if (pObj)
				pObj->CreateFromMemeory();
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

