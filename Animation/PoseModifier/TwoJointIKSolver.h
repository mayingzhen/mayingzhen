#ifndef __TwoJointIKSolver_H__
#define __TwoJointIKSolver_H__

namespace ma
{
	class ANIMATION_API TwoJointIKSolver : public PoseModifier
	{
		void Init(BoneIndex uBoneAInd,BoneIndex uBoneBInd,BoneIndex uBoneCInd,
			const D3DVECTOR& vHingAxisLs,const Skeleton* pResfSkel);

		void UpdatePose(SkeletonPose* pNodePose);

	private:
		const Skeleton* m_pResfSkel;
		BoneIndex m_uBoneAID;
		BoneIndex m_uBoneBID;
		BoneIndex m_uBoneCID;
		Vector3 m_vHingAxisLS;
		Vector3 m_vGoalOS;

		// 
		NodeTransform m_pivotALS;
		NodeTransform m_pivotBLS;
		NodeTransform m_pivotCLS;
	};
}


#endif