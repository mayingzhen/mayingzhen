#pragma once


namespace ma
{


class WorkerThread;
struct JobGroup;
struct JobInfo;


/// Work queue subsystem for multithreading.
class JobScheduler 
{
    friend class WorkerThread;
    
public:

	typedef int JobGroupID;

    JobScheduler();

    ~JobScheduler();
 
    /// Create worker threads. Can only be called once.
    void		CreateThreads(int numThreads,int maxGroups);
	UINT	    GetNumThreads() const { return (UINT)m_vecThread.size(); }
	void		ClearThreads();

	JobGroupID	BeginGroup( int maxJobs ); 
	bool		IsGroupFinished( JobGroupID group );
	void		WaitForGroup( JobGroupID group );
	
	bool		SubmitJob(JobGroupID group, const std::function<void()>& func);
 
private:
	JobGroupID	BeginGroupInternal( int maxJobs, bool isBlocking ); 
	JobInfo*	FetchNextJob( int& threadActiveGroup );
	JobInfo*	FetchJobInGroup( int group );
	void		ProcessJob( JobInfo& job, int group );
	void		AwakeIdleWorkerThreads( int count );
    
private: 
    /// Worker threads.
    std::vector< std::thread > m_vecThread;

	std::vector<JobGroup*> m_Groups;

	/// Worker queue mutex.
	std::mutex	m_csQueueMutex;
    
	/// Shutting down flag.
	std::atomic<bool> m_bShutDown;
    
	std::atomic<int>  m_ThreadsIdle;
	std::atomic<int> m_PriorityGroup;

	Semaphore m_AwakeSemaphore;
};


extern JobScheduler* g_pJobScheduler;
JobScheduler* GetJobScheduler();
}
