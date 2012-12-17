#include "Animation/PoseModifier/TwoJointIKSolver.h"

namespace ma
{
	maTransformXZAxis(maNodeTransform* pTSF,D3DVECTOR* pXAxis,D3DVECTOR* pZAxis)
	{
		D3DXMATRIX matAxis;
		D3DXMatrixIdentity(&matAxis);
		D3DXVECTOR3& vXAxis = *maMatrixAsVector3(&matAxis,0);
		D3DXVECTOR3& vYAxis = *maMatrixAsVector3(&matAxis,1);
		D3DXVECTOR3* vZAxis = *maMatrixAsVector3(&matAxis,2);

		D3DXVec3Cross(&vYAxis,pZAxis,pXAxis);
		D3DXVec3Normalize(&vYAxis,&vYAxis);
		D3DXVec3Cross(&vZAxis,pXAxis,&vYAxis);
		vXAxis = *pXAxis;

		maTransformFromMatrix(pTSF,&matAxis);
	}

	void TwoJointIKSolver::Init(BoneIndex uBoneAInd,BoneIndex uBoneBInd,BoneIndex uBoneCInd,
		const D3DVECTOR& vHingAxisLs,const Skeleton* pResfSkel)
	{
		m_uBoneAID = uBoneAInd;
		m_uBoneBID = uBoneBInd;
		m_uBoneCID = uBoneCInd;
		m_vHingAxisLS = vHingAxisLs;
		m_pResfSkel = pResfSkel;

		const NodePose* pRefPose = pResfSkel->GetResPose();
		D3DXVECTOR3 vBoneAToBoneBOS = pRefPose->GetTransformOS(uBoneBInd).m_vPos - pRefPose->GetTransformOS(uBoneAInd).m_vPos;
		D3DXVECTOR3 vBoneBToBoneCOS = pRefPose->GetTransformOS(uBoneCInd).m_vPos - pRefPose->GetTransformOS(uBoneBInd).m_vPos;
		D3DXVec3Normalize(&vBoneAToBoneBOS,&vBoneAToBoneBOS);
		D3DXVec3Normalize(&vBoneBToBoneCOS,&vBoneBToBoneCOS);
		D3DXVECTOR3 vHingAxisOS;
		D3DXVec3Cross(&vHingAxisOS,&vBoneAToBoneBOS,&vBoneBToBoneCOS);

		maNodeTransform pivotAOS,pivotBOS,pivotCOS;
		maNodeTransform boneAInvOS,boneBInvOS,boneCInvOS;

		maTransformXZAxis(&pivotAOS,&vBoneAToBoneBOS,&vHingAxisOS);
		pivotAOS.m_vPos = pRefPose->GetTransformOS(uBoneAInd).m_vPos;

		maTransformXZAxis(&pivotBOS,&vBoneBToBoneCOS,&vHingAxisOS);
		pivotBOS.m_vPos = pRefPose->GetTransformOS(uBoneBInd).m_vPos;

		pivotCOS.m_qRot = pivotBOS.m_qRot;
		pivotCOS.m_vPos = pRefPose->GetTransformOS(uBoneCInd).m_vPos;

		maTransformInverse(&boneAInvOS,&pRefPose->GetTransformOS(uBoneAInd));
		maTransfromMul(&m_pivotALS,&pivotAOS,&boneAInvOS);

		maTransformInverse(&boneBInvOS,&pRefPose->GetTransformOS(uBoneBInd));
		maTransfromMul(&m_pivotBLS,&pivotBOS,&boneBInvOS);

		maTransformInverse(&boneCInvOS,&pRefPose->GetTransformOS(uBoneCInd));
		maTransfromMul(&m_pivotCLS,&pivotCOS,&boneCInvOS);
	}

	void TwoJointIKSolver::CalculateConstrainPlane(D3DXQUATERNION& qConstrainPlanePitchLS)
	{
		const NodePose* pRefPose = m_pResfSkel->GetResPose();

		bool bGoalAlignJointA = false;
		D3DXVECTOR3 vGoalPivotSpace;
		float fGoalDistPivotSpace;
		
		maNodeTransform pivotATSFOS;
		maNodeTransform pivotATSFInvOS;
		maTransfromMul(&pivotATSFOS,&m_pivotALS,&pRefPose->GetTransformOS(m_uBoneAID));
		maTransformInverse(&pivotATSFInvOS,&pivotATSFOS);
		maTransformPoint(&vGoalPivotSpace,&m_vGoalOS,&pivotATSFInvOS);

	}

	void TwoJointIKSolver::UpdatePose(NodePose* pNodePose)
	{
		if (pNodePose == NULL)
			return;

		if (!m_bEnable)
			return;

		D3DXQUATERNION qConstrainPlanePitchLS;
		CalculateConstrainPlane(qConstrainPlanePitchLS);
	}
}

