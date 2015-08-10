#ifndef  _Animation_BlendNode__H__
#define  _Animation_BlendNode__H__


namespace ma
{
	struct AnimEvalContext;

	class AnimBlendNode : public AnimTreeNode
	{
	
	public:
		AnimBlendNode();
		
		~AnimBlendNode();

		DECL_OBJECT(AnimBlendNode)

		virtual void	SetSrcAnimNode(AnimTreeNode* pAnimNode) {m_pSrcAnimNode = pAnimNode;}

		virtual void	SetDestAnimNode(AnimTreeNode* pAnimNode) {m_pDestAnimNode = pAnimNode;}

		virtual void	SetWeight(float fWeight) { m_fWeight = fWeight;}

		virtual void	AdvanceTime(float fTimeElapsed);

		virtual void	EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight, EBlendMode eBlendMode);

		virtual	void	SetFrame(float fFrame);

		virtual void	SetSkeletion(Skeleton* pSkeletion);

		virtual bool	Instantiate();

		//virtual void	Serialize(Serializer& sl, const char* pszLable/* = "AnimBlendNode" */);

		virtual bool	IsReady();

	private:
		RefPtr<AnimTreeNode>	m_pSrcAnimNode;
		RefPtr<AnimTreeNode>	m_pDestAnimNode;
		float					m_fWeight;	
	};
}




#endif