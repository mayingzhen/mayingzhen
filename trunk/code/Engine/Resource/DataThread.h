#ifndef _DataThread_h__
#define _DataThread_h__

#include "../Thread/Thread.h"

namespace ma
{
	enum
	{
		DATA_THREAD_EXIGENCE_NORMAL = 0,
		DATA_THREAD_EXIGENCE_IMMEDIATE = 1,	
	};

	class ENGINE_API IDataObj
	{
	public:
		//virtual  int64_t GetID()	=	0;
		virtual  bool	LoadFileToMemeory() = 0;

		virtual	 bool	CreateFromMemeory() = 0;
		//virtual  DWORD	GetExigence()			=	0;
		//virtual void	DataThreadRelease()		=	0;
	};

    class Thread;
    class CMyEvent;

	class ENGINE_API DataThread : public Thread
	{	
	public:
		DataThread();

		~DataThread();

		void		Start();

		void		Stop();

		void		SetThreadPriority(ULONG	uData);

		IDataObj*	PopUpDataObj(void);	
		
		IDataObj*	PushBackDataObj(IDataObj* pObj);
		
		ULONG		Release(void){delete this; return 0;}
		
		bool		IsFree(void);
		
		bool		Process(void);
		
		void		SetImmediateExigence(bool bImmediate){m_bImmediate = bImmediate;}


	private:
		void		Run();

	public:
		typedef std::deque<IDataObj*> CDataObjQueue;
		CDataObjQueue	m_queLoaded;
		CDataObjQueue	m_queUnloaded;
		CDataObjQueue	m_queUnloadedBuffer;

	public:
		Thread*				m_pThread;
		CMyEvent*			m_pReadEvent; // read event, for process read data

		CriticalSection		m_csRequestQueue;	// 加载队列同步锁
		CriticalSection		m_csLoadedQueue;	// 加载队列同步锁

		bool				m_bImmediate;
		bool				m_bFree;
		bool				m_bExit;
		DWORD				m_dwTlsIndex;
		DWORD				m_dwThreadAppID;
	};
}



#endif 

