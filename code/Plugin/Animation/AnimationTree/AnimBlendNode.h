#ifndef  _Animation_BlendNode__H__
#define  _Animation_BlendNode__H__


namespace ma
{
	struct AnimEvalContext;

	class ANIMATION_API AnimBlendNode : public IAnimBlendNode
	{
		DECL_OBJECT(AnimBlendNode)

	public:
		AnimBlendNode();
		
		~AnimBlendNode();

		virtual void	SetSrcAnimNode(IAnimTreeNode* pAnimNode) {m_pSrcAnimNode = pAnimNode;}

		virtual void	SetDestAnimNode(IAnimTreeNode* pAnimNode) {m_pDestAnimNode = pAnimNode;}

		virtual void	SetWeight(float fWeight) { m_fWeight = fWeight;}

		virtual void	AdvanceTime(float fTimeElapsed);

		virtual void	EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight);

		virtual	void	SetFrame(float fFrame);

		virtual void	SetSkeleton(Skeleton* pSkeleton) {}

		virtual void	Serialize(Serializer& sl, const char* pszLable = "AnimBlendNode" );

	private:
		IAnimTreeNode*	m_pSrcAnimNode;
		IAnimTreeNode*	m_pDestAnimNode;
		float			m_fWeight;
		
	};
}




#endif