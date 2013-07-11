#ifndef _Event_Android_h__
#define _Event_Android_h__


#include <pthread.h>

namespace ma
{
	class CMyEvent
	{
	public:
		CMyEvent()
		{
			pthread_mutex_init(&m_mutex, NULL);
			pthread_cond_init(&m_condition, NULL);
		}

		~CMyEvent()
		{
			pthread_cond_destroy(&m_condition);
			pthread_mutex_destroy(&m_mutex);
		}

		void Wait()
		{
			pthread_mutex_lock(&m_mutex);
			pthread_cond_wait(&m_condition, &m_mutex);
			pthread_mutex_unlock(&m_mutex);
		}

		void Signal()
		{
			pthread_mutex_lock(&m_mutex);
			pthread_cond_signal(&m_condition);
			pthread_mutex_unlock(&m_mutex);
		}

	public:
		pthread_mutex_t m_mutex;
		pthread_cond_t m_condition;
	};

}

#endif // _Event_Android_h__
