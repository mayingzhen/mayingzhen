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
			ResData resData = m_queUnloaded.front();
			m_queUnloaded.pop_front();
			m_csRequestQueue.Unlock();

			resData.m_pRes->LoadFileToMemeory();

			// ʹ�õ�����������m_queLoaded�Ĳ���,ʹ��lock���Ƶ�ʱ��θ���.
			m_csLoadedQueue.Lock();
			m_queLoaded.push_back(resData);
			m_csLoadedQueue.Unlock();
		}

		m_bFree = true;
	}

	void DataThread::PushBackDataObj(const ResData& pObj)
	{	
		m_queUnloadedBuffer.push_back(pObj);
	}

	bool DataThread::Process()
	{	
		while(!m_queLoaded.empty())
		{	
			m_csLoadedQueue.Lock();
			ResData resData = m_queLoaded.front();
			m_queLoaded.pop_front();
			m_csLoadedQueue.Unlock();	
			
			ASSERT(resData.m_pRes)
			if (resData.m_pRes)
			{
				resData.m_pRes->CreateFromMemeory();
			}

			if (resData.m_pCallBack)
			{
				resData.m_pCallBack->OnCall(NULL,NULL,NULL);
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

