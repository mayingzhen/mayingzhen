#include "Animation/PoseModifier/LookAtModifier.h"
#include "Animation/SkeletonPose.h"

namespace ma
{
	IMPL_OBJECT(LookAtModifier,PoseModifier)

	LookAtModifier::LookAtModifier()
		:m_vGoalOS(0.0,1.0,0.0)
		,m_qPivotLS(0.0f,0.0f,0.0f,1.0f)
		,m_nBoneID(InvalidID<UINT>())
		,m_fGoalDistThreshold(1.0f)
		,m_fMaxYaw(PI/2.0f)
	{

	}


	void LookAtModifier::UpdatePose(SkeletonPose* pNodePose)
	{
		if (pNodePose == NULL)
			return;

		if (!m_bEnable)
			return;

		NodeTransform pivotOS;
		NodeTransform pivotInvOS;
		pivotOS = pNodePose->GetTransformOS(m_nBoneID);
		TransformInverse(&pivotInvOS,&pivotOS);

		Vector3 vGoalPivotSpace;
		//TransformPoint(&vGoalPivotSpace,&m_vGoalOS,&pivotInvOS);
		float fGoalLenPivotSpace = Vec3Length(&vGoalPivotSpace);
		Vector3 vGoalDirPivotSpace = vGoalPivotSpace / fGoalLenPivotSpace;
		//maEulerAngleFromXToAxis()





// 		NodeTransform pivotOS;
// 		NodeTransform pivotInvOS;
// 
// 		pivotOS = pNodePose->GetTransformOS(m_nBoneID); //GetBoneTransformOS(m_nBoneID);
// 		pivotOS.m_qRot = m_qPivotLS * pivotOS.m_qRot;
// 		maTransformInverse(&pivotInvOS,&pivotOS);
// 
// 		Vector3 vGoalPivotSpace;
// 		maTransformPoint(&vGoalPivotSpace,&m_vGoalOS,&pivotInvOS);
// 		float fGoalLenPivotSpace = Vec3Length(&vGoalPivotSpace);
// 
// 		EulerAngleXYZ eOffsetPivotSpace;
// 		if (fGoalLenPivotSpace > FEPS)
// 		{
// 			Vector3 vGoalDirPivotSpace = vGoalPivotSpace / fGoalLenPivotSpace;
// 			maEulerAngleFromXToAxis(&eOffsetPivotSpace,&vGoalDirPivotSpace);
// 		}
// 		else
// 		{
// 			memset(&eOffsetPivotSpace,0,sizeof(EulerAngleXYZ));
// 		}
// 
// 		Quaternion qRotLS;
// 		maQuaternionFromEulerAngleXYZ(&qRotLS,&eOffsetPivotSpace);
// 		Quaternion qPivotInvLS;
// 		QuaternionInverse(&qPivotInvLS,&m_qPivotLS);
// 
// 		QuaternionMultiply(&qRotLS,&qPivotInvLS,&qRotLS);
// 		QuaternionMultiply(&qRotLS,&qRotLS,&m_qPivotLS);
// 
// 		// È¨ÖØ¼ÆËã
// 		Quaternion iden;
// 		QuaternionIdentity(&iden);
// 		QuaternionSlerp(&qRotLS,&iden,&qRotLS,m_fGain);
// 			
// 		NodeTransform offsetTSFLS;
// 		TransformSetIdentity(&offsetTSFLS);
// 		offsetTSFLS.m_qRot = qRotLS;
// 
// 		pNodePose->ApplyTransformLS(&offsetTSFLS,m_nBoneID);

	}



	void LookAtModifier::Init(UINT nBoneID,const Vector3& vInFowardLS,const Vector3& vInUpLS)
	{
		m_nBoneID = nBoneID;

		CalculatePovitLs(vInFowardLS, vInUpLS);
	}

	void LookAtModifier::CalculatePovitLs(const Vector3& vFowardLS,const Vector3& vUpLS)
	{
		Matrix4x4 matPovitLS;
		Vector3 vSide;
		Vector3 vUp;
		Vec3Cross(&vSide,&vUpLS,&vFowardLS);
		Vec3Normalize(&vSide,&vSide);
		Vec3Cross(&vUp,&vFowardLS,&vSide);
		MatrixAxis(&matPovitLS,&vFowardLS,&vSide,&vUp);
		QuaternionRotationMatrix(&m_qPivotLS,&matPovitLS);
	}

	Vector3	LookAtModifier::GetFowardLS() const
	{
		Matrix4x4 matOldPPovitLS;
		MatrixRotationQuaternion(&matOldPPovitLS, &m_qPivotLS);
		Vector3 vOldFowardLS = Vector3(matOldPPovitLS._11, matOldPPovitLS._12, matOldPPovitLS._13);
		return vOldFowardLS;
	}
	Vector3	LookAtModifier::GetUpLS() const
	{
		Matrix4x4 matOldPPovitLS;
		MatrixRotationQuaternion(&matOldPPovitLS, &m_qPivotLS);
		Vector3 vOldUpLS = Vector3(matOldPPovitLS._31, matOldPPovitLS._32, matOldPPovitLS._33);
		return vOldUpLS;
	}
	void LookAtModifier::SetBoneID( UINT nBoneID )
	{
		m_nBoneID = nBoneID;
	}

	UINT LookAtModifier::GetBoneID() const
	{
		return m_nBoneID;
	}


	void				LookAtModifier::SetGoalObjectSpace(const Vector3& vGoalOS)
	{
		m_vGoalOS = vGoalOS;
	}


	const Vector3&	LookAtModifier::GetGoalObjectSpace()const
	{
		return m_vGoalOS;
	}

	void				LookAtModifier::SetMaxYaw(float fMaxYaw)
	{
		m_fMaxYaw = Clamp(fMaxYaw,0.0f,PI);
	}

	float				LookAtModifier::GetMaxYaw() const
	{
		return m_fMaxYaw;
	}

	//Matrix4x4 LookAtModifier::ApplyLimit(const Matrix4x4* pBonePS)const
	//{
	//	Matrix4x4 ret;
	//	EulerAngleXYZ eRotPS;
	//	EulerAngleXYZFromMatrix(&eRotPS,pBonePS);
	//	eRotPS.x = Clamp(eRotPS.x,m_vAngleMin.x,m_vAngleMax.x);
	//	eRotPS.y = Clamp(eRotPS.y,m_vAngleMin.y,m_vAngleMax.y);
	//	eRotPS.z = Clamp(eRotPS.z,m_vAngleMin.z,m_vAngleMax.z);
	//	MatrixFromEulerAngleXYZ(&ret,&eRotPS);
	//
	//	*MatrixAsTranslation(&ret) = *MatrixAsTranslation(pBonePS);
	//	return ret;
	//}



	void LookAtModifier::DbgDraw() const
	{
		// 	IDebugRenderer* pdbgr = GetDebugRenderer();
		// 	if (pdbgr)
		// 	{
		// 		pdbgr->DrawBox(&m_vGoalOS,NULL,1.0f,0xffffffff);
		// 		pdbgr->DrawString(m_vGoalOS,"LookAtModifierGoalOS");
		// 	}
	}
}

