#include "Timer.h"

namespace ma
{
    Time* g_pTimer = NULL;

	Time* GetTimer()
	{
		return g_pTimer;
	}


	Time::Time()
	{
		m_nFameCnt = 0;
		m_nFrameTotalTime = 0;
		m_fFrameDeltaTime = 0.0f;
	}

	Time::~Time()
	{

	}

	uint64_t Time::GetMillisceonds() const
	{
		return StaticFunc::GetTime();
	}

	void Time::UpdateFrame()
	{
		uint64_t curTime = GetMillisceonds();
		m_fFrameDeltaTime = (curTime - m_nFrameTotalTime) * 0.001f;
		m_nFrameTotalTime = curTime;
		++m_nFameCnt;

		mTimeEvent.trigger(m_fFrameDeltaTime);
	}

	uint32_t Time::GetFrameCount() const
	{
		return m_nFameCnt;
	}

	float Time::GetFrameDeltaTime()const
	{
		return m_fFrameDeltaTime;
	}


}
