#ifndef _PosixThread_h__
#define _PosixThread_h__


#include <pthread.h>


namespace ma
{
	class Thread
	{	
	public:
		Thread();

		virtual void	ThreadUpdate() = 0;
  
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

	static pthread_mutex_t mutex_t;
	template<typename T>
	const volatile T InterlockedIncrement(volatile T* pT)
	{
		pthread_mutex_lock(&mutex_t);
		++(*pT);
		pthread_mutex_unlock(&mutex_t);
		return *pT;
	}

	template<typename T>
	const volatile T InterlockedDecrement(volatile T* pT)
	{
		pthread_mutex_lock(&mutex_t);
		--(*pT);
		pthread_mutex_unlock(&mutex_t);
		return *pT;
	}

}


#endif // _PosixThread_h__

