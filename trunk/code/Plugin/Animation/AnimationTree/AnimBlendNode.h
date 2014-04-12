#ifndef  _Animation_BlendNode__H__
#define  _Animation_BlendNode__H__


namespace ma
{
	struct AnimEvalContext;

	class ANIMATION_API AnimBlendNode : public AnimTreeNode
	{
		DECL_OBJECT(AnimBlendNode)

	public:
		AnimBlendNode();
		
		~AnimBlendNode();

		virtual void	SetSrcAnimNode(AnimTreeNode* pAnimNode) {m_pSrcAnimNode = pAnimNode;}

		virtual void	SetDestAnimNode(AnimTreeNode* pAnimNode) {m_pDestAnimNode = pAnimNode;}

		virtual void	SetWeight(float fWeight) { m_fWeight = fWeight;}

		virtual void	AdvanceTime(float fTimeElapsed);

		virtual void	EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight, EBlendMode eBlendMode);

		virtual	void	SetFrame(float fFrame);

	private:
		AnimTreeNode*	m_pSrcAnimNode;
		AnimTreeNode*	m_pDestAnimNode;
		float			m_fWeight;	
	};
}




#endif