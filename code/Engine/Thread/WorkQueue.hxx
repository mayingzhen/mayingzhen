#include "WorkQueue.h"
#include "libcpuid.h"

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#include <mach/mach_host.h>
#endif

namespace ma
{

struct JobInfo
{
	WorkQueue::JobFunction func;
	void* userData;
	void* userData1;
	WorkQueue::ReturnCode* returnCode;
};

struct JobGroup
{
	enum
	{
		kSizeIncrement = 256,
		kTaskCountUnused = -1
	};
	JobGroup() : taskCount(kTaskCountUnused), activeThreads(0), nextJob(0), jobsAdded(0) {}

	volatile int taskCount;
	volatile int activeThreads;
	volatile int nextJob;
	volatile int jobsAdded;
	vector<JobInfo> jobQueue;
	Semaphore doneSemaphore;
};

/// Worker thread managed by the work queue.
class WorkerThread : public Thread, public Referenced
{
public:
    /// Construct.
    WorkerThread(WorkQueue* owner, unsigned index) 
		: Thread("WorkQueueThread")
    {
		m_pOwner = owner;
		m_uIndex=  index;
    }

	~WorkerThread()
	{
	}
    
    /// Process work items until stopped.
    virtual void ThreadLoop()
    {
		int activeGroup = -1;
		while( !m_bExit && !m_pOwner->m_bShutDown )
		{
			JobInfo* job = m_pOwner->FetchNextJob(activeGroup);
			if( job )
			{
				m_pOwner->ProcessJob(*job, activeGroup);
			}
			else
			{
				AtomicIncrement(&m_pOwner->m_ThreadsIdle);
				m_pOwner->m_AwakeSemaphore.WaitForSignal();
			}
		}

		return;
    }
    
    /// Return thread index.
    unsigned GetIndex() const { return m_uIndex; }
    
private:
    /// Work queue.
    WorkQueue* m_pOwner;
    /// Thread index.
    unsigned m_uIndex;
};

WorkQueue::WorkQueue() :
    m_bShutDown(false),
	m_ThreadsIdle(0),
	m_PriorityGroup(0)
{
	m_Groups = NULL;
	m_GroupCount = 0;
}

WorkQueue::~WorkQueue()
{
	ClearThreads();

	SAFE_DELETE_ARRAY(m_Groups);
}

void WorkQueue::ClearThreads()
{
	m_bShutDown = true;

	UnityMemoryBarrier();

	for( uint32 i = 0; i < m_vecThread.size(); ++i )
	{
		m_AwakeSemaphore.Signal();
	}

	for (uint32 i = 0; i < m_vecThread.size(); ++i)
	{
		m_vecThread[i]->Stop();
	}

	m_vecThread.clear();
}

void WorkQueue::CreateThreads(int numThreads,int maxGroups)
{
	m_Groups = new JobGroup[maxGroups];
	m_GroupCount = maxGroups;

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
        RefPtr<WorkerThread> pThread(new WorkerThread(this, i + 1));
        pThread->Start();
        m_vecThread.push_back(pThread);
    }

	LogInfo("Created %u worker thread%s", numThreads, numThreads > 1 ? "s" : "");
}


JobInfo* WorkQueue::FetchNextJob( int& activeGroup )
{
	if( activeGroup == m_PriorityGroup )
	{
		JobInfo* job = FetchJobInGroup(activeGroup);
		if( job )
			return job;
	}

	// We need a lock to change groups!
	AutoLock autoLock(m_csQueueMutex);

	if( activeGroup != -1 )
		m_Groups[activeGroup].activeThreads--;
	int group = m_PriorityGroup;
	for( int i = 0; i < m_GroupCount; i++ )
	{
		JobInfo* job = FetchJobInGroup(group);
		if( job )
		{
			m_Groups[group].activeThreads++;
			// Set priority group to one that is actually available
			// Less work for other threads to find a good group
			m_PriorityGroup = group;
			activeGroup = group;
			return job;
		}
		if( ++group >= m_GroupCount )
			group = 0;
	}
	activeGroup = -1;
	return NULL;
}

JobInfo* WorkQueue::FetchJobInGroup( int group )
{
	JobGroup& jg = m_Groups[group];
	int curJob = jg.nextJob;
	while( curJob < jg.jobsAdded )
	{
		if( AtomicCompareExchange(&jg.nextJob, curJob + 1, curJob) )
			return &jg.jobQueue[curJob];

		curJob = jg.nextJob;
	}
	return NULL;
}

void WorkQueue::ProcessJob( JobInfo& job, int group )
{
	ASSERT(job.func);
	void* ret = job.func(job.userData,job.userData1);
	if( job.returnCode )
	{
		UnityMemoryBarrier();
		*job.returnCode = ret;
	}

	// Signal if we are the last to finish
	JobGroup& jg = m_Groups[group];
	if( AtomicDecrement(&jg.taskCount) == 0 )
		jg.doneSemaphore.Signal();
}

void WorkQueue::AwakeIdleWorkerThreads( int count )
{
	for( int i = 0; i < count && m_ThreadsIdle > 0; i++ )
	{
		if( AtomicDecrement(&m_ThreadsIdle) < 0 )
		{
			AtomicIncrement(&m_ThreadsIdle);
			break;
		}
		m_AwakeSemaphore.Signal();
	}
}

WorkQueue::JobGroupID WorkQueue::BeginGroup( int maxJobs )
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

WorkQueue::JobGroupID WorkQueue::BeginGroupInternal( int maxJobs, bool isBlocking )
{
	// Find unused group. We need a lock for that.
	m_csQueueMutex.Lock();
	for( int i = 0; i < m_GroupCount; ++i )
	{
		JobGroup& group = m_Groups[i];
		if( group.taskCount == JobGroup::kTaskCountUnused 
			&& ( isBlocking || group.activeThreads == 0 ) )
		{
			// We consider finishing group a pending task
			// Keeps job group alive until everything is done
			group.taskCount = 1;

			// Spin while worker threads are using our group
			// Do this *after* we've marked it used (case 492417)
			while( group.activeThreads != 0 )
			{
				m_csQueueMutex.Unlock();
				m_csQueueMutex.Lock();
			}
			group.jobsAdded = 0;
			group.nextJob = 0;
			const int rounding = JobGroup::kSizeIncrement;
			int roundedSize = (maxJobs + rounding - 1) / rounding * rounding;
			group.jobQueue.reserve(roundedSize);
			group.jobQueue.resize(maxJobs);
			m_csQueueMutex.Unlock();
			return i;
		}
	}
	m_csQueueMutex.Unlock();
	return -1;
}

bool WorkQueue::IsGroupFinished( JobGroupID group )
{
	const JobGroup& jg = m_Groups[group];
	// Last reference is kept until WaitForGroup()
	return jg.taskCount == 1;
}


void WorkQueue::WaitForGroup( JobGroupID group )
{
	if( group >= m_GroupCount )
	{
		LogError( "Invalid job group ID" );
		return;
	}

	JobGroup& jg = m_Groups[group];

	// Release our reference to job group
	// Pending jobs (if any) also have refs
	if( AtomicDecrement(&jg.taskCount) != 0 )
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

		jg.doneSemaphore.WaitForSignal();		
		ASSERT(jg.nextJob == jg.jobsAdded);
		ASSERT(jg.taskCount == 0);
	}

	// Set count to kTaskCountUnused (-1)
	AtomicDecrement(&jg.taskCount);
}


bool WorkQueue::SubmitJob( JobGroupID group, JobFunction func, void *data, void* data1,ReturnCode *returnCode )
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

	if( group >= m_GroupCount || group < 0)
	{
		LogError( "Invalid job group ID" );
		return false;
	}

	JobGroup& jg = m_Groups[group];
	AtomicIncrement(&jg.taskCount);
	int jobIndex = jg.jobsAdded;
	JobInfo& job = jg.jobQueue[jobIndex];
	job.func = func;
	job.userData = data;
	job.userData1 = data1;
	job.returnCode = returnCode;
	int nextIndex = AtomicIncrement(&jg.jobsAdded);
	// This may fail if you add jobs from multiple threads to the same group
	ASSERT(nextIndex == jobIndex + 1);
	AwakeIdleWorkerThreads(nextIndex - jg.nextJob);
	return true;
}




WorkQueue* g_pWorkQueue = NULL;
WorkQueue* GetWorkQueue()
{
	return g_pWorkQueue;
}

#if defined(__ANDROID__) || defined(RPI)
static uint32 GetArmCPUCount()
{
	FILE* fp;
	int res, i = -1, j = -1;

	fp = fopen("/sys/devices/system/cpu/present", "r");
	// If failed, return at least 1
	if (fp == 0)
		return 1;

	res = fscanf(fp, "%d-%d", &i, &j);
	fclose(fp);

	if (res == 1 && i == 0)
		return 1;
	else if (res == 2 && i == 0)
		return j + 1;

	// If failed, return at least 1
	return 1;
}
#endif

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
static void GetCPUData(host_basic_info_data_t* data)
{
	mach_msg_type_number_t infoCount;
	infoCount = HOST_BASIC_INFO_COUNT;
	host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t)data, &infoCount);
}
#elif !defined(__ANDROID__) && !defined(RPI) && !defined(EMSCRIPTEN)
static void GetCPUData(struct cpu_id_t* data)
{
	if (cpu_identify(0, data) < 0)
	{
		data->num_logical_cpus = 1;
		data->num_cores = 1;
	}
}
#endif

/// Return the number of physical CPU cores.
uint32 GetNumPhysicalCPUs()
{
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
	host_basic_info_data_t data;
	GetCPUData(&data);
#if defined(TARGET_IPHONE_SIMULATOR)
	// Hardcoded to dual-core on simulator mode even if the host has more
	return min(2, data.physical_cpu);
#else
	return data.physical_cpu;
#endif
#elif defined(__ANDROID__) || defined(RPI)
	return GetArmCPUCount();
#elif !defined(EMSCRIPTEN)
	struct cpu_id_t data;
	GetCPUData(&data);
	return data.num_cores;
#else
	/// \todo Implement properly
	return 1;
#endif
}

/// Return the number of logical CPUs (different from physical if hyperthreading is used.)
uint32 GetNumLogicalCPUs()
{
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
	host_basic_info_data_t data;
	GetCPUData(&data);
#if defined(TARGET_IPHONE_SIMULATOR)
	return min(2, data.logical_cpu);
#else
	return data.logical_cpu;
#endif
#elif defined(__ANDROID__) || defined (RPI)
	return GetArmCPUCount();
#elif !defined(EMSCRIPTEN)
	struct cpu_id_t data;
	GetCPUData(&data);
	return data.num_logical_cpus;
#else
	/// \todo Implement properly
	return 1;
#endif
}

}
