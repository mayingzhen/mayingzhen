#ifndef  _AnimationEvalContext__H__
#define  _AnimationEvalContext__H__

namespace ma
{
	class SkeletonPose;

	struct AnimEvalContext
	{
		std::vector<Transform> m_arrTSFLS;

		const SkeletonPose* m_refNodePos;

		SkeletonPose* m_pNodePos;
	};
}


#endif