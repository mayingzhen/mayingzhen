#include "Thread_IOS.h"

#include <Foundation/Foundation.h>

@interface ObjCThreadFunction : NSObject
{
    class ma::Thread* m_pTheead;
}

-(void)ExecuteThread:(id)param;
-(void)SetThread:(ma::Thread*)pThread;
@end

@implementation ObjCThreadFunction
-(void)ExecuteThread:(id)pThis
{
    m_pTheead->m_threadMainFun(m_pTheead->m_pParameter);
}

-(void)SetThread:(ma::Thread*)pThread;
{
    m_pTheead = pThread;
}
@end

namespace ma
{


    Thread::Thread(ThreadMain pFunPtr,void* pParameter)
    {
        ObjCThreadFunction* pThreadFunction = [ObjCThreadFunction new];
        [pThreadFunction SetThread:this];
        m_pThreadFunction = pThreadFunction;
        m_pParameter = pParameter;

        //m_hThread = [[NSThread alloc] initWithTarget:pThreadFunction selector:@selector(ExecuteThread:) object:nil];
        //[(NSThread*)m_hThread start];
    }

    Thread::~Thread()
    {
        while([(NSThread*)m_hThread isExecuting])
        {
            [NSThread sleepForTimeInterval:(0.1)];
        }

        [(NSThread*)m_hThread release];
        m_hThread = nil;

        if(m_pThreadFunction)
        {
            [(ObjCThreadFunction*)m_pThreadFunction release];
            m_pThreadFunction = nil;
        }
    }
}



