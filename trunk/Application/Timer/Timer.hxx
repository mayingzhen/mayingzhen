#include "Application/Timer/Timer.h"

namespace ma
{

	int gettimeofday(struct timeval * val, struct timezone *)
	{
		if (val)
		{
			SYSTEMTIME wtm;
			GetLocalTime(&wtm);

			struct tm tTm;
			tTm.tm_year     = wtm.wYear - 1900;
			tTm.tm_mon      = wtm.wMonth - 1;
			tTm.tm_mday     = wtm.wDay;
			tTm.tm_hour     = wtm.wHour;
			tTm.tm_min      = wtm.wMinute;
			tTm.tm_sec      = wtm.wSecond;
			tTm.tm_isdst    = -1;

			val->tv_sec     = (long)mktime(&tTm);       // time_t is 64-bit on win32
			val->tv_usec    = wtm.wMilliseconds * 1000;
		}
		return 0;
	}

	Time::Time()
		:m_frameCnt(0)
		,m_fFrameTotalTime(0.0)
		,m_fFrameDeltaTime(0.0f)
	{
		Reset();
	}



	Time* g_pTimer = NULL;

	Time* GetTimer()
	{
		return g_pTimer;
	}

	void  SetTimer(Time* pTimer)
	{
		g_pTimer = pTimer;
	}


	Time::~Time()
	{
	}

	void	Time::Reset()
	{
		m_frameCnt = 0;
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
		++m_frameCnt;
	}


	float	Time::GetFrameTotalTime()const
	{
		return m_fFrameTotalTime;
	}


	float	Time::GetFrameDeltaTime()const
	{
		return m_fFrameDeltaTime;
	}

}
