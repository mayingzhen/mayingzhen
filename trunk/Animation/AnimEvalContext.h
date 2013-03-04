#ifndef  _AnimationEvalContext__H__
#define  _AnimationEvalContext__H__

namespace ma
{
	class SkeletonPose;

	struct ANIMATION_API AnimEvalContext
	{
		std::vector<maNodeTransform> m_arrTSFLS;

		const SkeletonPose* m_refNodePos;

		SkeletonPose* m_pNodePos;
	};
}


#endif