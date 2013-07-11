#include "Engine/stdAfx.h"
#include "DataThread.h"
#include "AutoLock.h"

namespace ma
{
	static DataThread*	gDataThread;

	DataThread*	GetDataThread()
	{
		return gDataThread;
	}

	void SetDataThread(DataThread* pDataThread)
	{
		ASSERT(gDataThread == NULL);
		gDataThread = pDataThread;
	}

	void DataThread::MyDataThread(void* pThis)
	{	
		DataThread* pDataThread = (DataThread*)pThis;
		if(NULL != pDataThread)
		{
			//TlsSetValue(pDataThread->m_dwTlsIndex, (LPVOID)pDataThread->m_dwThreadAppID);

			pDataThread->ProcessLoadRequest();
		}
	}

	void DataThread::ProcessLoadRequest()
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
				// ������
				m_csRequestQueue.Lock();
				IDataObj* pObj = m_queUnloaded.front();
				m_queUnloaded.pop_front();
				m_csRequestQueue.Unlock();

				pObj->LoadToMemory();

				// ʹ�õ�����������m_queLoaded�Ĳ���,ʹ��lock���Ƶ�ʱ��θ���.
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

	DataThread::DataThread(DWORD dwTlsIndex, DWORD dwThreadAppID)
	{
		m_bFree = true;
		m_bExit = false;
		//m_pThreadExitCallback = NULL;
		m_dwTlsIndex = dwTlsIndex;
		m_dwThreadAppID = dwThreadAppID;
		m_bImmediate = false;
		//m_pThreadExitCallback = NULL;

		m_pReadEvent = new CMyEvent();

		m_pThread = new Thread(MyDataThread,this);
	}

	DataThread::~DataThread()
	{
		SAFE_DELETE(m_pThread);
		SAFE_DELETE(m_pReadEvent);
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

		// �����߳�û��ʹ��m_queUnloadedBuffer!
		//if(pObj->GetExigence() == DATA_THREAD_EXIGENCE_IMMEDIATE || m_bImmediate)
		//{
		//	pObj->Load();
		//	return pObj;
		//}
		//else
		{
			// m_queUnloadedBuffer�������������߳�ʹ��,����Ҫͬ��.
			m_queUnloadedBuffer.push_back(pObj);
		}
		return NULL;
	}


	bool DataThread::Process(void)
	{	
		// Note: Process�����̵߳��õ�
		if(this->IsFree())
		{
			m_csLoadedQueue.Lock();
			while(!m_queLoaded.empty())
			{	
				IDataObj* pObj = m_queLoaded.front();
				m_queLoaded.pop_front();
				if (pObj)
					pObj->LoadImp();
			}
			m_csLoadedQueue.Unlock();	

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

