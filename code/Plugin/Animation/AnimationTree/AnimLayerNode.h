#ifndef  _Animation_LayerNode__H__
#define  _Animation_LayerNode__H__

#include "Animation/AnimationTree/AnimTreeNode.h"

namespace ma
{
	struct AnimEvalContext;

	class ANIMATION_API AnimLayerNode : public AnimTreeNode
	{
	public:
		virtual void AddLayer(AnimTreeNode* pAnimTreeNode);

		virtual void RemoveLayer(AnimTreeNode* pAnimTreeNode);

		virtual void AdvanceTime(float fTimeElapsed);

		virtual void EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight);

		virtual void SetFrame(float fFrame);

		template <class T>
		T*			CreateLayer()
		{
			AnimTreeNode* pLayer = new T();
			m_arrAnimNode.push_back(pLayer);
			pLayer->SetSkeleton(m_pSkeleton);
			return (T*)pLayer;
		}

	private:
		std::vector<AnimTreeNode*> m_arrAnimNode;
	};
}




#endif