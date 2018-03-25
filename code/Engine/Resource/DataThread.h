#ifndef _DataThread_h__
#define _DataThread_h__


namespace ma
{
	class Resource;

	class DataThread 
	{	
	public:
		DataThread();

		~DataThread();

		virtual void	Start();

		virtual void	Stop();

		void			Process();
		
		void			PushBackDataObj(Resource* pRes);

	private:
		void			ThreadLoop();

	public:
		typedef std::deque< RefPtr<Resource> > DataObjQueue;
		DataObjQueue		m_queUnloaded;
		DataObjQueue		m_queLoaded;
	
		std::condition_variable	m_readEvent;

		std::mutex	m_csRequestQueue;
		std::mutex	m_csLoadedQueue;

		std::thread			m_thread;

		bool				m_bExit = false;
	};

	extern DataThread* g_pDataThread;
	DataThread* GetDataThread();
}



#endif 

