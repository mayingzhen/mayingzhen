#ifndef  _Animation_LayerNode__H__
#define  _Animation_LayerNode__H__


namespace ma
{
	struct AnimEvalContext;

	class AnimLayerNode : public IAnimLayerNode
	{
		DECL_OBJECT(AnimLayerNode)

	public:
		virtual void AddLayer(IAnimTreeNode* pAnimTreeNode);

		virtual void RemoveLayer(IAnimTreeNode* pAnimTreeNode);

		virtual void AdvanceTime(float fTimeElapsed);

		virtual void EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight,EBlendMode eBlendMode);

		virtual void SetFrame(float fFrame);

		//virtual void SetSkeleton(Skeleton* pSkeleton);

		

	private:
		std::vector<IAnimTreeNode*> m_arrAnimNode;
	};
}




#endif