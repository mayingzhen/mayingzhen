#include "Event_IOS.h"

#include <Foundation/Foundation.h>

namespace ma
{
    CMyEvent::CMyEvent()
    {
        m_ptrCondition = [NSCondition new];
    }

    CMyEvent::~CMyEvent()
    {
        if(m_ptrCondition)
        {
            [(NSCondition*)m_ptrCondition lock];
            [(NSCondition*)m_ptrCondition release];
            [(NSCondition*)m_ptrCondition unlock];
            m_ptrCondition = nil;
        }
    }

    void CMyEvent::Wait()
    {
        [(NSCondition*)m_ptrCondition lock];
        [(NSCondition*)m_ptrCondition wait];
        [(NSCondition*)m_ptrCondition unlock];
    }

    void CMyEvent::Signal()
    {
        [(NSCondition*)m_ptrCondition lock];
        [(NSCondition*)m_ptrCondition signal];
        [(NSCondition*)m_ptrCondition unlock];
    }
}



