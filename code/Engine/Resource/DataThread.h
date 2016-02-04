#ifndef _DataThread_h__
#define _DataThread_h__

#include "../Thread/Thread.h"

namespace ma
{
	class Thread;
	class CMyEvent;
	class Resource;

	class DataThread : public Thread
	{	
	public:
		DataThread();

		~DataThread();

		virtual void	Stop();

		virtual	void	ThreadLoop();

		void			Process();
		
		void			PushBackDataObj(Resource* pRes);

	public:
		typedef std::deque< RefPtr<Resource> > DataObjQueue;
		DataObjQueue		m_queUnloaded;
		DataObjQueue		m_queLoaded;
		
		CMyEvent			m_readEvent;

		CriticalSection		m_csRequestQueue;
		CriticalSection		m_csLoadedQueue;
	};

	extern DataThread* g_pDataThread;
	DataThread* GetDataThread();
}



#endif 

