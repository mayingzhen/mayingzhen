#ifndef  _Animation_LayerNode__H__
#define  _Animation_LayerNode__H__


namespace ma
{
	struct AnimEvalContext;

	class ANIMATION_API AnimLayerNode : public AnimTreeNode
	{
		DECL_OBJECT(AnimLayerNode)

	public:
		virtual void AddLayer(AnimTreeNode* pAnimTreeNode);

		virtual void RemoveLayer(AnimTreeNode* pAnimTreeNode);

		virtual void AdvanceTime(float fTimeElapsed);

		virtual void EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight, EBlendMode eBlendMode);

		virtual void SetFrame(float fFrame);

		virtual void SetSkeletion(Skeleton* pSkeletion);

		virtual void Serialize(Serializer& sl, const char* pszLable/* = "AnimLayerNode" */);
		
		virtual bool OnLoadOver();

	private:
		typedef std::vector< RefPtr<AnimTreeNode> > VEC_ANIMNODE;
		VEC_ANIMNODE m_arrAnimNode;
	};

	ANIMATION_API RefPtr<AnimLayerNode> CreateLayerNode();
}




#endif