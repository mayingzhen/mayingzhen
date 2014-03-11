#ifndef  _Animation_ClipNode__H__
#define  _Animation_ClipNode__H__


namespace ma
{
	class AnimationClip;
	struct AnimEvalContext;
	class BoneSet;


	class ANIMATION_API AnimClipNode : public IAnimClipNode
	{
		DECL_OBJECT(AnimClipNode)

	public:
		AnimClipNode(Skeleton* pSkeleton);
	
		~AnimClipNode();

		virtual void	SetAnimationClip(const char* pszSkaPath);

		virtual void	SetBoneSet(const char* pBoneSetName);

		virtual void	AdvanceTime(float fTimeElapsed);

		virtual void	EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight,EBlendMode eBlendMode);

		virtual	void	SetFrame(float fFrame);

		Skeleton*		GetSkeleton() {return m_pSkeleton;}
		
		//virtual void	SetSkeleton(Skeleton* pSkeleton); 

		void			SetAnimationClip(AnimationClip* pAnimClip); 

		void			SetBoneSet(BoneSet* pBoneSet) {m_pBoneSet = pBoneSet;}

	private:
		AnimationClip*	m_pAnimClip;

		BoneSet*		m_pBoneSet;
	
		Skeleton*		m_pSkeleton;
	};
}




#endif