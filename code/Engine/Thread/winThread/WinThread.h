#ifndef _WinThread_h__
#define _WinThread_h__


namespace ma
{
	class ENGINE_API Thread
	{	
	public:
		Thread();

		virtual	void	ThreadUpdate() = 0;

		virtual void	Start();

		virtual void	Stop();

		virtual void	ThreadLoop();

		DWORD			GetThreadId();

	public:
		HANDLE		m_hThread;
		DWORD		m_dwThreadID;

		bool		m_bExit;
	};

}


#endif // _WinThread_h__

