#ifndef _PosixThread_h__
#define _PosixThread_h__


#include <pthread.h>


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

	protected:
		pthread_t		m_hThread;
		string			m_strName;
        
        volatile bool   m_bExit;
	};


	inline DWORD GetCurrentThreadId()
	{
		return DWORD(pthread_self());
	}
}


#endif // _PosixThread_h__

