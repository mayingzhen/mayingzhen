#ifndef _PosixThread_h__
#define _PosixThread_h__


#include <pthread.h>


namespace ma
{
	class ENGINE_API Thread
	{	
	public:
		Thread();

		virtual void	Update() = 0;
  
        virtual void	Start();
  
        virtual void	Stop();

		virtual void	ThreadLoop();

		DWORD			GetThreadId();

	protected:
		pthread_t	m_hThread;
        
        bool        m_bExit;
	};


	inline DWORD GetCurrentThreadId()
	{
		return DWORD(pthread_self());
	}
}


#endif // _PosixThread_h__

