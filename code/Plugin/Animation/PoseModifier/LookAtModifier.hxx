#include "Animation/PoseModifier/LookAtModifier.h"
#include "Animation/SkeletonPose.h"

namespace ma
{
	LookAtModifier::LookAtModifier()
	{
		m_vGoalOS = Vector3::ZERO;
		
		m_nBoneID = Math::InvalidID<BoneIndex>();

		m_vForwardLS = Vector3::ZERO;

		m_vUpLs = Vector3::ZERO;

		m_fGoalDistThreshold  = 0.1f;

		m_fMaxYaw = 0;

		m_bInit = false;
	}

	LookAtModifier::~LookAtModifier()
	{

	}

	void LookAtModifier::RegisterAttribute()
	{
		ACCESSOR_ATTRIBUTE(LookAtModifier, "BoneName", GetBoneName, SetBoneName, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(LookAtModifier, "MaxYaw", GetMaxYaw, SetMaxYaw, float, 0, AM_DEFAULT);
		REF_ACCESSOR_ATTRIBUTE(LookAtModifier, "FowardLS", GetFowardLS, SetFowardLS, Vector3, Vector3::ZERO, AM_DEFAULT);
		REF_ACCESSOR_ATTRIBUTE(LookAtModifier, "UpLS", GetUpLS, SetUpLs, Vector3, Vector3::ZERO, AM_DEFAULT);
	}

	void LookAtModifier::UpdatePose(SkeletonPose* pNodePose,Skeleton* pSkeleton,float fWeight)
	{
		if (!m_bEnable)
			return;
 
		if (!m_bInit)
		{
			Vector3 vSide = m_vUpLs.crossProduct(m_vForwardLS).normalisedCopy();
			Vector3 vUp = m_vForwardLS.crossProduct(vSide).normalisedCopy();
			m_qPivotLS.FromAxes(m_vForwardLS,vSide,vUp);

			m_nBoneID = pSkeleton->GetBoneIdByName(m_strBoneName.c_str());

			m_bInit = true;
		}

		Transform pivotOS;
		pivotOS = pNodePose->GetTransformOS(m_nBoneID);
		pivotOS.m_qRot = pivotOS.m_qRot * m_qPivotLS;

		Transform pivotInvOS;
		TransformInverse(&pivotInvOS,&pivotOS);

		Vector3 vGoalPivotSpace;
		TransformPoint(&vGoalPivotSpace,&m_vGoalOS,&pivotInvOS);
		float fGoalLenPivotSpace = vGoalPivotSpace.length();

		EulerAngleXYZ eOffsetPivotSpace;
		if (fGoalLenPivotSpace > Math::FEPS)
		{
			Vector3 vGoalDirPivotSpace = vGoalPivotSpace / fGoalLenPivotSpace;
			EulerAngleFromXToAxis(&eOffsetPivotSpace,&vGoalDirPivotSpace);

			//fade out look at by goal distance
			if (fGoalLenPivotSpace < m_fGoalDistThreshold)
			{
				eOffsetPivotSpace *= fGoalLenPivotSpace / m_fGoalDistThreshold;
			}

			float fInvalidYaw = fabs(eOffsetPivotSpace.z) - m_fMaxYaw;
			if (fInvalidYaw > 0.0f)
			{
				float fYawFadeRange = Math::PI - m_fMaxYaw;
				if (fYawFadeRange > Math::FEPS)
				{
					float fYawFade = 1.0f - fInvalidYaw / fYawFadeRange;
					eOffsetPivotSpace.z *= Math::Max(fYawFade,0.0f);
				}
			}		
		}
		else
		{
			memset(&eOffsetPivotSpace,0,sizeof(EulerAngleXYZ));
		}

		Transform offsetTSFLS;
		QuaternionFromEulerAngleXYZ(&offsetTSFLS.m_qRot,&eOffsetPivotSpace);
		
		Quaternion qPivotInvLS = m_qPivotLS.Inverse();

		offsetTSFLS.m_qRot = offsetTSFLS.m_qRot * qPivotInvLS;
		offsetTSFLS.m_qRot = m_qPivotLS * offsetTSFLS.m_qRot;

		// È¨ÖØ¼ÆËã
		Math::Lerp(Quaternion::IDENTITY,offsetTSFLS.m_qRot,fWeight);
		Math::Lerp(Vector3::ZERO,offsetTSFLS.m_vPos,fWeight);

		pNodePose->ApplyTransformLS(&offsetTSFLS,m_nBoneID);

	}

	void LookAtModifier::DbgDraw() const
	{
		// 	IDebugRenderer* pdbgr = GetDebugRenderer();
		// 	if (pdbgr)
		// 	{
		// 		pdbgr->DrawBox(&m_vGoalOS,NULL,1.0f,0xffffffff);
		// 		pdbgr->DrawString(m_vGoalOS,"LookAtModifierGoalOS");
		// 	}
	}

	RefPtr<LookAtModifier> CreateLookAtModifier()
	{
		return new LookAtModifier();
	}
}

