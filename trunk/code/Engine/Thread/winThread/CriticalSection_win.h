#ifndef _CriticalSection_Win_H__
#define _CriticalSection_Win_H__

namespace ma
{
	class CriticalSection
	{
	public:
		CriticalSection()	
		{
			::InitializeCriticalSection(&m_cs); 
		}

		~CriticalSection() 
		{
			::DeleteCriticalSection(&m_cs); 
		}

		void Lock()
		{
			::EnterCriticalSection(&m_cs); 
		}

		void Unlock()
		{
			::LeaveCriticalSection(&m_cs); 
		}

	private:
		CRITICAL_SECTION	m_cs;	
	};
}



#endif



