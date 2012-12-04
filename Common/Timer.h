#ifndef __S3ATimer_H__
#define __S3ATimer_H__

namespace ma
{

	class COMMON_API Time
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

		
	};

	COMMON_API Time* GetTimer();

	COMMON_API void  SetTimer(Time* pTimer);
}

#endif// __S3ATimer_H__