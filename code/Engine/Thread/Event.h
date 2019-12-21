#pragma once


#if defined(__ANDROID__) || defined(__APPLE__)
#include <pthread.h>
#define INFINITE 0xFFFFFFFF
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace ma
{
	enum EventResult
	{
		Fail = -1,
		Succeed = 0,
		Timeout = 1,
	};

	class Event
	{
	public:

		Event();
		~Event();

		bool Wait();

		//return 0 if wait complete, return 1 if timeout, return -1 if error
		EventResult Wait(unsigned long millsecond);

		bool Signal();

		template<bool NEED_THREAD_SAFE = true>
		void Reset();

	private:
		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;

#if defined(__ANDROID__) || defined(__APPLE__)
		pthread_cond_t m_cond;
		pthread_mutex_t m_mutex;
		bool m_signaled;
#endif

#ifdef WIN32
		HANDLE m_event;
#endif
	};

	template<bool NEED_THREAD_SAFE>
	void Event::Reset()
	{
#if defined(__ANDROID__) || defined(__APPLE__)
		if (NEED_THREAD_SAFE)
		{
			pthread_mutex_lock(&m_mutex);
			m_signaled = false;
			pthread_mutex_unlock(&m_mutex);
		}
		else
		{
			m_signaled = false;
		}
#elif defined(WIN32)
		::ResetEvent(m_event);
#endif
	}

}
