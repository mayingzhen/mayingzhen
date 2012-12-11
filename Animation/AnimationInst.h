#ifndef  _AnimationInstance__H__
#define  _AnimationInstance__H__

namespace ma
{
	class Animation;
	class NodeLink;
	class BoneSet;
	class AnimEvalContext;

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
	public:
		AnimationInst(Animation* pAnimation,Skeleton* pSkeleton = NULL);

		~AnimationInst();

		void EvaluateAnimation(AnimEvalContext* pEvalContext,float fWeight = 1.0f,BoneSet* pBoneSet = NULL);

		void AdvanceTime(float fTimeElapsed);

		NodeLink* GetNodeLink() {return m_pNodeLink;}

		Animation* GetAnimation() {return m_pAnimation;}

	private:
		Animation* m_pAnimation;

		NodeLink* m_pNodeLink;

		float m_fLocalFrame;
		float m_fPlaySpeed;

		eS3LPlaybackMode m_playbackMode;
		eS3LPlayerStatus m_playerStatus;

	};
}


#endif