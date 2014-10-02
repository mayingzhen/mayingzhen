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
		PLAYBACK_ONCE = 0,
		PLAYBACK_LOOP
	};

	enum ePlayerStatus
	{
		PLAYER_STOP = 0,
		PLAYER_PLAYING,
		PLAYER_PAUSE,
		PLAYER_STATUS_NUM,
	};


	class ANIMATION_API AnimationClip
	{
	public:
		AnimationClip(RefPtr<Animation> pAnimation,Skeleton* pSkeleton = NULL);

		~AnimationClip();

		void		SetSkeleton(Skeleton* pSkeleton = NULL);

		void		EvaluateAnimation(AnimEvalContext* pEvalContext,float fWeight = 1.0f, BoneSet* pBoneSet = NULL,EBlendMode eBlendMode = BLENDMODE_ADDITIVE);

		void		AdvanceTime(float fTimeElapsed);

		void		SetFrame(float fFrame);

		BoneMap*	GetNodeLink() {return m_pNodeLink;}

		void		SetPlaybackMode(ePlaybackMode playMode) {m_playbackMode = playMode;}

	private:
		void		WrapLocalFrame();

	private:
		RefPtr<Animation>	m_pAnimation;

		BoneMap*			m_pNodeLink;

		Skeleton*			m_pSkeleton;

		float				m_fLocalFrame;
		
		float				m_fPlaySpeed;

		ePlaybackMode		m_playbackMode;

		ePlayerStatus		m_playerStatus;

		bool				m_bInit;

	};
}


#endif // _AnimationClip__H__