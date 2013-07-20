#ifndef _Thread_IOS_h__
#define _Thread_IOS_h__


namespace ma
{
	typedef void (*ThreadMain)(void* pThis);

	class Thread
	{	
	public:
		Thread(ThreadMain pFunPtr,void* pParameter);

		~Thread();

	public:
        void*   m_hThread;
        void*   m_pThreadFunction;
		//NSThread* m_hThread;
		//ObjCThreadFunction* m_pThreadFunction;
        
        ThreadMain  m_threadMainFun;
        void*       m_pParameter;
	};

}


#endif // _Thread_IOS_h__

