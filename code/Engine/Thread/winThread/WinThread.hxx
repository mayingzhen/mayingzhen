#include "WinThread.h"

namespace ma
{
	void ThreadFun(void* pParameter)
	{
		Thread* pThread = (Thread*)pParameter;
		pThread->ThreadLoop();
	}


	Thread::Thread()
	{
		m_hThread = 0;
		m_dwThreadID = 0;
		m_bExit = false;
	}

	void Thread::Start()
	{
		m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun, this, CREATE_SUSPENDED, &m_dwThreadID);

		::ResumeThread(m_hThread);//Æô¶¯Ïß³Ì
	}

	void Thread::Stop()
	{
		m_bExit = true;

		::WaitForSingleObject(m_hThread, 0xFFFFFFFF);

		::CloseHandle(m_hThread);
		m_hThread	= NULL;
		m_dwThreadID = 0;
	}

	DWORD Thread::GetThreadId() 
	{
		return m_dwThreadID;
	}

	void Thread::ThreadLoop()
	{
		while(!m_bExit)
		{
			Update();
		}
	}
}


