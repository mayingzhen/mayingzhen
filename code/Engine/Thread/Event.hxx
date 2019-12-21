#include "Event.h"


#if defined(__ANDROID__) || defined(__APPLE__) 

#include <time.h>
#include <sys/time.h>
#include <errno.h>

namespace ma
{
	Event::Event()
		: m_signaled(false)
	{
		pthread_mutex_init(&m_mutex, NULL);
		pthread_cond_init(&m_cond, NULL);
	}

	Event::~Event()
	{
		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond);
	}

	bool Event::Wait()
	{
		pthread_mutex_lock(&m_mutex);
		while (!m_signaled)
		{
			pthread_cond_wait(&m_cond, &m_mutex);
		}
		// auto-reset
		m_signaled = false;
		pthread_mutex_unlock(&m_mutex);
		return true;
	}

	EventResult Event::Wait(unsigned long millsecond)
	{
		if (millsecond == INFINITE)
		{
			return Wait() ? Succeed : Fail;
		}

		timespec ts;
#if !defined(__ANDROID__) || (defined(__ANDROID_API__) && __ANDROID_API__ < 21)
		ts.tv_sec = millsecond / 1000;
		ts.tv_nsec = (millsecond % 1000) * 1000 * 1000;
#endif

		pthread_mutex_lock(&m_mutex);
		int ret = 0;
		while (!m_signaled && ret != ETIMEDOUT)
		{
#if defined(__ANDROID__)

#if defined(__ANDROID_API__) && __ANDROID_API__ >= 21
			clock_gettime(CLOCK_MONOTONIC, &ts);
			ts.tv_sec += millsecond / 1000;
			ts.tv_nsec += (millsecond % 1000) * 1000 * 1000;
			ts.tv_sec += ts.tv_nsec / 1000000000;
			ts.tv_nsec = ts.tv_nsec % 1000000000;
			ret = pthread_cond_timedwait(&m_cond, &m_mutex, &ts);
#else//#if defined(__ANDROID_API__) && __ANDROID_API__ >= 21
			ret = pthread_cond_timedwait_relative_np(&m_cond, &m_mutex, &ts);
#endif//#if defined(__ANDROID_API__) && __ANDROID_API__ >= 21

			ret = pthread_cond_timedwait_relative_np(&m_cond, &m_mutex, &ts);
#endif
		}
		// auto-reset
		m_signaled = false;
		pthread_mutex_unlock(&m_mutex);

		if (ret == 0) return Succeed;

		if (ret == ETIMEDOUT) return Timeout;

		return Fail;
	}

	bool Event::Signal()
	{
		pthread_mutex_lock(&m_mutex);
		m_signaled = true;
		pthread_mutex_unlock(&m_mutex);
		pthread_cond_signal(&m_cond);
		return true;
	}

}
#endif

#ifdef WIN32
namespace ma
{
	Event::Event()
	{
		m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	Event::~Event()
	{
		CloseHandle(m_event);
	}

	bool Event::Wait()
	{
		DWORD ret = ::WaitForSingleObject(m_event, INFINITE);
		switch (ret)
		{
		case WAIT_OBJECT_0:
			return true;
		default:
			return false;
		}
	}

	EventResult Event::Wait(unsigned long millsecond)
	{
		DWORD ret = ::WaitForSingleObject(m_event, millsecond);
		switch (ret)
		{
		case WAIT_OBJECT_0:
			return Succeed;
		case WAIT_TIMEOUT:
			return Timeout;
		default:
			return Fail;
		}
	}

	bool Event::Signal()
	{
		SetEvent(m_event);
		return true;
	}

}
#endif

