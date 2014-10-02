#ifndef _DataThread_h__
#define _DataThread_h__

#include "../Thread/Thread.h"

namespace ma
{
	class Thread;
	class CMyEvent;
	class Resource;

	struct ResData
	{
		RefPtr<Resource>		m_pRes;

		RefPtr<EventListener>	m_pCallBack;
	};

	class DataThread : public Thread
	{	
	public:
		DataThread();

		~DataThread();

		virtual	void	ThreadUpdate();
		
		void			PushBackDataObj(const ResData& pObj);
		
		bool			IsFree(void);
		
		bool			Process(void);

	public:
		typedef std::deque< ResData > DataObjQueue;
		DataObjQueue		m_queLoaded;
		DataObjQueue		m_queUnloaded;
		DataObjQueue		m_queUnloadedBuffer;

		Thread*				m_pThread;
		CMyEvent*			m_pReadEvent; // read event, for process read data

		CriticalSection		m_csRequestQueue;	// 加载队列同步锁
		CriticalSection		m_csLoadedQueue;	// 加载队列同步锁

		bool				m_bFree;
	};
}



#endif 

