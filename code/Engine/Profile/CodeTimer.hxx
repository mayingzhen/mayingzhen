#include "CodeTimer.h"

namespace ma
{
	CodeTimer::CodeTimer(const char * func)
		: mStartTime(0),
		mTotalTime(0),
		mCurrentTime(0),
		mTotalCalls(0),
		mMaxTime(0),
		mMinTime(0),
		mAveTime(0),
		mNext(NULL)
	{
		strncpy(munction, func, _max_func_size);
		GetCodetTimeMng()->AddTime(this);
	}

	CodeTimer::~CodeTimer()
	{
	}

	void CodeTimer::Begin()
	{
		if (!mStartTime)
		{
			++mTotalCalls;
			//mStartTime = System::InternalTime.ElapsedTime();
			mStartTime = GetTimer()->GetMillisceonds(); 
		}
	}

	void CodeTimer::End()
	{
		if (mStartTime)
		{
			//mCurrentTime = System::InternalTime.ElapsedTime() - mStartTime;
			mCurrentTime = GetTimer()->GetMillisceonds() - mStartTime;
			assert (mCurrentTime >= 0);
			mTotalTime +=  mCurrentTime;
			mAveTime = (mAveTime + mCurrentTime) / 2;

			if (mMinTime > mCurrentTime)
				mMinTime = mCurrentTime;
			if (mMaxTime < mCurrentTime)
				mMaxTime = mCurrentTime;

			mStartTime = 0;
		}
	}

	void CodeTimer::Reset()
	{
		mStartTime = 0;
		mTotalTime = 0;
		mTotalCalls = 0;
		mMaxTime = 0;
		mMinTime = 0;
		mAveTime = 0;
	}

	void CodeTimer::Output() const
	{
		LogInfo("---------------------------------------------------------\n"  \
			"fuction:\t  %s\n" \
			"total time:\t  %d\n" \
			"current time:\t  %d\n" \
			"average time:\t  %d\n" \
			"total calls:\t  %d\n" \
			"max time:\t  %d\n" \
			"min time:\t  %d",
			Function(),
			TotalTime(),
			CurrentTime(),
			AverageTime(),
			TotalCalls(),
			MaxTime(),
			MinTime());
	}


	CodeTimerManager* g_pCodeTimerManager = NULL;

	CodeTimerManager* GetCodetTimeMng() 
	{
		return g_pCodeTimerManager;
	}
	
	CodeTimerManager::CodeTimerManager()
		: mTimers(NULL)
	{
	}

	CodeTimerManager::~CodeTimerManager()
	{
	}


	void CodeTimerManager::AddTime(CodeTimer * time)
	{
		time->mNext = mTimers;
		mTimers = time;
	}

	void CodeTimerManager::OuputAll()
	{
		CodeTimer * time = mTimers;

		while (time)
		{
			time->Output();
			time = time->mNext;
		}
	}
}

