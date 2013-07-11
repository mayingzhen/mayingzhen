
#include <Foundation/Foundation.h>

namespace ma
{
	class CMyEvent
	{
	public:
		CMyEvent()
		{
			m_ptrCondition = [NSCondition new];
		}

		~CMyEvent()
		{
			if(m_ptrCondition)
			{
				[m_ptrCondition lock];
				[m_ptrCondition release];
				[m_ptrCondition unlock];
				m_ptrCondition = nil;;
			}
		}

		void Wait()
		{
			[m_ptrCondition lock];
			[m_ptrCondition wait];
			[m_ptrCondition unlock];
		}

		void Signal()
		{
			[m_ptrCondition lock];
			[m_ptrCondition signal];
			[m_ptrCondition unlock];
		}

	public:
		NSCondition* m_ptrCondition;
	};
}



