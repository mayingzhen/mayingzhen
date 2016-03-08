#ifndef __Timer_H__
#define __Timer_H__

namespace ma
{

	class Time
	{
	public:

		Time();

		~Time();

		void	Reset();

		void	UpdateFrame();

		float	GetFrameDeltaTime()const;

		UINT	GetFrameCount() const;

		uint64  GetMillisceonds() const;
		
	private:
		UINT	m_nFameCnt;		   //time of update called
		uint64	m_nFrameTotalTime; //total time since last reset
		float	m_fFrameDeltaTime; //last frame's delta time in seconds
	};

	extern Time* g_pTimer;
	Time* GetTimer();
}

#endif// __Timer_H__
