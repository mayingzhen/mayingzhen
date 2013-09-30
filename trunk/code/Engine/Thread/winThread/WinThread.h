#ifndef _WinThread_h__
#define _WinThread_h__


namespace ma
{
	class Thread
	{	
	public:
		Thread()
		{
			m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun, this, CREATE_SUSPENDED, &m_dwThreadID);

			::ResumeThread(m_hThread);//Æô¶¯Ïß³Ì
		}

		~Thread()
		{
			::WaitForSingleObject(m_hThread, 0xFFFFFFFF);

			::CloseHandle(m_hThread);
			m_hThread	=NULL;
		}

		
		DWORD			GetThreadId() {return m_dwThreadID;}

protected:

		virtual void	Run() = 0;

		static void		ThreadFun(void* pParameter)
		{
			Thread* pThread = (Thread*)pParameter;
			pThread->Run();
		}

	public:
		HANDLE	m_hThread;
		DWORD	m_dwThreadID;
	};

}


#endif // _WinThread_h__

