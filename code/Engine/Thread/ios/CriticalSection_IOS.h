#ifndef _MyCs_IOS_H__
#define _MyCs_IOS_H__

class CriticalSection
{
public:
	CriticalSection::CriticalSection()
	{
		m_ptrNSLock = [NSLock new];
	}

	CriticalSection::~CriticalSection()
	{
		if(m_ptrNSLock != NULL)
		{
			[((NSLock*)m_ptrNSLock) release];
			m_ptrNSLock = NULL;
		}
	}

	void CriticalSection::Lock()
	{
		[((NSLock*)m_ptrNSLock) lock];
	}

	void CriticalSection::Unlock()
	{
		[((NSLock*)m_ptrNSLock) unlock];
	}

public:
	void* m_ptrNSLock;
};


#endif//_MyCs_IOS_H__
