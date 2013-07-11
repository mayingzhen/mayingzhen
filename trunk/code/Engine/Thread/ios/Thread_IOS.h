#ifndef _Thread_h__
#define _Thread_h__


#include <Foundation/Foundation.h>

// 包装objc线程执行函数
@interface ObjCThreadFunction : NSObject
{
	void*	m_pFunPtr;
	void*	m_pParameter;
}
-(void)ExecuteThread:(id)param;
-(void)SetFunPtr:(void*)pFunPtr;
-(void)SetParameter:(void*)pParameter;
@end

@implementation ObjCThreadFunction
-(void)ExecuteThread:(id)pThis
{
	//NSThread* ptrCurThread = [NSThread currentThread];
	//[[ptrCurThread threadDictionary] setValue:[NSNumber numberWithUnsignedInt:(m_pDataThread->m_dwThreadAppID)] forKey:@"ResAppID"];

	m_pFunPtr(m_pParameter);
}

-(void)SetFunPtr:(void*)pFunPtr
{
	m_pFunPtr = pFunPtr;
}

-(void)SetParameter:(void*)pParameter;
{
	m_pParameter = pParameter;
}

@end

namespace ma
{

	typedef void* (*ThreadMain)(void* pThis);

	class Thread
	{	
	public:
		Thread(ThreadMain pFunPtr,void* pParameter)
		{
			m_pThreadFunction = [ObjCThreadFunction new];
			[m_pThreadFunction SetFunPtr:pFunPtr];
			[m_pThreadFunction SetParameter:pParameter];

			m_hThread = [[NSThread alloc] initWithTarget:m_pThreadFunction selector:@selector(ExecuteThread:) object:nil];
			[m_hThread start];
		}

		~Thread()
		{
			while([m_hThread isExecuting])
			{	
				[NSThread sleepForTimeInterval:(0.1)];
			}

			[m_hThread release];
			m_hThread = nil;

			if(m_pThreadFunction)
			{
				[m_pThreadFunction release];
				m_pThreadFunction = nil;
			}
		}


	public:
		NSThread* m_hThread;
		ObjCThreadFunction* m_pThreadFunction;
	};

}


#endif // _Thread_h__

