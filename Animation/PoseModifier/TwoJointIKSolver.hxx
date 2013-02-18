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
		fGoalDistPivotSpace = D3DXVec3Length(&vGoalPivotSpace);
		if (fGoalDistPivotSpace > F_EPS)
		{
			D3DXVECTOR3 vGoalDirPivotSpace = vGoalPivotSpace / fGoalDistPivotSpace;
			D3DXVECTOR3 vGoalDirXY(vGoalDirPivotSpace.x,vGoalDirPivotSpace.y,0.0f);
			D3DXVECTOR3 fGoalDirXYLen = D3DXVec3Length(&vGoalDirXY);
			if (fGoalDirXYLen > F_EPS)
			{
				vGoalDirXY = vGoalDirXY / fGoalDirXYLen;
				maQuaternionFromAxisToAxis(qConstrainPlanePitchLS,&vGoalDirXY,&vGoalDirPivotSpace);
			}
			else
			{
				*qConstrainPlanePitchLS = vGoalDirPivotSpace.y > 0.0f ?
					D3DXQUATERNION(0.0f, 0.0f, sin(-D3DX_PI / 1.0f), cos(-D3DX_PI / 4.0f)) :
					D3DXQUATERNION(0.0f, 0.0f, sin(D3DX_PI / 1.0f), cos(D3DX_PI / 4.0f));
			}
		}
		else
		{
			bGoalAlignJointA = true;
			D3DXQuaternionIdentity(qConstrainPlanePitchLS);
		}

		return bGoalAlignJointA;
	}

	void SolveConstrainPlane(
		maNodeTransform* pJointATSF,maNodeTransform* pJointBTSF,maNodeTransform* pJointCTSF,
		const D3DXVECTOR3* pGoal,
		float fLinkALength, float fLinkBLength,
		float fJointAYawMax,float fJointAYawMin,
		float fJointBYawMinLS,  // //related to joint a
		const D3DXVECTOR3* pLinkADefaultDir)

	{
		float fGoalDist = D3DXVec3Length(pGoal);
		D3DXVECTOR3 vGoalDir;
		if (fGoalDist > F_EPS)
		{
			vGoalDir = *pGoal / fGoalDist;
		}
		else
		{
			vGoalDir = pLinkADefaultDir? *pLinkADefaultDir : D3DXVECTOR3(1.0f,0.0f,0.0f);
		}


		D3DXVECTOR3 vXAxis = D3DXVECTOR3(1.0f,0.0f,0.0f);
		D3DXVECTOR3 vZAxis = D3DXVECTOR3(0.0f,0.0f,1.0f);
		
		D3DXVECTOR3 vRegionADir = D3DXVECTOR3(cos(fJointAYawMax),sin(fJointAYawMax),0.0f);
		D3DXVECTOR3 vRegionBDir = D3DXVECTOR3(cos(fJointAYawMin),sin(fJointAYawMin),0.0f);
		D3DXVECTOR3 vRegionACenter = vRegionADir * fLinkALength;
		D3DXVECTOR3 vRegionBCenter = vRegionBDir * fLinkALength;
		D3DXVECTOR3 vAP = *pGoal - vRegionACenter;

		bool bIsInRegionA = false;
		float fAPlen = D3DXVec3Length(&vAP);
		if (fAPlen < fLinkBLength)
		{
			D3DXVECTOR3 vOBPerp(vRegionACenter.y,-vRegionACenter.x,0.0f);
			bIsInRegionA = D3DXVec3Dot(&vOBPerp,&vAP) > 0.0f;
		}

		if (bIsInRegionA)
		{
			D3DXVECTOR3 vAPDir;
		}

	}

	void TwoJointIKSolver::UpdatePose(NodePose* pNodePose)
	{
		if (pNodePose == NULL)
			return;

		if (!m_bEnable)
			return;

		D3DXQUATERNION qConstrainPlanePitchLS;
		CalculateConstrainPlane(qConstrainPlanePitchLS,);

		maNodeTransform newPivotTSFOS, newPivotTSFInvOS;
		maTransfromMul(&newPivotTSFOS,&m_pivotALS,pNodePose->GetTransformOS(m_uBoneAID));
		newPivotTSFOS.m_qRot = qConstrainPlanePitchLS * newPivotTSFOS.m_qRot;
		maTransformInverse(*newPivotTSFInvOS,&newPivotTSFOS);

		// object space to solver space
		D3DXVECTOR3 vGoalNPS, vLinkANPS;
		D3DXVECTOR3 vLinkAOS = pNodePose->GetTransformOS(m_uBoneBID).m_vPos - pNodePose->GetTransformOS(m_uBoneAID).m_vPos;
		D3DXVECTOR3 vLinkBOS = pNodePose->GetTransformOS(m_uBoneCID).m_vPos - pNodePose->GetTransformOS(m_uBoneBID).m_vPos;
		float fLinkALength = D3DXVec3Length(&vLinkAOS);
		float fLinkBLength = D3DXVec3Length(&vLinkBOS);
		maTransformVector(&vLinkANPS,&vLinkAOS,&newPivotTSFInvOS);
		maTransformPoint(&vGoalNPS,&m_vGoalOS,&newPivotTSFInvOS);

		maNodeTransform jointANewTSFNPS,jointBNewTSFNPS,jointCNewTSFNPS;
		SolveConstrainPlane()


	}
}

