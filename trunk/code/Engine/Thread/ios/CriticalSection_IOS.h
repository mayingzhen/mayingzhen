#ifndef _CriticalSection_IOS_H__
#define _CriticalSection_IOS_H__


namespace  ma
{
	class CriticalSection
	{
	public:
		CriticalSection();

		~CriticalSection();

		void Lock();

		void Unlock();

	public:
		void* m_ptrNSLock;
	};
}


#endif//_CriticalSection_IOS_H__

