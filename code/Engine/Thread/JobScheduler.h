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

	typedef void* (*JobFunction)(void*,void*);
	typedef void* volatile ReturnCode;
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
	
	bool		SubmitJob( JobGroupID group, JobFunction func, void *data, void* data1,ReturnCode *returnCode );
 
private:
	JobGroupID	BeginGroupInternal( int maxJobs, bool isBlocking ); 
	JobInfo*	FetchNextJob( int& threadActiveGroup );
	JobInfo*	FetchJobInGroup( int group );
	void		ProcessJob( JobInfo& job, int group );
	void		AwakeIdleWorkerThreads( int count );
    
private: 
    /// Worker threads.
    vector< RefPtr<WorkerThread> > m_vecThread;

	vector<JobGroup*> m_Groups;

	/// Worker queue mutex.
	std::mutex	m_csQueueMutex;
    
	/// Shutting down flag.
    volatile bool m_bShutDown;
    
	volatile int m_ThreadsIdle;
	volatile int m_PriorityGroup;

	Semaphore m_AwakeSemaphore;
};


extern JobScheduler* g_pJobScheduler;
JobScheduler* GetJobScheduler();
}
