#ifndef _PosixThread_h__
#define _PosixThread_h__


#include <pthread.h>


namespace ma
{
	class Thread
	{	
	public:
		Thread()
		{
			pthread_create(&m_hThread, NULL, ThreadFun, this);
		}

		~Thread()
		{
			if (m_hThread)
			{
				pthread_join(m_hThread, NULL);
				m_hThread = NULL;
			}
		}

		virtual void Run() = 0;

		void* ThreadFun(void* pParameter)
		{
			Thread* pThread = (Thread*)pParameter;
			pThread->Run();
		}


	public:
		pthread_t	m_hThread;
	};

}


#endif // _PosixThread_h__

