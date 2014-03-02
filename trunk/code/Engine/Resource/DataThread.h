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

		virtual	void	Update();

		Resource*		PopUpDataObj();	
		
		void			PushBackDataObj(Resource* pObj);
		
		bool			IsFree(void);
		
		bool			Process(void);

	public:
		typedef std::deque<Resource*> DataObjQueue;
		DataObjQueue		m_queLoaded;
		DataObjQueue		m_queUnloaded;
		DataObjQueue		m_queUnloadedBuffer;

		Thread*				m_pThread;
		CMyEvent*			m_pReadEvent; // read event, for process read data

		CriticalSection		m_csRequestQueue;	// ���ض���ͬ����
		CriticalSection		m_csLoadedQueue;	// ���ض���ͬ����

		bool				m_bFree;
	};
}



#endif 

