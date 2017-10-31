#include "JobScheduler.h"

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#include <mach/mach_host.h>
#endif

namespace ma
{

struct JobInfo
{
	JobScheduler::JobFunction func;
	void* userData;
	void* userData1;
	JobScheduler::ReturnCode* returnCode;
};

struct JobGroup
{
	enum
	{
		kSizeIncrement = 256,
		kTaskCountUnused = -1
	};
	JobGroup(const char* pSemaphoreName) : m_nTaskCount(kTaskCountUnused), m_nActiveThreads(0), m_nNextJob(0),
    m_nJobsAdded(0),m_doneSemaphore(pSemaphoreName) {}

	std::atomic<int> m_nTaskCount;
	std::atomic<int> m_nActiveThreads;
	std::atomic<int> m_nNextJob;
	std::atomic<int> m_nJobsAdded;

	std::vector<JobInfo> m_vecJobQueue;

	Semaphore m_doneSemaphore;
};

/// Worker thread managed by the work queue.
class WorkerThread
{
public:
    /// Construct.
    WorkerThread(JobScheduler* owner, unsigned index) 
    {
		m_pOwner = owner;
		m_uIndex=  index;
    }

	~WorkerThread()
	{
	}
    
    /// Process work items until stopped.
    void operator() () const
    {
		int activeGroup = -1;
		while( !m_pOwner->m_bShutDown )
		{
			JobInfo* job = m_pOwner->FetchNextJob(activeGroup);
			if( job )
			{
				m_pOwner->ProcessJob(*job, activeGroup);
			}
			else
			{
				--m_pOwner->m_ThreadsIdle;
				m_pOwner->m_AwakeSemaphore.WaitForSignal();
			}
		}

		return;
    }
    
    /// Return thread index.
    unsigned GetIndex() const { return m_uIndex; }
    
private:
    /// Work queue.
    JobScheduler* m_pOwner;
    /// Thread index.
    unsigned m_uIndex;
};

JobScheduler::JobScheduler() :
    m_bShutDown(false),
	m_ThreadsIdle(0),
	m_PriorityGroup(0),
    m_AwakeSemaphore("JobScheduler_AwakeSemaphore")
{
}

JobScheduler::~JobScheduler()
{
	ClearThreads();

    for (uint32 i = 0; i < m_Groups.size(); ++i)
    {
        SAFE_DELETE(m_Groups[i]);
    }
    m_Groups.clear();
}

void JobScheduler::ClearThreads()
{
	m_bShutDown = true;

	//UnityMemoryBarrier();

	for( uint32 i = 0; i < m_vecThread.size(); ++i )
	{
		m_AwakeSemaphore.Signal();
	}

	for (uint32 i = 0; i < m_vecThread.size(); ++i)
	{
		m_vecThread[i].join();
	}

	m_vecThread.clear();
}

void JobScheduler::CreateThreads(int numThreads,int maxGroups)
{
    m_Groups.resize(maxGroups);
    for (uint32 i = 0; i < m_Groups.size(); ++i)
    {
        string strSemName = string("GroupSem") + StringConverter::toString(i);
        m_Groups[i] = new JobGroup(strSemName.c_str());
    }

	if (numThreads <= 0)
		return;

    // Other subsystems may initialize themselves according to the number of threads.
    // Therefore allow creating the threads only once, after which the amount is fixed
	ASSERT(m_vecThread.empty());
    if (!m_vecThread.empty())
        return;
    
	m_bShutDown = false;
    for (int i = 0; i < numThreads; ++i)
    {
		WorkerThread worker(this, i + 1);
		m_vecThread.push_back(std::thread(worker));
    }

	LogInfo("Created %u worker thread%s", numThreads, numThreads > 1 ? "s" : "");
}


JobInfo* JobScheduler::FetchNextJob( int& activeGroup )
{
	if( activeGroup == m_PriorityGroup )
	{
		JobInfo* job = FetchJobInGroup(activeGroup);
		if( job )
			return job;
	}

	// We need a lock to change groups!
	std::lock_guard<std::mutex> autoLock(m_csQueueMutex);

	if( activeGroup != -1 )
		m_Groups[activeGroup]->m_nActiveThreads--;
	int group = m_PriorityGroup;
	for( uint32 i = 0; i < m_Groups.size(); i++ )
	{
		JobInfo* job = FetchJobInGroup(group);
		if( job )
		{
			m_Groups[group]->m_nActiveThreads++;
			// Set priority group to one that is actually available
			// Less work for other threads to find a good group
			m_PriorityGroup = group;
			activeGroup = group;
			return job;
		}
		if( ++group >= (int)m_Groups.size() )
			group = 0;
	}
	activeGroup = -1;
	return NULL;
}

JobInfo* JobScheduler::FetchJobInGroup( int group )
{
	JobGroup& jg = *m_Groups[group];
	int curJob = jg.m_nNextJob;
	while( curJob < jg.m_nJobsAdded )
	{
		if( std::atomic_compare_exchange_strong(&jg.m_nNextJob, &curJob, curJob + 1) )
			return &jg.m_vecJobQueue[curJob];

		curJob = jg.m_nNextJob;
	}
	return NULL;
}

void JobScheduler::ProcessJob( JobInfo& job, int group )
{
	ASSERT(job.func);
	void* ret = job.func(job.userData,job.userData1);
	if( job.returnCode )
	{
		//UnityMemoryBarrier();
		*job.returnCode = ret;
	}

	// Signal if we are the last to finish
	JobGroup& jg = *m_Groups[group];
	if (--jg.m_nTaskCount == 0)
	{
		jg.m_doneSemaphore.Signal();
	}
}

void JobScheduler::AwakeIdleWorkerThreads( int count )
{
	for( int i = 0; i < count && m_ThreadsIdle > 0; i++ )
	{
		if( --m_ThreadsIdle < 0 )
		{
			++m_ThreadsIdle;
			break;
		}
		m_AwakeSemaphore.Signal();
	}
}

JobScheduler::JobGroupID JobScheduler::BeginGroup( int maxJobs )
{
	// See if we can allocate a group without blocking.
	for( int isBlocking = 0; isBlocking < 2; ++isBlocking )
	{
		// If a group still has active threads we can't use it immediately after WaitForGroup().
		// By blocking we guarantee to find a group, as long as we stay within maxGroups.
		JobGroupID id = BeginGroupInternal(maxJobs, isBlocking != 0);
		if( id != -1 )
			return id;
	}
	LogError("JobScheduler: too many job groups");
	return -1;
}

JobScheduler::JobGroupID JobScheduler::BeginGroupInternal( int maxJobs, bool isBlocking )
{
	// Find unused group. We need a lock for that.
	m_csQueueMutex.lock();
	for( uint32 i = 0; i < m_Groups.size(); ++i )
	{
		JobGroup& group = *m_Groups[i];
		if( group.m_nTaskCount == JobGroup::kTaskCountUnused 
			&& ( isBlocking || group.m_nActiveThreads == 0 ) )
		{
			// We consider finishing group a pending task
			// Keeps job group alive until everything is done
			group.m_nTaskCount = 1;

			// Spin while worker threads are using our group
			// Do this *after* we've marked it used (case 492417)
			while( group.m_nActiveThreads != 0 )
			{
				m_csQueueMutex.unlock();
				m_csQueueMutex.lock();
			}
			group.m_nJobsAdded = 0;
			group.m_nNextJob = 0;
			const int rounding = JobGroup::kSizeIncrement;
			int roundedSize = (maxJobs + rounding - 1) / rounding * rounding;
			group.m_vecJobQueue.reserve(roundedSize);
			group.m_vecJobQueue.resize(maxJobs);
			m_csQueueMutex.unlock();
			return i;
		}
	}
	m_csQueueMutex.lock();
	return -1;
}

bool JobScheduler::IsGroupFinished( JobGroupID group )
{
	const JobGroup& jg = *m_Groups[group];
	// Last reference is kept until WaitForGroup()
	return jg.m_nTaskCount == 1;
}


void JobScheduler::WaitForGroup( JobGroupID group )
{
	if( group >= (JobGroupID)m_Groups.size() )
	{
		LogError( "Invalid job group ID" );
		return;
	}

	JobGroup& jg = *m_Groups[group];

	// Release our reference to job group
	// Pending jobs (if any) also have refs
	if( --jg.m_nTaskCount != 0 )
	{
		// Set our group as having priority so other threads fetch from it
		m_PriorityGroup = group;

		for( ;; )
		{
			JobInfo* job = FetchJobInGroup(group);
			if( !job )
				break;

			ProcessJob(*job, group);
		}

		jg.m_doneSemaphore.WaitForSignal();		
		ASSERT(jg.m_nNextJob == jg.m_nJobsAdded);
		ASSERT(jg.m_nTaskCount == 0);
	}

	// Set count to kTaskCountUnused (-1)
	--jg.m_nTaskCount;
}


bool JobScheduler::SubmitJob( JobGroupID group, JobFunction func, void *data, void* data1,ReturnCode *returnCode )
{
	ASSERT(func != NULL);
	if( m_vecThread.size() <= 0 )
	{
		// not multi-threaded: execute job right now
		void* z = func( data,data1 );
		if( returnCode )
			*returnCode = z;
		return true;
	}

	if( group >= (int)m_Groups.size() || group < 0)
	{
		LogError( "Invalid job group ID" );
		return false;
	}

	JobGroup& jg = *m_Groups[group];
	++jg.m_nTaskCount;
	int jobIndex = jg.m_nJobsAdded;
	JobInfo& job = jg.m_vecJobQueue[jobIndex];
	job.func = func;
	job.userData = data;
	job.userData1 = data1;
	job.returnCode = returnCode;
	int nextIndex = ++jg.m_nJobsAdded;
	// This may fail if you add jobs from multiple threads to the same group
	ASSERT(nextIndex == jobIndex + 1);
	AwakeIdleWorkerThreads(nextIndex - jg.m_nNextJob);
	return true;
}




JobScheduler* g_pJobScheduler = NULL;
JobScheduler* GetJobScheduler()
{
	return g_pJobScheduler;
}

}

