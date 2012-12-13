#ifndef  _AnimationEvalContext__H__
#define  _AnimationEvalContext__H__

namespace ma
{
	class NodePose;

	struct ANIMATION_API AnimEvalContext
	{
		std::vector<maNodeTransform> m_arrTSFLS;

		const NodePose* m_refNodePos;

		NodePose* m_pNodePos;
	};
}


#endif