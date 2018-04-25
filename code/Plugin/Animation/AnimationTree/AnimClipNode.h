#ifndef  _Animation_ClipNode__H__
#define  _Animation_ClipNode__H__

#include "Animation/BoneMap.h"

namespace ma
{
	class AnimationClip;
	struct AnimEvalContext;
	class BoneSet;


	class AnimClipNode : public AnimTreeNode
	{
	public:
		AnimClipNode();

		~AnimClipNode();

		DECL_OBJECT(AnimClipNode)
	
		static void		RegisterAttribute();

		virtual void	SetAnimationClip(const char* pszSkaPath);
		const char*		GetAnimationClip() const;

		void			SetStartFrame(uint32_t nStartFrame) {m_nStartFrame = nStartFrame;}
		uint32_t			GetStartFrame() const {return m_nStartFrame;}

		void			SetEndFrame(uint32_t nEndFram) {m_nEndFrame = nEndFram;}
		uint32_t			GetEndFrame() const {return m_nEndFrame;}

		virtual uint32_t	GetFrameCount();

		virtual void	Activate();

		virtual void	AdvanceTime(float fTimeElepse);

		virtual void	EvaluateAnimation(AnimationNodeOutput &output, float fWeight);

		virtual	void	SetFrame(float fFrame);

		virtual bool	IsReady();

		void			AddFrameEvent(FrameEvent* pFrameEvent);

	private:
		string					m_strSkaName;
		RefPtr<Animation>		m_pAnimation;

		BoneMap					m_NodeLink;

		uint32_t					m_nStartFrame;
		uint32_t					m_nEndFrame;

		float					m_fSkaFrame;

		RefPtr<Skeleton>		m_pSkeleton;

		RefPtr<BoneSet>			m_pBoneSet;

		bool					m_bLoadOver;

		Animatable				m_player;

		typedef vector< RefPtr<FrameEvent> > VEC_FRAMEEVENT;
		VEC_FRAMEEVENT			m_vecFrameEvent;
	};

	RefPtr<AnimClipNode> CreateClipNode(const char* skaName, const char* pszName = NULL);
}




#endif
