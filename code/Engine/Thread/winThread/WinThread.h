#ifndef _WinThread_h__
#define _WinThread_h__


namespace ma
{
	class Thread
	{	
	public:
		Thread(const char* pszName);

		virtual void	Start();

		virtual void	Stop();

		virtual void	ThreadLoop() = 0;

		DWORD			GetThreadId();

	private:
		void			SetName(const char* pszName);

	public:
		HANDLE			m_hThread;
		DWORD			m_dwThreadID;
		string			m_strName;

		volatile bool	m_bExit;
	};

}


#endif // _WinThread_h__

