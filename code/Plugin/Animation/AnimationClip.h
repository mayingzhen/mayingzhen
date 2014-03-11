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
		AnimationClip(ref_ptr<Animation> pAnimation,Skeleton* pSkeleton = NULL);

		~AnimationClip();

		void		SetSkeleton(Skeleton* pSkeleton = NULL);

		void		EvaluateAnimation(AnimEvalContext* pEvalContext,float fWeight = 1.0f,
			BoneSet* pBoneSet = NULL,EBlendMode eBlendMode = BLENDMODE_ADDITIVE);

		void		AdvanceTime(float fTimeElapsed);

		void		SetFrame(float fFrame);

		BoneMap*	GetNodeLink() {return m_pNodeLink;}

		//Animation*	GetAnimation() {return m_pAnimation;}

	private:
		void		WrapLocalFrame();

	private:
		ref_ptr<Animation>	m_pAnimation;

		BoneMap*			m_pNodeLink;

		float				m_fLocalFrame;
		
		float				m_fPlaySpeed;

		ePlaybackMode		m_playbackMode;

		ePlayerStatus m_playerStatus;

	};
}


#endif // _AnimationClip__H__