#ifndef __Timer_H__
#define __Timer_H__

namespace ma
{

	class ENGINE_API Time
	{
		long	m_frameCnt;	  //time of update called
		float	m_fFrameTotalTime; //total time since last reset
		float	m_fFrameDeltaTime; //last frame's delta time in seconds

		timeval m_lastReset;

	public:

		Time();

		~Time();

		void	Reset();

		void	UpdateFrame();

		//get milliseconds since last reset
		float	GetMillisceonds() const;

		//get milliseconds between last reset and last update frame
		float	GetFrameTotalTime()const;

		float	GetFrameDeltaTime()const;
		
		static void	Sleep(UINT mSec);
		
	};

	ENGINE_API Time* GetTimer();

	ENGINE_API void  SetTimer(Time* pTimer);
}

#endif// __Timer_H__
