#ifndef  _AnimationInstance__H__
#define  _AnimationInstance__H__

namespace ma
{
	class Animation;

	enum eS3LPlaybackMode
	{
		S3L_PLAYBACK_ONCE = 0,
		S3L_PLAYBACK_LOOP
	};

	enum eS3LPlayerStatus
	{
		S3L_PLAYER_STOP = 0,
		S3L_PLAYER_PLAYING,
		S3L_PLAYER_PAUSE,
		S3L_PLAYER_STATUS_NUM,
	};

	class ANIMATION_API AnimationInst
	{
		AnimationInst(Animation* pAnimation);

		~AnimationInst();

		void EvaluateAnimation(std::vector<maNodeTransform>&m_arrTSFLS,float fWeight = 1.0f);

		void AdvanceTime(float fTimeElapsed);

	private:
		Animation* m_pAnimation;
		//const IS3ANodeLink*	m_pNodeLink;
		float m_fLocalFrame;
		float m_fPlaySpeed;

		eS3LPlaybackMode m_playbackMode;
		eS3LPlayerStatus m_playerStatus;

	};
}


#endif