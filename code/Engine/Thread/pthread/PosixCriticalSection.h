#ifndef _PosixCriticalSection_H__
#define _PosixCriticalSection_H__

#include "pthread.h"

namespace ma
{
	class CriticalSection
	{

	public:
		CriticalSection()
		{
			pthread_mutex_t* pMutex = new pthread_mutex_t;
			pthread_mutex_init(pMutex, NULL);
			m_ptrNSLock = pMutex;
		}

		~CriticalSection()
		{
			pthread_mutex_t* pMutex = (pthread_mutex_t*)m_ptrNSLock;
			pthread_mutex_destroy(pMutex);
			delete pMutex;
			m_ptrNSLock = NULL;
		}

		void Lock()
		{
			pthread_mutex_lock((pthread_mutex_t*)m_ptrNSLock);
		}

		void Unlock()
		{
			pthread_mutex_unlock((pthread_mutex_t*)m_ptrNSLock);
		}

	public:
		void* m_ptrNSLock;
	};

}

#endif // _PosixCriticalSection_H__




