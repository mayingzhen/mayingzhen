#include "Timer.h"

namespace ma
{
	static Time* gpTimer = NULL;

	Time* GetTimer()
	{
		return gpTimer;
	}

	void  SetTimer(Time* pTimer)
	{
		gpTimer = pTimer;
	}

	Time::Time()
		:m_nFameCnt(0)
		,m_fFrameTotalTime(0.0)
		,m_fFrameDeltaTime(0.0f)
	{
		Reset();
	}

	Time::~Time()
	{
	}

	void	Time::Reset()
	{
		m_nFameCnt = 0;
		m_fFrameTotalTime = 0.0;
		m_fFrameDeltaTime = 0.0f;
		gettimeofday(&m_lastReset,0);
	}

	float	Time::GetMillisceonds() const
	{
		timeval t;
		gettimeofday(&t, 0);
		return (t.tv_sec - m_lastReset.tv_sec) * 1000 + (t.tv_usec - m_lastReset.tv_usec) * 0.001f;
	}

	void	Time::UpdateFrame()
	{
		float curTime = GetMillisceonds() * 0.001f;
		m_fFrameDeltaTime = curTime - m_fFrameTotalTime;
		m_fFrameTotalTime = curTime;
		++m_nFameCnt;
	}


	float	Time::GetFrameTotalTime()const
	{
		return m_fFrameTotalTime;
	}
		
	UINT	Time::GetFrameCount() const
	{
		return m_nFameCnt;
	}

	float	Time::GetFrameDeltaTime()const
	{
		return m_fFrameDeltaTime;
	}

	void Time::Sleep(unsigned mSec)
	{
#ifdef WIN32
		::Sleep(mSec);
#else
		usleep(mSec * 1000);
#endif
	}


}
