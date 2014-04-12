#include "WorkQueue.h"
#include "Thread.h"


namespace ma
{

	const UINT MAX_NONTHREADED_WORK_USEC = 1000;

	/// Worker thread managed by the work queue.
	class WorkerThread : public Thread, public Referenced
	{
	public:
		/// Construct.
		WorkerThread(WorkQueue* owner, UINT index) :
			owner_(owner),
			index_(index)
		{
		}
	    
		/// Process work items until stopped.
// 		virtual void ThreadFunction()
// 		{
// 			// Init FPU state first
// 			InitFPU();
// 		}

		virtual	void	ThreadUpdate()
		{
			owner_->ProcessItems(index_);
		}
	    
		/// Return thread index.
		UINT GetIndex() const { return index_; }
	    
	private:
		/// Work queue.
		WorkQueue* owner_;
		/// Thread index.
		UINT index_;
	};

	WorkQueue::WorkQueue(/*Context* context*/) :
		//Object(context),
		shutDown_(false),
		pausing_(false),
		paused_(false)
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
			ref_ptr<WorkerThread> pThread = new WorkerThread(this, i + 1);
			pThread->Start();
			threads_.push_back(pThread);
		}
	}

	void WorkQueue::AddWorkItem(const WorkItem& item)
	{
		// Push to the main thread list to keep item alive
		// Clear completed flag in case item is reused
		workItems_.push_back(item);
		WorkItem* itemPtr = &workItems_.back();
		itemPtr->completed_ = false;
	    
		// Make sure worker threads' list is safe to modify
		if (threads_.size() && !paused_)
			queueMutex_.Lock();
	    
		// Find position for new item
		if (queue_.empty())
			queue_.push_back(itemPtr);
		else
		{
			for (std::list<WorkItem*>::iterator i = queue_.begin(); i != queue_.end(); ++i)
			{
				if ((*i)->priority_ <= itemPtr->priority_)
				{
					queue_.insert(i, itemPtr);
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
	    
		PurgeCompleted();
	}

	bool WorkQueue::IsCompleted(UINT priority) const
	{
		for (std::list<WorkItem>::const_iterator i = workItems_.begin(); i != workItems_.end(); ++i)
		{
			if (i->priority_ >= priority && !i->completed_)
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

	void WorkQueue::PurgeCompleted()
	{
		//using namespace WorkItemCompleted;
	    
		//VariantMap& eventData = GetEventDataMap();
	    
		// Purge completed work items and send completion events.
		for (std::list<WorkItem>::iterator i = workItems_.begin(); i != workItems_.end();)
		{
			if (i->completed_)
			{
				if (i->sendEvent_)
				{
					//eventData[P_ITEM] = (void*)(&(*i));
					//SendEvent(E_WORKITEMCOMPLETED, eventData);
				}
	            
				i = workItems_.erase(i);
			}
			else
				++i;
		}
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
	    
		PurgeCompleted();
	}

}
