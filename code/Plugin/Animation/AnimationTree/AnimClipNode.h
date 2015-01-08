#ifndef  _Animation_ClipNode__H__
#define  _Animation_ClipNode__H__


namespace ma
{
	class AnimationClip;
	struct AnimEvalContext;
	class BoneSet;


	class ANIMATION_API AnimClipNode : public AnimTreeNode
	{
		DECL_OBJECT(AnimClipNode)

	public:
		AnimClipNode();
	
		~AnimClipNode();

		virtual void	SetAnimationClip(const char* pszSkaPath);

		virtual void	SetBoneSet(const char* pBoneSetName);

		virtual void	AdvanceTime(float fTimeElapsed);

		virtual void	EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight,EBlendMode eBlendMode);

		virtual	void	SetFrame(float fFrame);

		virtual void	SetSkeletion(Skeleton* pSkeletion);

		virtual void	Serialize(Serializer& sl, const char* pszLable /*= "AnimClipNode" */);

		virtual bool	OnLoadOver();

		Skeleton*		GetSkeleton() {return m_pSkeleton.get();}
	
		void			SetAnimationClip(AnimationClip* pAnimClip); 

		void			SetBoneSet(BoneSet* pBoneSet) {m_pBoneSet = pBoneSet;}

	private:
		AnimationClip*	m_pAnimClip;
		std::string		m_strClipName;

		BoneSet*		m_pBoneSet;
		std::string     m_strBoneSetName;
	
		RefPtr<Skeleton>	m_pSkeleton;

		bool			m_bClipInit;
	};
}




#endif