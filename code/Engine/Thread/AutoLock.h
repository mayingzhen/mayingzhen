#ifndef _AutoLOCK_H__
#define _AutoLOCK_H__

#if PLATFORM_WIN == 1
#include "Win/CriticalSection_win.h"
#elif PLAFTORM_IOS == 1
#include "IOS/CriticalSection_IOS.h"
#elif PLATFORM_ANDROID == 1
#include "Android/CriticalSection_Android.h"
#endif

namespace ma
{
	class AutoLock
	{
	public:
		AutoLock(CriticalSection& cs) : m_cs(cs) 
		{
			m_cs.Lock(); 
		}

		~AutoLock() 
		{
			m_cs.Unlock(); 
		}

	private:
		CriticalSection& m_cs;
	};
}


#endif//_AutoLOCK_H__