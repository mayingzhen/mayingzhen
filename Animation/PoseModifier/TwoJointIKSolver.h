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
		D3DXVECTOR3 m_vHingAxisLS;
		D3DXVECTOR3 m_vGoalOS;

		// 
		maNodeTransform m_pivotALS;
		maNodeTransform m_pivotBLS;
		maNodeTransform m_pivotCLS;
	};
}


#endif