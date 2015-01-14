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

		static void		RegisterObject(Context* context);

		virtual void	SetAnimationClip(const char* pszSkaPath);
		const char*		GetAnimationClip() const;

		virtual void	SetBoneSet(const char* pBoneSetName);
		const char*		GetBoneSet() const;

		virtual void	AdvanceTime(float fTimeElapsed);

		virtual void	EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight,EBlendMode eBlendMode);

		virtual	void	SetFrame(float fFrame);

		virtual void	SetSkeletion(Skeleton* pSkeletion);
		Skeleton*		GetSkeleton() const {return m_pSkeleton.get();}

		virtual bool	OnLoadOver();

	private:
		RefPtr<AnimationClip>	m_pAnimClip;

		string					m_strBoneSetName;
		RefPtr<BoneSet>			m_pBoneSet;
		
		RefPtr<Skeleton>		m_pSkeleton;

		bool					m_bClipInit;
	};
	
	ANIMATION_API RefPtr<AnimClipNode> CreateClipNode(const char* skaName,const char* boneSetName =  NULL);
}




#endif