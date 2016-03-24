#include "Animatable.h"
#include "Timer.h"

namespace ma
{

	Animatable::Animatable(void)
	{
		m_fOldElapsedTime = 0;
		m_fElapsedTime = 0;
		m_nLastTime = 0;
		m_state = State_NONE;
		m_fIntervalMillisecond = DEFAULT_INTERVAL;
		mLoop = 0xFFFFffff;
	}

	Animatable::~Animatable(void)
	{
	}

	void Animatable::Play( float fElapsedTime /*= 0*/ )
	{
		m_fOldElapsedTime = fElapsedTime;
		m_fElapsedTime = m_fOldElapsedTime;
		m_nLastTime = GetTimer()->GetMillisceonds();
		m_state = State_Playing;
	}

	void Animatable::Stop(float fTime)
	{
		m_nLastTime = GetTimer()->GetMillisceonds();
		if (fTime < 0)
		{
			m_fOldElapsedTime = m_fElapsedTime;
		}
		else
		{
			m_fOldElapsedTime = fTime;
		}
		
		m_state = State_Stopped;
	}

	void Animatable::Pause()
	{
		m_nLastTime = GetTimer()->GetMillisceonds();
		m_fOldElapsedTime = m_fElapsedTime;
		m_state = State_Paused;
	}

	void Animatable::Resume()
	{
		m_nLastTime = GetTimer()->GetMillisceonds();
		m_state = State_Playing;
	}

	void Animatable::Step(float fTime)
	{
		m_fOldElapsedTime = fTime;
		m_fElapsedTime = fTime;
		m_nLastTime = GetTimer()->GetMillisceonds();
		m_state = State_Paused;
	}

	float Animatable::CalcLocalTime()
	{
		switch(m_state)
		{
		case State_Playing:
			m_fElapsedTime = m_fOldElapsedTime + GetTimer()->GetMillisceonds() - m_nLastTime;
			return m_fElapsedTime;
		case State_Paused:
		case State_NONE:
		case State_Stopped:
			return m_fOldElapsedTime;
		}

		ASSERT(false);
		return 0;
	}

	void Animatable::Reset()
	{
		m_fElapsedTime = 0;
		m_nLastTime = GetTimer()->GetMillisceonds();  
	}


}