#ifndef _Thread_Android_h__
#define _Thread_Android_h__


#include <pthread.h>


namespace ma
{
	typedef void (*ThreadMain)(void* pParameter);

	class Thread
	{	
	public:
		Thread(ThreadMain pFunPtr,void* pParameter)
		{
			m_pFunPtr = pFunPtr;
			m_pParameter = pParameter;
			pthread_create(&m_hThread, NULL, Thread::pThreadFun, this);
		}

		~Thread()
		{
			if (m_hThread)
			{
				pthread_join(m_hThread, NULL);
				m_hThread = NULL;
			}
		}

		void Run()
		{
			m_pFunPtr(m_pParameter);
		}

		static void* pThreadFun(void* pParameter)
		{
			Thread* pThread = (Thread*)pParameter;	
			pThread->Run();
		}


	public:
		pthread_t	m_hThread;

		ThreadMain	m_pFunPtr;
		void*		m_pParameter;
	};

}


#endif // _Thread_Android_h__

