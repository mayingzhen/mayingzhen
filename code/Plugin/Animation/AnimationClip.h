#ifndef  _AnimationClip__H__
#define  _AnimationClip__H__

#include "BoneMap.h"

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


	class ANIMATION_API AnimationClip : public Referenced
	{
	public:
		AnimationClip();

		~AnimationClip();
		
		void		SetAnimationPath(const char* pszSka);
		const char* GetAnimationPath() const;

		void		Instance();

		bool		OnLoadOver();

		Animation*	GetAnimation() {return m_pAnimation.get();}

		void		SetSkeleton(Skeleton* pSkeleton = NULL);

		void		EvaluateAnimation(AnimEvalContext* pEvalContext,float fWeight = 1.0f, BoneSet* pBoneSet = NULL,EBlendMode eBlendMode = BLENDMODE_ADDITIVE);

		void		AdvanceTime(float fTimeElapsed);

		void		SetFrame(float fFrame);

		void		SetPlaybackMode(ePlaybackMode playMode) {m_playbackMode = playMode;}

	private:
		void		WrapLocalFrame();

	private:
		RefPtr<Animation>	m_pAnimation;

		BoneMap				m_NodeLink;

		RefPtr<Skeleton>	m_pSkeleton;

		float				m_fLocalFrame;
		
		float				m_fPlaySpeed;

		ePlaybackMode		m_playbackMode;

		ePlayerStatus		m_playerStatus;

		bool				m_bLoadOver;

	};
}


#endif // _AnimationClip__H__