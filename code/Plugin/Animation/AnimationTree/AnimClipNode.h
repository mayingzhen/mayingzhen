#ifndef  _Animation_ClipNode__H__
#define  _Animation_ClipNode__H__

#include "BoneMap.h"

namespace ma
{
	class AnimationClip;
	struct AnimEvalContext;
	class BoneSet;

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


	class AnimClipNode : public AnimTreeNode
	{
	public:
		AnimClipNode();

		~AnimClipNode();

		DECL_OBJECT(AnimClipNode)
	
		static void		RegisterAttribute();

		virtual void	SetAnimationClip(const char* pszSkaPath);
		const char*		GetAnimationClip() const;

		virtual void	SetBoneSet(const char* pBoneSetName);
		const char*		GetBoneSet() const;

		void			SetStartFrame(uint32 nStartFrame) {m_nStartFrame = nStartFrame;}
		uint32			GetStartFrame() const {return m_nStartFrame;}

		void			SetTotalFrame(uint32 nTotalFram) {m_nTotalFrame = nTotalFram;}
		uint32			GetTotalFrame() const {return m_nTotalFrame;}

		virtual bool	Instantiate();

		virtual void	AdvanceTime(float fTimeElapsed);

		virtual void	EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight,EBlendMode eBlendMode);

		virtual	void	SetFrame(float fFrame);

		virtual void	SetSkeletion(Skeleton* pSkeletion);
		Skeleton*		GetSkeleton() const {return m_pSkeleton.get();}

		virtual bool	IsReady();

	private:
		void			WrapLocalFrame();

	private:
		string					m_strSkaName;
		RefPtr<Animation>		m_pAnimation;

		string					m_strBoneSetName;
		RefPtr<BoneSet>			m_pBoneSet;

		BoneMap					m_NodeLink;

		float					m_fLocalFrame;

		float					m_fPlaySpeed;

		ePlaybackMode			m_playbackMode;

		ePlayerStatus			m_playerStatus;

		uint32					m_nStartFrame;

		uint32					m_nTotalFrame;

		RefPtr<Skeleton>		m_pSkeleton;

		bool					m_bLoadOver;
	};

	RefPtr<AnimClipNode> CreateClipNode();
	RefPtr<AnimClipNode> CreateClipNode(const char* skaName, const char* pszName = NULL,const char* boneSetName =  NULL);
}




#endif