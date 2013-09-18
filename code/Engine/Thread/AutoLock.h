#ifndef _AutoLOCK_H__
#define _AutoLOCK_H__

#if PLATFORM_WIN == 1
#include "WinThread/WinCriticalSection.h"
#elif PLAFTORM_IOS == 1 || PLATFORM_ANDROID == 1
#include "pthread/PosixCriticalSection.h"
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