#ifndef _Event_Win_h__
#define _Event_Win_h__


#include <Windows.h>

namespace ma
{
	class CMyEvent
	{
	public:
		CMyEvent()
		{
			m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

		}

		~CMyEvent()
		{
			::CloseHandle(m_hEvent);
			m_hEvent = NULL;
		}

		void Wait()
		{
			::WaitForSingleObject(m_hEvent, INFINITE);
		}

		inline void Signal()
		{
			::SetEvent(m_hEvent);
		}

	public:
		HANDLE	m_hEvent;	
	};

}

#endif // _Event_Android_h__
