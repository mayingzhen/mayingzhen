#ifndef _PosixCriticalSection_H__
#define _PosixCriticalSection_H__

#include <pthread.h>

namespace ma
{
	class CriticalSection
	{

	public:
		CriticalSection()
		{
			pthread_mutex_init(&m_mutex, NULL);
		}

		~CriticalSection()
		{
			pthread_mutex_destroy(&m_mutex);
		}

		void Lock()
		{
			pthread_mutex_lock(&m_mutex);
		}

		void Unlock()
		{
			pthread_mutex_unlock(&m_mutex);
		}

		bool TryLock()
		{
			//return ::TryEnterCriticalSection(&m_cs);
		}

	public:
		pthread_mutex_t m_mutex;
	};

}

#endif // _PosixCriticalSection_H__




