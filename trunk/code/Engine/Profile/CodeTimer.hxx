#include "CodeTimer.h"

namespace ma
{
	CodeTimer::CodeTimer(const char * func)
		: mStartTime(0),
		mTotalTime(0),
		mCurrentTime(0),
		mTotalCalls(0),
		mMaxTime(0),
		mMinTime(FLT_MAX),
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
			mAveTime = (mAveTime + mCurrentTime) * 0.5f;

			if (mMinTime > mCurrentTime)
				mMinTime = mCurrentTime;
			if (mMaxTime < mCurrentTime)
				mMaxTime = mCurrentTime;

			mStartTime = 0.0f;
		}
	}

	void CodeTimer::Reset()
	{
		mStartTime = 0.0f;
		mTotalTime = 0.0f;
		mTotalCalls = 0;
		mMaxTime = 0.0f;
		mMinTime = FLT_MAX;
		mAveTime = 0.0f;
	}

	void CodeTimer::Output() const
	{
		Log("---------------------------------------------------------\n"  \
			"fuction:\t  %s\n" \
			"total time:\t  %f\n" \
			"current time:\t  %f\n" \
			"average time:\t  %f\n" \
			"total calls:\t  %d\n" \
			"max time:\t  %f\n" \
			"min time:\t  %f",
			Function(),
			TotalTime(),
			CurrentTime(),
			AverageTime(),
			TotalCalls(),
			MaxTime(),
			MinTime());
	}


	static CodeTimerManager* gPCodeTimerManager = NULL;

	CodeTimerManager* GetCodetTimeMng() 
	{
		return gPCodeTimerManager;
	}

	void SetCodeTimeMng(CodeTimerManager* pCodeTimeMng) 
	{
		gPCodeTimerManager = pCodeTimeMng;
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

