#ifndef _CMyEvent_IOS_H__
#define _CMyEvent_IOS_H__


namespace ma
{
	class CMyEvent
	{
	public:
		CMyEvent();

		~CMyEvent();

		void Wait();

		void Signal();

	private:
		void* m_ptrCondition;
	};
}

#endif



