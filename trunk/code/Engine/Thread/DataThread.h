#ifndef _DataThread_h__
#define _DataThread_h__


#if PLATFORM_WIN == 1
#include "Win/CriticalSection_win.h"
#include "Win/Event_win.h"
#include "Win/Thread_win.h"
#elif PLAFTORM_IOS == 1
#include "IOS/CriticalSection_ios.h"
#include "IOS/Event_IOS.h"
#include "IOS/Thread_IOS.h"
#elif PLATFORM_ANDROID == 1
#include "Android/CriticalSection_Android.h"
#include "Android/Event_Android.h"
#include "Android/Thread_Android.h"
#endif

namespace ma
{
	class Thread;
	class CMyEvent;


	enum
	{
		DATA_THREAD_EXIGENCE_NORMAL = 0,
		DATA_THREAD_EXIGENCE_IMMEDIATE = 1,	
	};

	class ENGINE_API IDataObj
	{
	public:
		//virtual  int64_t GetID()	=	0;
		virtual  bool	LoadToMemory() = 0;

		virtual	 bool	LoadImp() = 0;
		//virtual  DWORD	GetExigence()			=	0;
		//virtual void	DataThreadRelease()		=	0;
	};


	class ENGINE_API DataThread 
	{	
	public:
		DataThread(DWORD dwTlsIndex, DWORD dwThreadAppID);

		~DataThread();

		void		SetThreadPriority(ULONG	uData);

		IDataObj*	PopUpDataObj(void);	
		
		IDataObj*	PushBackDataObj(IDataObj* pObj);
		
		ULONG		Release(void){delete this; return 0;}
		
		bool		IsFree(void);
		
		bool		Process(void);
		
		void		SetImmediateExigence(bool bImmediate){m_bImmediate = bImmediate;}
		

	public:
		static	void MyDataThread(void* pThis);

	private:
		void		ProcessLoadRequest();

	public:
		typedef std::deque<IDataObj*> CDataObjQueue;
		CDataObjQueue	m_queLoaded;
		CDataObjQueue	m_queUnloaded;
		CDataObjQueue	m_queUnloadedBuffer;

	public:
		Thread*				m_pThread;
		CMyEvent*			m_pReadEvent; // read event, for process read data

		CriticalSection	m_csRequestQueue;	// 加载队列同步锁
		CriticalSection	m_csLoadedQueue;	// 加载队列同步锁

		bool			m_bImmediate;
		bool			m_bFree;
		bool			m_bExit;
		DWORD			m_dwTlsIndex;
		DWORD			m_dwThreadAppID;
	};

	ENGINE_API DataThread*	GetDataThread();
	
	ENGINE_API void			SetDataThread(DataThread* pDataThread);
}



#endif 

