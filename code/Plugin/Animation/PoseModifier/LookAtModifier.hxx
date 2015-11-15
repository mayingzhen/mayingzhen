#include "Animation/PoseModifier/LookAtModifier.h"
#include "Animation/SkeletonPose.h"

namespace ma
{
	LookAtModifier::LookAtModifier()
		:m_vGoalOS(0.0,1.0,0.0)
		,m_qPivotLS(0.0f,0.0f,0.0f,1.0f)
		,m_nBoneID(Math::InvalidID<BoneIndex>())
		,m_fGoalDistThreshold(1.0f)
		,m_fMaxYaw(Math::HALF_PI)
	{

	}


	void LookAtModifier::UpdatePose(SkeletonPose* pNodePose)
	{
		if (pNodePose == NULL)
			return;

		if (!m_bEnable)
			return;

		Transform pivotOS;
		Transform pivotInvOS;
		pivotOS = pNodePose->GetTransformOS(m_nBoneID);
		TransformInverse(&pivotInvOS,&pivotOS);

		Vector3 vGoalPivotSpace;
		//TransformPoint(&vGoalPivotSpace,&m_vGoalOS,&pivotInvOS);
		float fGoalLenPivotSpace = vGoalPivotSpace.length();
		Vector3 vGoalDirPivotSpace = vGoalPivotSpace / fGoalLenPivotSpace;
		//maEulerAngleFromXToAxis()





// 		Transform pivotOS;
// 		Transform pivotInvOS;
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
// 		Transform offsetTSFLS;
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
		Vector3 vFoward = vFowardLS.normalisedCopy();
		Vector3 vSide = vUpLS.crossProduct(vFoward).normalisedCopy();
		Vector3 vUp = vFowardLS.crossProduct(vSide).normalisedCopy();
		m_qPivotLS.FromAxes(vSide,vFoward,vUp);
	}

	Vector3	LookAtModifier::GetFowardLS() const
	{
		return m_qPivotLS.yAxis();
	}
	Vector3	LookAtModifier::GetUpLS() const
	{
		return m_qPivotLS.zAxis();
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
		m_fMaxYaw = Math::Clamp(fMaxYaw,0.0f,Math::PI);
	}

	float				LookAtModifier::GetMaxYaw() const
	{
		return m_fMaxYaw;
	}

	//Matrix4 LookAtModifier::ApplyLimit(const Matrix4* pBonePS)const
	//{
	//	Matrix4 ret;
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

