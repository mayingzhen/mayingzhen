#pragma once

#define DEFAULT_INTERVAL (1000.0f / 30.0f)

namespace ma
{

	class Animatable 
	{
	public:
		Animatable(void);
		~Animatable(void);

		void Play(float fElapsedTime = 0);

		// ���Ϊ-1����ԭֹ֡ͣ
		void Stop(float fTime = -1.0f);
		void Pause();
		void Resume();
		void Step(float fTime);
		void SetInterval(float nIntervalMillisecond){m_fIntervalMillisecond = nIntervalMillisecond;}
		float GetInterval() const{return m_fIntervalMillisecond;}
		float CalcLocalTime();
		void SetLoop(uint32_t nLoopTimes){mLoop = nLoopTimes;}
		uint32_t GetLoop() const{return mLoop == 0 ? -1 : mLoop;}

		float GetPlaySpeed() const {return  DEFAULT_INTERVAL / m_fIntervalMillisecond;}
		void SetPlaySpeed(float fPlaySpeed) {m_fIntervalMillisecond = DEFAULT_INTERVAL / fPlaySpeed;}

		enum STATE
		{
			State_NONE,
			State_Playing,
			State_Paused,
			State_Stopped,
		};
		bool IsPlaying() const{return m_state == State_Playing;}
		bool IsStopped() const{return m_state == State_Stopped;}
		void Reset();

		STATE GetState() const {return m_state;}

		float Process();
		void SetFrameCount(uint32_t nFrmaeCount) {m_fFrameCount = (float)nFrmaeCount;}
		float GetFrame() {return m_fLocalFrame;}
		virtual void SetFrame(float fFrame);

	protected:
		float m_fOldElapsedTime;
		float m_fElapsedTime;
		uint64_t m_nLastTime;
		STATE m_state;

		float m_fIntervalMillisecond;
		uint32_t mLoop;

		float m_fFrameCount;
		float m_fLocalFrame;
	};

}