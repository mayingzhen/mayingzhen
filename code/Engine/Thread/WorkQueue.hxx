#include "WorkQueue.h"
#include "Thread.h"


namespace ma
{
	WorkQueue* g_pWorkQueue = NULL;
	WorkQueue* GetWorkQueue() {return g_pWorkQueue;}

	const UINT MAX_NONTHREADED_WORK_USEC = 1000;

	/// Worker thread managed by the work queue.

	void WorkerThread::ThreadUpdate()
	{
		owner_->ProcessItems(index_);
	}


	WorkQueue::WorkQueue(/*Context* context*/) :
		//Object(context),
		shutDown_(false),
		pausing_(false),
    	paused_(false),
    	tolerance_(10),
    	lastSize_(0)
	{
		//SubscribeToEvent(E_BEGINFRAME, HANDLER(WorkQueue, HandleBeginFrame));
	}

	WorkQueue::~WorkQueue()
	{
		// Stop the worker threads. First make sure they are not waiting for work items
		shutDown_ = true;
		Resume();
	    
		for (UINT i = 0; i < threads_.size(); ++i)
			threads_[i]->Stop();
	}

	void WorkQueue::CreateThreads(UINT numThreads)
	{
		// Other subsystems may initialize themselves according to the number of threads.
		// Therefore allow creating the threads only once, after which the amount is fixed
		if (!threads_.empty())
			return;
	    
		// Start threads in paused mode
		Pause();
	    
		for (UINT i = 0; i < numThreads; ++i)
		{
			RefPtr<WorkerThread> pThread = new WorkerThread(this, i + 1);
			pThread->Start();
			threads_.push_back(pThread);
		}
	}
	

	RefPtr<WorkItem> WorkQueue::GetFreeItem()
	{
    	if (poolItems_.size() > 0)
    	{
        	RefPtr<WorkItem> item = poolItems_.front();
        	poolItems_.pop_front();
        	return item;
    	}
    	else
    	{
        	// No usable items found, create a new one set it as pooled and return it.
        	RefPtr<WorkItem> item(new WorkItem());
        	item->pooled_ = true;
        	return item;
    	}
	}

	void WorkQueue::AddWorkItem(RefPtr<WorkItem> item)
	{
    	if (!item)
    	{
       		ASSERT("Null work item submitted to the work queue" && false);
        	return;
    	}

    	// Check for duplicate items.
		assert( std::find(workItems_.begin(),workItems_.end(),item) == workItems_.end() );
		// Push to the main thread list to keep item alive
		// Clear completed flag in case item is reused
		workItems_.push_back(item);
    	item->completed_ = false;
	    
		// Make sure worker threads' list is safe to modify
		if (threads_.size() && !paused_)
			queueMutex_.Lock();
	    
		// Find position for new item
		if (queue_.empty())
			queue_.push_back(item.get());
		else
		{
			for (std::list<WorkItem*>::iterator i = queue_.begin(); i != queue_.end(); ++i)
			{
				if ((*i)->priority_ <= item->priority_)
				{
					queue_.insert(i, item.get());
					break;
				}
			}
		}
	    
		if (threads_.size())
		{
			queueMutex_.Unlock();
			paused_ = false;
		}
	}

	void WorkQueue::Pause()
	{
		if (!paused_)
		{
			pausing_ = true;
	        
			queueMutex_.Lock();
			paused_ = true;
	        
			pausing_ = false;
		}
	}

	void WorkQueue::Resume()
	{
		if (paused_)
		{
			queueMutex_.Unlock();
			paused_ = false;
		}
	}


	void WorkQueue::Complete(UINT priority)
	{
		if (threads_.size())
		{
			Resume();
	        
			// Take work items also in the main thread until queue empty or no high-priority items anymore
			while (!queue_.empty())
			{
				queueMutex_.Lock();
				if (!queue_.empty() && queue_.front()->priority_ >= priority)
				{
					WorkItem* item = queue_.front();
					queue_.pop_front();
					queueMutex_.Unlock();
					item->workFunction_(item, 0);
					item->completed_ = true;
				}
				else
				{
					queueMutex_.Unlock();
					break;
				}
			}
	        
			// Wait for threaded work to complete
			while (!IsCompleted(priority))
			{
			}
	        
			// If no work at all remaining, pause worker threads by leaving the mutex locked
			if (queue_.empty())
				Pause();
		}
		else
		{
			// No worker threads: ensure all high-priority items are completed in the main thread
			while (!queue_.empty() && queue_.front()->priority_ >= priority)
			{
				WorkItem* item = queue_.front();
				queue_.pop_front();
				item->workFunction_(item, 0);
				item->completed_ = true;
			}
		}
	    
		PurgeCompleted(priority);
	}

	bool WorkQueue::IsCompleted(UINT priority) const
	{
		for (std::list< RefPtr<WorkItem> >::const_iterator i = workItems_.begin(); i != workItems_.end(); ++i)
		{
        	if ((*i)->priority_ >= priority && !(*i)->completed_)
				return false;
		}
	    
		return true;
	}

	void WorkQueue::ProcessItems(UINT threadIndex)
	{
		bool wasActive = false;
	    
		//for (;;)
		//{
			if (shutDown_)
				return;
	        
			if (pausing_ && !wasActive)
				Time::Sleep(0);
			else
			{
				queueMutex_.Lock();
				if (!queue_.empty())
				{
					wasActive = true;
	                
					WorkItem* item = queue_.front();
					queue_.pop_front();
					queueMutex_.Unlock();
					item->workFunction_(item, threadIndex);
					item->completed_ = true;
				}
				else
				{
					wasActive = false;
	                
					queueMutex_.Unlock();
					Time::Sleep(0);
				}
			}
		//}
	}

	void WorkQueue::PurgeCompleted(UINT priority)
	{
		// Purge completed work items and send completion events. Do not signal items lower than priority threshold,
		// as those may be user submitted and lead to eg. scene manipulation that could happen in the middle of the
		// render update, which is not allowed
		for (std::list< RefPtr<WorkItem> >::iterator i = workItems_.begin(); i != workItems_.end();)
		{
        if ((*i)->completed_ && (*i)->priority_ >= priority)
			{
            if ((*i)->sendEvent_)
            {
                	//using namespace WorkItemCompleted;
                
                	//VariantMap& eventData = GetEventDataMap();
                	//eventData[P_ITEM] = i->Get();
                	//SendEvent(E_WORKITEMCOMPLETED, eventData);
				}
	            
            	// Check if this was a pooled item and set it to usable
            	if ((*i)->pooled_)
            	{
                	// Reset the values to their defaults. This should 
                	// be safe to do here as the completed event has 
                	// already been handled and this is part of the 
               	 	// internal pool.
                	(*i)->start_ = NULL;
                	(*i)->end_ = NULL;
               		(*i)->aux_ = NULL;
                	(*i)->workFunction_ = NULL;
                	(*i)->priority_ = M_MAX_UNSIGNED;
                	(*i)->sendEvent_ = false;
                	(*i)->completed_ = false;

                	poolItems_.push_back(*i);
            	}
				i = workItems_.erase(i);
			}
			else
				++i;
		}
	}
	
	void WorkQueue::PurgePool()
	{
    	unsigned int currentSize = poolItems_.size();
    	int difference = lastSize_ - currentSize;

    	// Difference tolerance, should be fairly significant to reduce the pool size.
    	for (unsigned i = 0; poolItems_.size() > 0 && difference > tolerance_ && i < (unsigned)difference; i++)
        	poolItems_.pop_front();

    	lastSize_ = currentSize;
	}

	

	void WorkQueue::HandleBeginFrame(/*StringHashstd::string eventType, VariantMap& eventData*/)
	{
		// If no worker threads, complete low-priority work here
		if (threads_.empty() && !queue_.empty())
		{
			//PROFILE(CompleteWorkNonthreaded);
	        
			//HiresTimer timer;
	        
			while (!queue_.empty() /*&& timer.GetUSec(false) < MAX_NONTHREADED_WORK_USEC*/)
			{
				WorkItem* item = queue_.front();
				queue_.pop_front();
				item->workFunction_(item, 0);
				item->completed_ = true;
			}
		}
	    
    	PurgeCompleted(0);
    	PurgePool();
	}

}
