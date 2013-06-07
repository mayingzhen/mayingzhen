#include "Animation/PoseModifier/TwoJointIKSolver.h"

namespace ma
{
	maTransformXZAxis(NodeTransform* pTSF,D3DVECTOR* pXAxis,D3DVECTOR* pZAxis)
	{
		Matrix4x4 matAxis;
		MatrixIdentity(&matAxis);
		Vector3& vXAxis = *maMatrixAsVector3(&matAxis,0);
		Vector3& vYAxis = *maMatrixAsVector3(&matAxis,1);
		Vector3* vZAxis = *maMatrixAsVector3(&matAxis,2);

		Vec3Cross(&vYAxis,pZAxis,pXAxis);
		Vec3Normalize(&vYAxis,&vYAxis);
		Vec3Cross(&vZAxis,pXAxis,&vYAxis);
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

		const SkeletonPose* pRefPose = pResfSkel->GetResPose();
		Vector3 vBoneAToBoneBOS = pRefPose->GetTransformOS(uBoneBInd).m_vPos - pRefPose->GetTransformOS(uBoneAInd).m_vPos;
		Vector3 vBoneBToBoneCOS = pRefPose->GetTransformOS(uBoneCInd).m_vPos - pRefPose->GetTransformOS(uBoneBInd).m_vPos;
		Vec3Normalize(&vBoneAToBoneBOS,&vBoneAToBoneBOS);
		Vec3Normalize(&vBoneBToBoneCOS,&vBoneBToBoneCOS);
		Vector3 vHingAxisOS;
		Vec3Cross(&vHingAxisOS,&vBoneAToBoneBOS,&vBoneBToBoneCOS);

		NodeTransform pivotAOS,pivotBOS,pivotCOS;
		NodeTransform boneAInvOS,boneBInvOS,boneCInvOS;

		maTransformXZAxis(&pivotAOS,&vBoneAToBoneBOS,&vHingAxisOS);
		pivotAOS.m_vPos = pRefPose->GetTransformOS(uBoneAInd).m_vPos;

		maTransformXZAxis(&pivotBOS,&vBoneBToBoneCOS,&vHingAxisOS);
		pivotBOS.m_vPos = pRefPose->GetTransformOS(uBoneBInd).m_vPos;

		pivotCOS.m_qRot = pivotBOS.m_qRot;
		pivotCOS.m_vPos = pRefPose->GetTransformOS(uBoneCInd).m_vPos;

		maTransformInverse(&boneAInvOS,&pRefPose->GetTransformOS(uBoneAInd));
		maTransformMul(&m_pivotALS,&pivotAOS,&boneAInvOS);

		maTransformInverse(&boneBInvOS,&pRefPose->GetTransformOS(uBoneBInd));
		maTransformMul(&m_pivotBLS,&pivotBOS,&boneBInvOS);

		maTransformInverse(&boneCInvOS,&pRefPose->GetTransformOS(uBoneCInd));
		maTransformMul(&m_pivotCLS,&pivotCOS,&boneCInvOS);
	}

	void TwoJointIKSolver::CalculateConstrainPlane(Quaternion& qConstrainPlanePitchLS)
	{
		const SkeletonPose* pRefPose = m_pResfSkel->GetResPose();

		bool bGoalAlignJointA = false;
		Vector3 vGoalPivotSpace;
		float fGoalDistPivotSpace;
		
		NodeTransform pivotATSFOS;
		NodeTransform pivotATSFInvOS;
		maTransformMul(&pivotATSFOS,&m_pivotALS,&pRefPose->GetTransformOS(m_uBoneAID));
		maTransformInverse(&pivotATSFInvOS,&pivotATSFOS);
		maTransformPoint(&vGoalPivotSpace,&m_vGoalOS,&pivotATSFInvOS);
		fGoalDistPivotSpace = Vec3Length(&vGoalPivotSpace);
		if (fGoalDistPivotSpace > FEPS)
		{
			Vector3 vGoalDirPivotSpace = vGoalPivotSpace / fGoalDistPivotSpace;
			Vector3 vGoalDirXY(vGoalDirPivotSpace.x,vGoalDirPivotSpace.y,0.0f);
			Vector3 fGoalDirXYLen = Vec3Length(&vGoalDirXY);
			if (fGoalDirXYLen > FEPS)
			{
				vGoalDirXY = vGoalDirXY / fGoalDirXYLen;
				maQuaternionFromAxisToAxis(qConstrainPlanePitchLS,&vGoalDirXY,&vGoalDirPivotSpace);
			}
			else
			{
				*qConstrainPlanePitchLS = vGoalDirPivotSpace.y > 0.0f ?
					Quaternion(0.0f, 0.0f, sin(-_PI / 1.0f), cos(-_PI / 4.0f)) :
					Quaternion(0.0f, 0.0f, sin(_PI / 1.0f), cos(_PI / 4.0f));
			}
		}
		else
		{
			bGoalAlignJointA = true;
			QuaternionIdentity(qConstrainPlanePitchLS);
		}

		return bGoalAlignJointA;
	}

	void SolveConstrainPlane(
		NodeTransform* pJointATSF,NodeTransform* pJointBTSF,NodeTransform* pJointCTSF,
		const Vector3* pGoal,
		float fLinkALength, float fLinkBLength,
		float fJointAYawMax,float fJointAYawMin,
		float fJointBYawMinLS,  // //related to joint a
		const Vector3* pLinkADefaultDir)

	{
		float fGoalDist = Vec3Length(pGoal);
		Vector3 vGoalDir;
		if (fGoalDist > FEPS)
		{
			vGoalDir = *pGoal / fGoalDist;
		}
		else
		{
			vGoalDir = pLinkADefaultDir? *pLinkADefaultDir : Vector3(1.0f,0.0f,0.0f);
		}


		Vector3 vXAxis = Vector3(1.0f,0.0f,0.0f);
		Vector3 vZAxis = Vector3(0.0f,0.0f,1.0f);
		
		Vector3 vRegionADir = Vector3(cos(fJointAYawMax),sin(fJointAYawMax),0.0f);
		Vector3 vRegionBDir = Vector3(cos(fJointAYawMin),sin(fJointAYawMin),0.0f);
		Vector3 vRegionACenter = vRegionADir * fLinkALength;
		Vector3 vRegionBCenter = vRegionBDir * fLinkALength;
		Vector3 vAP = *pGoal - vRegionACenter;

		bool bIsInRegionA = false;
		float fAPlen = Vec3Length(&vAP);
		if (fAPlen < fLinkBLength)
		{
			Vector3 vOBPerp(vRegionACenter.y,-vRegionACenter.x,0.0f);
			bIsInRegionA = Vec3Dot(&vOBPerp,&vAP) > 0.0f;
		}

		if (bIsInRegionA)
		{
			Vector3 vAPDir;
		}

	}

	void TwoJointIKSolver::UpdatePose(SkeletonPose* pNodePose)
	{
		if (pNodePose == NULL)
			return;

		if (!m_bEnable)
			return;

		Quaternion qConstrainPlanePitchLS;
		CalculateConstrainPlane(qConstrainPlanePitchLS,);

		NodeTransform newPivotTSFOS, newPivotTSFInvOS;
		maTransformMul(&newPivotTSFOS,&m_pivotALS,pNodePose->GetTransformOS(m_uBoneAID));
		newPivotTSFOS.m_qRot = qConstrainPlanePitchLS * newPivotTSFOS.m_qRot;
		maTransformInverse(*newPivotTSFInvOS,&newPivotTSFOS);

		// object space to solver space
		Vector3 vGoalNPS, vLinkANPS;
		Vector3 vLinkAOS = pNodePose->GetTransformOS(m_uBoneBID).m_vPos - pNodePose->GetTransformOS(m_uBoneAID).m_vPos;
		Vector3 vLinkBOS = pNodePose->GetTransformOS(m_uBoneCID).m_vPos - pNodePose->GetTransformOS(m_uBoneBID).m_vPos;
		float fLinkALength = Vec3Length(&vLinkAOS);
		float fLinkBLength = Vec3Length(&vLinkBOS);
		maTransformVector(&vLinkANPS,&vLinkAOS,&newPivotTSFInvOS);
		maTransformPoint(&vGoalNPS,&m_vGoalOS,&newPivotTSFInvOS);

		NodeTransform jointANewTSFNPS,jointBNewTSFNPS,jointCNewTSFNPS;
		SolveConstrainPlane()


	}
}

