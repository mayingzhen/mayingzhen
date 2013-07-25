#include "CriticalSection_IOS.h"

#include <Foundation/Foundation.h>


namespace  ma
{
    
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

}



