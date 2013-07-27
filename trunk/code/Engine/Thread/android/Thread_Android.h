#ifndef _Thread_Android_h__
#define _Thread_Android_h__


#include <pthread.h>


namespace ma
{
	typedef void* (*ThreadMain)(void* pParameter);

	class Thread
	{	
	public:
		Thread(ThreadMain pFunPtr,void* pParameter)
		{
			pthread_create(&m_hThread, NULL, pFunPtr, pParameter);
		}

		~Thread()
		{
			if (m_hThread)
			{
				pthread_join(m_hThread, NULL);
				m_hThread = NULL;
			}
		}


	public:
		pthread_t m_hThread;
	};

}


#endif // _Thread_Android_h__

