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
		m_fFrameCount = 1.0;
		m_fLocalFrame = 0;
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

	float Animatable::Process()
	{
		if ( !this->IsPlaying() )
			return 0;

		ASSERT(m_fFrameCount >= 1);

		float fAnimTime = (float)this->CalcLocalTime() / (float)this->GetInterval();
		float fLoopTime = (float)this->GetLoop() * (m_fFrameCount - 1.0f);
		if (m_fFrameCount <= 1.0f)
			fLoopTime = (float)this->GetLoop();

		if (fAnimTime >= fLoopTime)
		{
			this->SetFrame(m_fFrameCount - 1.00001f);

			this->Stop();
		}
		else
		{
			this->SetFrame(fAnimTime);
		}

		return m_fLocalFrame;
	}

	void Animatable::SetFrame(float fFrame)
	{
		if (fFrame < 0)
			fFrame = 0;

		m_fLocalFrame = fmod( fFrame, (float)m_fFrameCount - 1.0f + std::numeric_limits<Real>::epsilon() );
	}

}