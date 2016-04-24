#include "WinThread.h"

namespace ma
{
	void ThreadFun(void* pParameter)
	{
		Thread* pThread = (Thread*)pParameter;
		pThread->ThreadLoop();
	}


	Thread::Thread(const char* pszName)
	{
		m_hThread = 0;
		m_dwThreadID = 0;
		m_strName = pszName ? pszName : "";
		m_bExit = false;
	}

	void Thread::Start()
	{
		m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun, this, 0, &m_dwThreadID);

		SetName(m_strName.c_str());
	}

	void Thread::Stop()
	{
		m_bExit = true;

		::WaitForSingleObject(m_hThread, INFINITE);

		::CloseHandle(m_hThread);
		m_hThread	= NULL;
		m_dwThreadID = 0;
	}

	void Thread::SetName(const char* pszName)
	{
		struct THREADNAME_INFO
		{
			DWORD dwType;
			LPCSTR szName;
			DWORD dwThreadID;
			DWORD dwFlags;
		};

		THREADNAME_INFO threadName;
		threadName.dwType = 0x1000;
		threadName.szName = pszName;
		threadName.dwThreadID = m_dwThreadID;
		threadName.dwFlags = 0;

// 		__try
// 		{
// 			RaiseException( 0x406D1388, 0, sizeof(threadName)/sizeof(DWORD), (ULONG_PTR*)&threadName );
// 		}
// 		__except (EXCEPTION_CONTINUE_EXECUTION)
// 		{
// 		}
	}

	DWORD Thread::GetThreadId() 
	{
		return m_dwThreadID;
	}
}


