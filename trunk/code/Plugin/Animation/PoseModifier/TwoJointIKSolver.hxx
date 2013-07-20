#include "Animation/PoseModifier/TwoJointIKSolver.h"

namespace ma
{
// 	maTransformXZAxis(NodeTransform* pTSF,D3DVECTOR* pXAxis,D3DVECTOR* pZAxis)
// 	{
// 		Matrix4x4 matAxis;
// 		MatrixIdentity(&matAxis);
// 		Vector3& vXAxis = *maMatrixAsVector3(&matAxis,0);
// 		Vector3& vYAxis = *maMatrixAsVector3(&matAxis,1);
// 		Vector3* vZAxis = *maMatrixAsVector3(&matAxis,2);
// 
// 		Vec3Cross(&vYAxis,pZAxis,pXAxis);
// 		Vec3Normalize(&vYAxis,&vYAxis);
// 		Vec3Cross(&vZAxis,pXAxis,&vYAxis);
// 		vXAxis = *pXAxis;
// 
// 		maTransformFromMatrix(pTSF,&matAxis);
// 	}

// 	void TwoJointIKSolver::Init(BoneIndex uBoneAInd,BoneIndex uBoneBInd,BoneIndex uBoneCInd,
// 		const D3DVECTOR& vHingAxisLs,const Skeleton* pResfSkel)
// 	{
// 		m_uBoneAID = uBoneAInd;
// 		m_uBoneBID = uBoneBInd;
// 		m_uBoneCID = uBoneCInd;
// 		m_vHingAxisLS = vHingAxisLs;
// 		m_pResfSkel = pResfSkel;
// 
// 		const SkeletonPose* pRefPose = pResfSkel->GetResPose();
// 		Vector3 vBoneAToBoneBOS = pRefPose->GetTransformOS(uBoneBInd).m_vPos - pRefPose->GetTransformOS(uBoneAInd).m_vPos;
// 		Vector3 vBoneBToBoneCOS = pRefPose->GetTransformOS(uBoneCInd).m_vPos - pRefPose->GetTransformOS(uBoneBInd).m_vPos;
// 		Vec3Normalize(&vBoneAToBoneBOS,&vBoneAToBoneBOS);
// 		Vec3Normalize(&vBoneBToBoneCOS,&vBoneBToBoneCOS);
// 		Vector3 vHingAxisOS;
// 		Vec3Cross(&vHingAxisOS,&vBoneAToBoneBOS,&vBoneBToBoneCOS);
// 
// 		NodeTransform pivotAOS,pivotBOS,pivotCOS;
// 		NodeTransform boneAInvOS,boneBInvOS,boneCInvOS;
// 
// 		maTransformXZAxis(&pivotAOS,&vBoneAToBoneBOS,&vHingAxisOS);
// 		pivotAOS.m_vPos = pRefPose->GetTransformOS(uBoneAInd).m_vPos;
// 
// 		maTransformXZAxis(&pivotBOS,&vBoneBToBoneCOS,&vHingAxisOS);
// 		pivotBOS.m_vPos = pRefPose->GetTransformOS(uBoneBInd).m_vPos;
// 
// 		pivotCOS.m_qRot = pivotBOS.m_qRot;
// 		pivotCOS.m_vPos = pRefPose->GetTransformOS(uBoneCInd).m_vPos;
// 
// 		maTransformInverse(&boneAInvOS,&pRefPose->GetTransformOS(uBoneAInd));
// 		maTransformMul(&m_pivotALS,&pivotAOS,&boneAInvOS);
// 
// 		maTransformInverse(&boneBInvOS,&pRefPose->GetTransformOS(uBoneBInd));
// 		maTransformMul(&m_pivotBLS,&pivotBOS,&boneBInvOS);
// 
// 		maTransformInverse(&boneCInvOS,&pRefPose->GetTransformOS(uBoneCInd));
// 		maTransformMul(&m_pivotCLS,&pivotCOS,&boneCInvOS);
// 	}

//	void TwoJointIKSolver::CalculateConstrainPlane(Quaternion& qConstrainPlanePitchLS)
// 	{
// 		const SkeletonPose* pRefPose = m_pResfSkel->GetResPose();
// 
// 		bool bGoalAlignJointA = false;
// 		Vector3 vGoalPivotSpace;
// 		float fGoalDistPivotSpace;
// 		
// 		NodeTransform pivotATSFOS;
// 		NodeTransform pivotATSFInvOS;
// 		maTransformMul(&pivotATSFOS,&m_pivotALS,&pRefPose->GetTransformOS(m_uBoneAID));
// 		maTransformInverse(&pivotATSFInvOS,&pivotATSFOS);
// 		maTransformPoint(&vGoalPivotSpace,&m_vGoalOS,&pivotATSFInvOS);
// 		fGoalDistPivotSpace = Vec3Length(&vGoalPivotSpace);
// 		if (fGoalDistPivotSpace > FEPS)
// 		{
// 			Vector3 vGoalDirPivotSpace = vGoalPivotSpace / fGoalDistPivotSpace;
// 			Vector3 vGoalDirXY(vGoalDirPivotSpace.x,vGoalDirPivotSpace.y,0.0f);
// 			Vector3 fGoalDirXYLen = Vec3Length(&vGoalDirXY);
// 			if (fGoalDirXYLen > FEPS)
// 			{
// 				vGoalDirXY = vGoalDirXY / fGoalDirXYLen;
// 				maQuaternionFromAxisToAxis(qConstrainPlanePitchLS,&vGoalDirXY,&vGoalDirPivotSpace);
// 			}
// 			else
// 			{
// 				*qConstrainPlanePitchLS = vGoalDirPivotSpace.y > 0.0f ?
// 					Quaternion(0.0f, 0.0f, sin(-_PI / 1.0f), cos(-_PI / 4.0f)) :
// 					Quaternion(0.0f, 0.0f, sin(_PI / 1.0f), cos(_PI / 4.0f));
// 			}
// 		}
// 		else
// 		{
// 			bGoalAlignJointA = true;
// 			QuaternionIdentity(qConstrainPlanePitchLS);
// 		}
// 
// 		return bGoalAlignJointA;
// 	}

// 	void SolveConstrainPlane(
// 		NodeTransform* pJointATSF,NodeTransform* pJointBTSF,NodeTransform* pJointCTSF,
// 		const Vector3* pGoal,
// 		float fLinkALength, float fLinkBLength,
// 		float fJointAYawMax,float fJointAYawMin,
// 		float fJointBYawMinLS,  // //related to joint a
// 		const Vector3* pLinkADefaultDir)
// 
// 	{
// 		float fGoalDist = Vec3Length(pGoal);
// 		Vector3 vGoalDir;
// 		if (fGoalDist > FEPS)
// 		{
// 			vGoalDir = *pGoal / fGoalDist;
// 		}
// 		else
// 		{
// 			vGoalDir = pLinkADefaultDir? *pLinkADefaultDir : Vector3(1.0f,0.0f,0.0f);
// 		}
// 
// 
// 		Vector3 vXAxis = Vector3(1.0f,0.0f,0.0f);
// 		Vector3 vZAxis = Vector3(0.0f,0.0f,1.0f);
// 		
// 		Vector3 vRegionADir = Vector3(cos(fJointAYawMax),sin(fJointAYawMax),0.0f);
// 		Vector3 vRegionBDir = Vector3(cos(fJointAYawMin),sin(fJointAYawMin),0.0f);
// 		Vector3 vRegionACenter = vRegionADir * fLinkALength;
// 		Vector3 vRegionBCenter = vRegionBDir * fLinkALength;
// 		Vector3 vAP = *pGoal - vRegionACenter;
// 
// 		bool bIsInRegionA = false;
// 		float fAPlen = Vec3Length(&vAP);
// 		if (fAPlen < fLinkBLength)
// 		{
// 			Vector3 vOBPerp(vRegionACenter.y,-vRegionACenter.x,0.0f);
// 			bIsInRegionA = Vec3Dot(&vOBPerp,&vAP) > 0.0f;
// 		}
// 
// 		if (bIsInRegionA)
// 		{
// 			Vector3 vAPDir;
// 		}
// 
// 	}

// 	void TwoJointIKSolver::UpdatePose(SkeletonPose* pNodePose)
// 	{
// 		if (pNodePose == NULL)
// 			return;
// 
// 		if (!m_bEnable)
// 			return;
// 
// 		Quaternion qConstrainPlanePitchLS;
// 		CalculateConstrainPlane(qConstrainPlanePitchLS,);
// 
// 		NodeTransform newPivotTSFOS, newPivotTSFInvOS;
// 		maTransformMul(&newPivotTSFOS,&m_pivotALS,pNodePose->GetTransformOS(m_uBoneAID));
// 		newPivotTSFOS.m_qRot = qConstrainPlanePitchLS * newPivotTSFOS.m_qRot;
// 		maTransformInverse(*newPivotTSFInvOS,&newPivotTSFOS);
// 
// 		// object space to solver space
// 		Vector3 vGoalNPS, vLinkANPS;
// 		Vector3 vLinkAOS = pNodePose->GetTransformOS(m_uBoneBID).m_vPos - pNodePose->GetTransformOS(m_uBoneAID).m_vPos;
// 		Vector3 vLinkBOS = pNodePose->GetTransformOS(m_uBoneCID).m_vPos - pNodePose->GetTransformOS(m_uBoneBID).m_vPos;
// 		float fLinkALength = Vec3Length(&vLinkAOS);
// 		float fLinkBLength = Vec3Length(&vLinkBOS);
// 		maTransformVector(&vLinkANPS,&vLinkAOS,&newPivotTSFInvOS);
// 		maTransformPoint(&vGoalNPS,&m_vGoalOS,&newPivotTSFInvOS);
// 
// 		NodeTransform jointANewTSFNPS,jointBNewTSFNPS,jointCNewTSFNPS;
// 		SolveConstrainPlane()
// 
// 
// 	}

	// the destructor
	void TwoJointIKSolver::Update(const float timeDeltaInSeconds)
	{
		// get the worldspace positions of the three nodes
		const Vector3 posA = mStartNode->GetWorldPos();
		const Vector3 posB = mMidNode->GetWorldPos();
		const Vector3 posC = mEndNode->GetWorldPos();

		// get the goal we want (depending on if we want to use a node as goal or not)
		Vector3 goal;
		if (mGoalNode)
			goal = mGoalNode->GetWorldPos();
		else
			goal = mGoal;

		// solve the IK problem by calculating the new position of the mid node in worldspace
		Vector3 midPos;
		mHasSolution = TwoJointIKSolver::Solve2LinkIK(posA, posB, posC, goal, mBendDirection, &midPos);

		//----------------------------------------
		// calculate the first bone orientation
		//----------------------------------------
		Vector3 forward, up, right, scale;
		Matrix mat(true);

		// if start, mid and end node aren't the same
		if (mStartNode != mMidNode && mMidNode != mEndNode)
		{
			// calculate the matrix orientation
			forward = (midPos - posA).Normalize();
			up		= mStartNode->GetWorldTM().GetUp().Normalize();
			right	= forward.Cross( up ).Normalize();
			up		= right.Cross(forward).Normalize();
			scale	= mStartNode->GetWorldScale();

			// calculate the new matrix
			mat.SetRight( forward );
			mat.SetUp( up );
			mat.SetForward( right );
			mat.Scale(scale.x, scale.y, scale.z);
			mat.SetTranslation( mStartNode->GetWorldPos() );

			// recursively update the bone matrices (forward kinematics)
			mStartNode->RecursiveUpdateWorldTM( &mat );
		}
		else
			midPos = mMidNode->GetWorldPos();

		//----------------------------------------
		// calculate the second bone orientation
		//----------------------------------------
		// get the normalized matrix of the mid node
		Matrix midMat = mMidNode->GetWorldTM().Normalized();

		// calculate the new direction vectors
		forward = (goal - midPos).Normalize();
		up		= midMat.GetUp().Normalize();
		right	= forward.Cross( up ).Normalize();
		up		= right.Cross(forward).Normalize();
		scale	= mMidNode->GetWorldScale();

		// build the matrix with the new orientation and scale
		mat.SetRight( forward );
		mat.SetUp( up );
		mat.SetForward( right );
		mat.Scale(scale.x, scale.y, scale.z);
		mat.SetTranslation( midMat.GetTranslation() );

		// recursively update all child matrices as well (forward kinematics)
		mMidNode->RecursiveUpdateWorldTM( &mat );
	}

	bool TwoJointIKSolver::Solve2LinkIK(const Vector3& posA, const Vector3& posB, const Vector3& posC,
		const Vector3& goal, const Vector3& bendDir, Vector3* outMidPos)
	{
		const Vector3 localGoal = goal - posA;

		// get the lengths of the bones A and B
		//const float lengthA = (posB - posA).Length();
		//const float lengthB = (posC - posB).Length();
		Vector3 vTempAB(posB - posA); 
		Vector3 vTempBC(posC - posB);
		const float lengthA = Vec3Length(&vTempAB);
		const float lengthB = Vec3Length(&vTempBC);

		// calculate the matrix that rotates from IK solve space into world space
		Matrix4x4 matForward;
		TwoJointIKSolver::CalculateMatrix(localGoal, bendDir, &matForward);
		//Matrix matInverse = matForward.Transposed();
		Matrix matInverse;
		MatrixTranspose(&matInverse,&matForward);

		// calculate the goal position in IK solve space
		//const Vector3 R = matInverse.Mul3x3( localGoal );
		const Vector3 R = matInverse * localGoal;

		// calculate the d and e values from the equations by Ken Perlin
		//const float rLen = R.Length();
		const float rLen = Vec3Length(&R);
		const float l = (rLen + (lengthA*lengthA - lengthB*lengthB) / rLen) * 0.5f;
		const float d = Max<float>(0.0f, Min<float>(lengthA, l));
		const float e = sqrtf(lengthA*lengthA - d*d);

		// the solution on the YZ plane
		Vector3 solution(d, e, 0);

		// rotate the solution (the mid "knee/elbow" position) into world space
		//*outMidPos = posA + matForward.Mul3x3( solution );
		*outMidPos = posA + matForward * solution;

		// check if we found a solution or not
		return (d > 0 && d < lengthA);
	}
}

