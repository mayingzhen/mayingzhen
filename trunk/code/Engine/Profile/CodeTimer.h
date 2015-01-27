#ifndef __CodeTimer_H
#define __CodeTimer_H

#define _max_func_size          256

//#define PROFILE_CODE_TIMER

#ifdef PROFILE_CODE_TIMER

#define profile_code()          profile_code_time(__LINE__)

#define profile_code_time(name) static CodeTimer __ct_##name(__FUNCTION__); \
	FuncTimer _ct_func_##name(&__ct_##name)

#define begin_code_time(name)   static CodeTimer(#name) __ct_##name; \
	__ct_##name.Begin()

#define end_code_time(name)     static __s_##name.End()

#define output_all_code_time()  GetCodetTimeMng()->OuputAll()

#else

#define profile_code()
#define profile_code_time(name)
#define begin_code_time(name)
#define end_code_time(name)
#define output_all_code_time()

#endif

namespace ma
{
	class CodeTimer
	{
		friend class CodeTimerManager;

	public:
		CodeTimer(const char * func);
		~CodeTimer();

		void            Begin();
		void            End();
		void            Reset();
		void            Output() const;

		const char *    Function() const;
		float           AverageTime() const;
		float           TotalTime() const;
		float           CurrentTime() const;
		int             TotalCalls() const;
		float           MaxTime() const;
		float           MinTime() const;

		CodeTimer*		Next() const;

	protected:
		char            munction[_max_func_size];
		float           mStartTime;
		float           mTotalTime;
		float           mCurrentTime;
		int             mTotalCalls;
		float           mMaxTime;
		float           mMinTime;
		float           mAveTime;
		CodeTimer *     mNext;
	};

	class FuncTimer
	{
	public:
		FuncTimer(CodeTimer * timer);
		~FuncTimer();

	protected:
		CodeTimer * mCodeTime;
	};

	class CodeTimerManager 
	{
	public:
		CodeTimerManager();

		~CodeTimerManager();

		void            AddTime(CodeTimer * time);

		CodeTimer*		GetCodeTimer() {return mTimers;}

		void            OuputAll();

	protected:
		CodeTimer *     mTimers;
	};

	CodeTimerManager* GetCodetTimeMng();

	void SetCodeTimeMng(CodeTimerManager* pCodeTimeMng);
}



#include "CodeTimer.inl"



#endif
