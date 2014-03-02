#ifndef  _AnimationClip__H__
#define  _AnimationClip__H__

namespace ma
{
	class Animation;
	class BoneMap;
	class BoneSet;
	struct AnimEvalContext;

	enum ePlaybackMode
	{
		S3L_PLAYBACK_ONCE = 0,
		S3L_PLAYBACK_LOOP
	};

	enum ePlayerStatus
	{
		S3L_PLAYER_STOP = 0,
		S3L_PLAYER_PLAYING,
		S3L_PLAYER_PAUSE,
		S3L_PLAYER_STATUS_NUM,
	};

	class ANIMATION_API AnimationClip
	{
	public:
		AnimationClip(Animation* pAnimation,Skeleton* pSkeleton = NULL);

		~AnimationClip();

		void		SetSkeleton(Skeleton* pSkeleton = NULL);

		void		EvaluateAnimation(AnimEvalContext* pEvalContext,float fWeight = 1.0f,BoneSet* pBoneSet = NULL);

		void		AdvanceTime(float fTimeElapsed);

		void		SetFrame(float fFrame);

		BoneMap*	GetNodeLink() {return m_pNodeLink;}

		Animation*	GetAnimation() {return m_pAnimation;}

	private:
		void		WrapLocalFrame();

	private:
		Animation*	m_pAnimation;

		BoneMap*	m_pNodeLink;

		float		m_fLocalFrame;
		
		float		m_fPlaySpeed;

		ePlaybackMode m_playbackMode;

		ePlayerStatus m_playerStatus;

	};
}


#endif // _AnimationClip__H__