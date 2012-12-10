#include <Animation/PoseModifier/LookAtModifier.h>


LookAtModifier::LookAtModifier()
:m_vGoalOS(0.0,1.0,0.0)
,m_qPivotLS(0.0f,0.0f,0.0f,1.0f)
,m_nBoneID(InvalidID<xmUint>())
,m_bEnable(true)
,m_fGoalDistThreshold(1.0f)
,m_fMaxYaw(D3DX_PI/2.0f)
,m_fGain(1.0f)
{

}


void LookAtModifier::UpdatePose(NodePose* pNodePose,const PoseUpdateInfo& updateInfo)
{
	//S3ANodePose* pNodePose = reinterpret_cast<S3ANodePose*>(pInNodePose);
	//S3ARigidPose* pRigidPose = reinterpret_cast<S3ARigidPose*>(pInRigidPose);

	if (!m_bEnable)
		return;

	xmRigidTransform pivotOS;
	xmRigidTransform pivotInvOS;

	pivotOS = pRigidPose->GetBoneTransformOS(m_nBoneID);
	pivotOS.m_qRot = m_qPivotLS * pivotOS.m_qRot;
	xmTransformInverse(&pivotInvOS,&pivotOS);

#ifdef _DEBUG
	if(NULL != S3AGetDebugRenderer())
	{ 
		S3AGetDebugRenderer()->DrawTransform(&pivotOS,1.0f,20.0f);
	}
#endif

	xmVector3 vGoalPivotSpace;
	xmTransformPoint(&vGoalPivotSpace,&m_vGoalOS,&pivotInvOS);
	float fGoalLenPivotSpace = xmVec3Length(&vGoalPivotSpace);



	xmEulerAngleXYZ eOffsetPivotSpace;
	if (fGoalLenPivotSpace > xm_EPS)
	{
		xmVector3 vGoalDirPivotSpace = vGoalPivotSpace / fGoalLenPivotSpace;
		xmEulerAngleFromXToAxis(&eOffsetPivotSpace,&vGoalDirPivotSpace);

		//fade out look at by goal distance
		if (fGoalLenPivotSpace < m_fGoalDistThreshold)
		{
			eOffsetPivotSpace *= fGoalLenPivotSpace/m_fGoalDistThreshold;
		}

		{
			xmVector3 vPivotToGoalOS;
			xmTransformVector(&vPivotToGoalOS,&vGoalPivotSpace,&pivotOS);
			S3AGetDebugRenderer()->DrawVector(pivotOS.m_vPos,vPivotToGoalOS,0xffffff);
		}


		//fade out look at by yaw
		{
			float fInvalidYaw = abs(eOffsetPivotSpace.z) - m_fMaxYaw;
			if (fInvalidYaw > 0.0f)
			{
				float fYawFadeRange = xm_PI - m_fMaxYaw;
				if (fYawFadeRange > xm_EPS)
				{
					float fYawFade = 1.0f - fInvalidYaw / fYawFadeRange;
					eOffsetPivotSpace.z *= xmMax(fYawFade,0.0f);
				}
			}				
		}



	}else{
		memset(&eOffsetPivotSpace,0,sizeof(xmEulerAngleXYZ));
	}



	xmRigidTransform offsetTSFLS;
	xmQuaternionFromEulerAngleXYZ(&offsetTSFLS.m_qRot,&eOffsetPivotSpace);
	xmQuaternion qPivotInvLS;
	xmQuaternionInverse(&qPivotInvLS,&m_qPivotLS);

	xmQuaternionMultiply(&offsetTSFLS.m_qRot,&qPivotInvLS,&offsetTSFLS.m_qRot);
	xmQuaternionMultiply(&offsetTSFLS.m_qRot,&offsetTSFLS.m_qRot,&m_qPivotLS);

	memset(&offsetTSFLS.m_vPos,0,sizeof(xmVector3));


	{
		xmRigidTransform iden;
		xmRigidTransform offsetLS;
		xmTransformSetIdentity(&iden);

		xmLerp(&offsetLS,&iden,&offsetTSFLS,m_fGain);
		xmQuaternionNormalize(&offsetLS.m_qRot,&offsetLS.m_qRot);

		xmNodeTransform nodeTSFLS;
		xmTransformSetIdentity(&nodeTSFLS);
		nodeTSFLS.m_qRot = offsetLS.m_qRot;

		pRigidPose->ApplyTransformLS(&offsetLS,m_nBoneID);
		pNodePose->ApplyTransformLS(&nodeTSFLS,m_nBoneID);
	}

}



void LookAtModifier::Init(xmUint nBoneID,const xmVector3& vInFowardLS,const xmVector3& vInUpLS)
{
	m_nBoneID = nBoneID;

	CalculatePovitLs(vInFowardLS, vInUpLS);
}

void LookAtModifier::CalculatePovitLs(const xmVector3& vFowardLS,const xmVector3& vUpLS)
{
	xmMatrix4x4 matPovitLS;
	xmVector3 vSide;
	xmVector3 vUp;
	D3DXVec3Cross(&vSide,&vUpLS,&vFowardLS);
	D3DXVec3Normalize(&vSide,&vSide);
	D3DXVec3Cross(&vUp,&vFowardLS,&vSide);
	xmMatrixAxis(&matPovitLS,&vFowardLS,&vSide,&vUp);
	D3DXQuaternionRotationMatrix(&m_qPivotLS,&matPovitLS);
}

xmVector3	LookAtModifier::GetFowardLS() const
{
	xmMatrix4x4 matOldPPovitLS;
	D3DXMatrixRotationQuaternion(&matOldPPovitLS, &m_qPivotLS);
	xmVector3 vOldFowardLS = xmVector3(matOldPPovitLS._11, matOldPPovitLS._12, matOldPPovitLS._13);
	return vOldFowardLS;
}
xmVector3	LookAtModifier::GetUpLS() const
{
	xmMatrix4x4 matOldPPovitLS;
	D3DXMatrixRotationQuaternion(&matOldPPovitLS, &m_qPivotLS);
	xmVector3 vOldUpLS = xmVector3(matOldPPovitLS._31, matOldPPovitLS._32, matOldPPovitLS._33);
	return vOldUpLS;
}
void LookAtModifier::SetBoneID( xmUint nBoneID )
{
	m_nBoneID = nBoneID;
}

xmUint LookAtModifier::GetBoneID() const
{
	return m_nBoneID;
}


void				LookAtModifier::SetGoalObjectSpace(const xmVector3& vGoalOS)
{
	m_vGoalOS = vGoalOS;
}


const xmVector3&	LookAtModifier::GetGoalObjectSpace()const
{
	return m_vGoalOS;
}

void				LookAtModifier::SetMaxYaw(float fMaxYaw)
{
	m_fMaxYaw = xmClamp(fMaxYaw,0.0f,D3DX_PI);
}

float				LookAtModifier::GetMaxYaw() const
{
	return m_fMaxYaw;
}

//xmMatrix4x4 S3ALookAtModifier::ApplyLimit(const xmMatrix4x4* pBonePS)const
//{
//	xmMatrix4x4 ret;
//	xmEulerAngleXYZ eRotPS;
//	xmEulerAngleXYZFromMatrix(&eRotPS,pBonePS);
//	eRotPS.x = xmClamp(eRotPS.x,m_vAngleMin.x,m_vAngleMax.x);
//	eRotPS.y = xmClamp(eRotPS.y,m_vAngleMin.y,m_vAngleMax.y);
//	eRotPS.z = xmClamp(eRotPS.z,m_vAngleMin.z,m_vAngleMax.z);
//	xmMatrixFromEulerAngleXYZ(&ret,&eRotPS);
//
//	*xmMatrixAsTranslation(&ret) = *xmMatrixAsTranslation(pBonePS);
//	return ret;
//}



void LookAtModifier::DbgDraw() const
{
// 	IS3ADebugRenderer* pdbgr = S3AGetDebugRenderer();
// 	if (pdbgr)
// 	{
// 		pdbgr->DrawBox(&m_vGoalOS,NULL,1.0f,0xffffffff);
// 		pdbgr->DrawString(m_vGoalOS,"LookAtModifierGoalOS");
// 	}
}