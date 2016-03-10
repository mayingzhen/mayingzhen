#ifndef  _Animation_LayerNode__H__
#define  _Animation_LayerNode__H__


namespace ma
{
	struct AnimEvalContext;

	class AnimLayerNode : public AnimTreeNode
	{
	
	public:
		DECL_OBJECT(AnimLayerNode)

		virtual void	AddLayer(AnimTreeNode* pAnimTreeNode);

		virtual void	RemoveLayer(AnimTreeNode* pAnimTreeNode);

		virtual void	AdvanceTime(float fTimeElapsed);

		virtual void	EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight);

		virtual void	SetFrame(float fFrame);

		virtual bool	Instantiate(Skeleton* pSkeletion);
		
		virtual bool	IsReady();

		virtual uint32	GetFrameCount();

	private:
		typedef std::vector< RefPtr<AnimTreeNode> > VEC_ANIMNODE;
		VEC_ANIMNODE m_arrAnimNode;
	};

	RefPtr<AnimLayerNode> CreateLayerNode();
}




#endif