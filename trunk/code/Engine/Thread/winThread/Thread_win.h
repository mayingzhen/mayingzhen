#ifndef _Thread_win_h__
#define _Thread_win_h__


namespace ma
{
	typedef void (*ThreadMain)(void* pParameter);

	class Thread
	{	
	public:
		Thread(ThreadMain pFunPtr,void* pParameter)
		{
			m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pFunPtr, pParameter, CREATE_SUSPENDED, &m_dwThreadID);

			::ResumeThread(m_hThread);//Æô¶¯Ïß³Ì
		}

		~Thread()
		{
			::WaitForSingleObject(m_hThread, 0xFFFFFFFF);

			::CloseHandle(m_hThread);
			m_hThread	=NULL;
		}


	public:
		HANDLE	m_hThread;
		DWORD	m_dwThreadID;
	};

}


#endif // _Thread_win_h__

