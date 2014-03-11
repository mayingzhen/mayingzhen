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

	void DataThread::ThreadUpdate()
	{
		m_pReadEvent->Wait();

		m_bFree = false;

		while(!m_queUnloaded.empty())
		{	
			// ������
			m_csRequestQueue.Lock();
			ResourcePtr pObj = m_queUnloaded.front();
			m_queUnloaded.pop_front();
			m_csRequestQueue.Unlock();

			pObj->LoadFileToMemeory();

			// ʹ�õ�����������m_queLoaded�Ĳ���,ʹ��lock���Ƶ�ʱ��θ���.
			m_csLoadedQueue.Lock();
			m_queLoaded.push_back(pObj);
			m_csLoadedQueue.Unlock();
		}

		m_bFree = true;
	}

// 	Resource* DataThread::PopUpDataObj(void)
// 	{	
// 		AutoLock lock(m_csLoadedQueue);
// 		if(!m_queLoaded.empty())
// 		{	
// 			ResourcePtr pObj = m_queLoaded.front();
// 			m_queLoaded.pop_front();
// 			return pObj;
// 		}
// 		else 
// 		{
// 			return NULL;
// 		}
// 	}	

	void DataThread::PushBackDataObj(ResourcePtr pObj)
	{	
		m_queUnloadedBuffer.push_back(pObj);
	}


	bool DataThread::Process()
	{	
		while(!m_queLoaded.empty())
		{	
			m_csLoadedQueue.Lock();
			ResourcePtr pObj = m_queLoaded.front();
			m_queLoaded.pop_front();
			m_csLoadedQueue.Unlock();	
			
			if (pObj)
			{
				pObj->CreateFromMemeory();
			}
		}

		// Note: Process�����̵߳��õ�
		if(this->IsFree())
		{	
			// ��m_queUnloadedBufferת��m_queUnloaded 
			// Note: m_queUnloadedBufferֻ�����̲߳���.
			// Note: ��������߳���ͣ��ProcessLoadRequest��m_bFree = false;֮ǰ 
			// �����߳̿��ܶ�ε��øô�Process
			if(!m_queUnloadedBuffer.empty())
			{	
				m_csRequestQueue.Lock();	// ��ס m_queUnloaded
				m_queUnloaded.insert(m_queUnloaded.end(), m_queUnloadedBuffer.begin(), m_queUnloadedBuffer.end());
				m_csRequestQueue.Unlock();
				m_queUnloadedBuffer.clear();
				m_pReadEvent->Signal();
			}
		}

		return !m_queLoaded.empty();
	}

}

