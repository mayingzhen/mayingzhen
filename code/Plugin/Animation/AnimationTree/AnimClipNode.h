#ifndef  _Animation_ClipNode__H__
#define  _Animation_ClipNode__H__

#include "BoneMap.h"

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

		virtual void	SetBoneSet(const char* pBoneSetName);
		const char*		GetBoneSet() const;

		void			SetRefSkeleton(const char* pszRefSkeleton);
		const char*		GetRefSkeleton() const;

		void			SetStartFrame(uint32 nStartFrame) {m_nStartFrame = nStartFrame;}
		uint32			GetStartFrame() const {return m_nStartFrame;}

		void			SetEndFrame(uint32 nEndFram) {m_nEndFrame = nEndFram;}
		uint32			GetEndFrame() const {return m_nEndFrame;}

		virtual uint32	GetFrameCount();

		virtual bool	Instantiate(Skeleton* pSkeleton);

		virtual void	EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight);

		virtual	void	SetFrame(float fFrame);

		virtual bool	IsReady();

	private:
		string					m_strSkaName;
		RefPtr<Animation>		m_pAnimation;

		string					m_strBoneSetName;
		RefPtr<BoneSet>			m_pBoneSet;

		BoneMap					m_NodeLink;

		uint32					m_nStartFrame;
		uint32					m_nEndFrame;

		float					m_fSkaFrame;

		RefPtr<Skeleton>		m_pSkeleton;
		RefPtr<Skeleton>		m_pRefSkeleton;

		bool					m_bLoadOver;
	};

	RefPtr<AnimClipNode> CreateClipNode(const char* skaName, const char* pszName = NULL,const char* boneSetName =  NULL,const char* pRefSke = NULL);
}




#endif