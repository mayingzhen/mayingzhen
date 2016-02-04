#include "PosixThread.h"


namespace ma
{
	static void* ThreadFun(void* pParameter)
	{
		Thread* pThread = (Thread*)pParameter;
		pThread->ThreadLoop();
	}

	Thread::Thread(const char* pszName)
	{
		m_hThread = NULL;
		m_strName = pszName ? pszName : "";
		m_bExit = false;
	}

	void Thread::Start()
    {
		pthread_create(&m_hThread, NULL, ThreadFun, this);
    }
        
    void Thread::Stop()
    {
		m_bExit = true;
		pthread_join(m_hThread, NULL);
		m_hThread = NULL;
    }

	void Thread::SetName(const char* pszName)
	{

	}

	DWORD Thread::GetThreadId()
	{
		return DWORD(m_hThread);
	}


	void Thread::ThreadLoop()
	{
		while(!m_bExit)
		{
			ThreadUpdate();
		}
	}
}


