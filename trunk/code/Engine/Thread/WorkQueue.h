#ifndef _WorkQueue_h__
#define _WorkQueue_h__




namespace ma
{

	/// Work item completed event.
// 	EVENT(E_WORKITEMCOMPLETED, WorkItemCompleted)
// 	{
// 		PARAM(P_ITEM, Item);                        // WorkItem ptr
// 	}
	

	class WorkerThread;

	/// Work queue item.
	struct WorkItem : public Referenced
	{
		friend class WorkQueue;

		// Construct
		WorkItem() :
        	priority_(0),
			sendEvent_(false),
        	completed_(false),
        	pooled_(false)
		{
		}
	    
		/// Work function. Called with the work item and thread index (0 = main thread) as parameters.
		void (*workFunction_)(const WorkItem*, UINT);
		/// Data start pointer.
		void* start_;
		/// Data end pointer.
		void* end_;
		/// Auxiliary data pointer.
		void* aux_;
		/// Priority. Higher value = will be completed first.
		UINT priority_;
		/// Whether to send event on completion.
		bool sendEvent_;
		/// Completed flag.
		volatile bool completed_;

	private:
		bool pooled_;
	};

	/// Work queue subsystem for multithreading.
	class ENGINE_API WorkQueue /*: public Object*/
	{
		//OBJECT(WorkQueue);
	    
		friend class WorkerThread;
	    
	public:
		/// Construct.
		WorkQueue(/*Context* context*/);
		/// Destruct.
		~WorkQueue();
	    
		/// Create worker threads. Can only be called once.
		void CreateThreads(UINT numThreads);
		/// Get pointer to an usable WorkItem from the item pool. Allocate one if no more free items.
		RefPtr<WorkItem> GetFreeItem();
		/// Add a work item and resume worker threads.
    	void AddWorkItem(RefPtr<WorkItem> item);
		/// Pause worker threads.
		void Pause();
		/// Resume worker threads.
		void Resume();
		/// Finish all queued work which has at least the specified priority. Main thread will also execute priority work. Pause worker threads if no more work remains.
		void Complete(UINT priority);
    	/// Set the pool telerance before it starts deleting pool items.
    	void SetTolerance(int tolerance) { tolerance_ = tolerance; }
	    
		/// Return number of worker threads.
		UINT GetNumThreads() const { return threads_.size(); }
		/// Return whether all work with at least the specified priority is finished.
		bool IsCompleted(UINT priority) const;
    	/// Return the pool tolerance.
    	int GetTolerance() const { return tolerance_; }
	    
	private:
		/// Process work items until shut down. Called by the worker threads.
		void ProcessItems(UINT threadIndex);
    	/// Purge completed work items which have at least the specified priority, and send completion events as necessary.
   	 	void PurgeCompleted(unsigned priority);
    	/// Purge the pool to reduce allocation where its unneeded.
    	void PurgePool();
		/// Handle frame start event. Purge completed work from the main thread queue, and perform work if no threads at all.
		void HandleBeginFrame(/*StringHashstd::string eventType, VariantMap& eventData*/);
	    
		/// Worker threads.
		std::vector< RefPtr<WorkerThread> > threads_;
    	/// Work item pool for reuse to cut down on allocation. The bool is a flag for item pooling and whether it is available or not.
    	std::list< RefPtr<WorkItem> > poolItems_;
		/// Work item collection. Accessed only by the main thread.
		std::list< RefPtr<WorkItem> > workItems_;
		/// Work item prioritized queue for worker threads. Pointers are guaranteed to be valid (point to workItems.)
		std::list<WorkItem*> queue_;
		/// Worker queue mutex.
		CriticalSection queueMutex_;
		/// Shutting down flag.
		volatile bool shutDown_;
		/// Pausing flag. Indicates the worker threads should not contend for the queue mutex.
		volatile bool pausing_;
		/// Paused flag. Indicates the queue mutex being locked to prevent worker threads using up CPU time.
		bool paused_;
		/// Tolerance for the shared pool before it begins to deallocate.
		int tolerance_;
		/// Last size of the shared pool.
		UINT lastSize_;
	};

	ENGINE_API WorkQueue* GetWorkQueue();
	ENGINE_API void SetWorkQueue(WorkQueue* pWorkQueue);

}

#endif
