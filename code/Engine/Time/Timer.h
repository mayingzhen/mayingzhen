#ifndef __Timer_H__
#define __Timer_H__

namespace ma
{

	class Time
	{
	public:

		Time();

		~Time();

		void		Reset();

		void		UpdateFrame();

		float		GetFrameDeltaTime()const;

		uint32_t	GetFrameCount() const;

		uint64_t	GetMillisceonds() const;
		
	private:
		uint32_t	m_nFameCnt;		   //time of update called
		uint64_t	m_nFrameTotalTime; //total time since last reset
		float		m_fFrameDeltaTime; //last frame's delta time in seconds

	public:
		Signal< void(float) > mTimeEvent;
	};

	extern Time* g_pTimer;
	Time* GetTimer();
}

#endif// __Timer_H__
