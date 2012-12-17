#include "Animation/PoseModifier/LookAtModifier.h"
#include "Animation/NodePose.h"

namespace ma
{
	LookAtModifier::LookAtModifier()
		:m_vGoalOS(0.0,1.0,0.0)
		,m_qPivotLS(0.0f,0.0f,0.0f,1.0f)
		,m_nBoneID(InvalidID<xmUint>())
		,m_fGoalDistThreshold(1.0f)
		,m_fMaxYaw(D3DX_PI/2.0f)
	{

	}


	void LookAtModifier::UpdatePose(NodePose* pNodePose)
	{
		if (pNodePose == NULL)
			return;

		if (!m_bEnable)
			return;

		maNodeTransform pivotOS;
		maNodeTransform pivotInvOS;
		pivotOS = pNodePose->GetTransformOS(m_nBoneID);
		maTransformInverse(&pivotInvOS,&pivotOS);

		xmVector3 vGoalPivotSpace;
		maTransformPoint(&vGoalPivotSpace,&m_vGoalOS,&pivotInvOS);
		float fGoalLenPivotSpace = D3DXVec3Length(&vGoalPivotSpace);
		xmVector3 vGoalDirPivotSpace = vGoalPivotSpace / fGoalLenPivotSpace;
		maEulerAngleFromXToAxis()





// 		maNodeTransform pivotOS;
// 		maNodeTransform pivotInvOS;
// 
// 		pivotOS = pNodePose->GetTransformOS(m_nBoneID); //GetBoneTransformOS(m_nBoneID);
// 		pivotOS.m_qRot = m_qPivotLS * pivotOS.m_qRot;
// 		maTransformInverse(&pivotInvOS,&pivotOS);
// 
// 		xmVector3 vGoalPivotSpace;
// 		maTransformPoint(&vGoalPivotSpace,&m_vGoalOS,&pivotInvOS);
// 		float fGoalLenPivotSpace = D3DXVec3Length(&vGoalPivotSpace);
// 
// 		xmEulerAngleXYZ eOffsetPivotSpace;
// 		if (fGoalLenPivotSpace > xm_EPS)
// 		{
// 			xmVector3 vGoalDirPivotSpace = vGoalPivotSpace / fGoalLenPivotSpace;
// 			maEulerAngleFromXToAxis(&eOffsetPivotSpace,&vGoalDirPivotSpace);
// 		}
// 		else
// 		{
// 			memset(&eOffsetPivotSpace,0,sizeof(xmEulerAngleXYZ));
// 		}
// 
// 		D3DXQUATERNION qRotLS;
// 		maQuaternionFromEulerAngleXYZ(&qRotLS,&eOffsetPivotSpace);
// 		xmQuaternion qPivotInvLS;
// 		D3DXQuaternionInverse(&qPivotInvLS,&m_qPivotLS);
// 
// 		D3DXQuaternionMultiply(&qRotLS,&qPivotInvLS,&qRotLS);
// 		D3DXQuaternionMultiply(&qRotLS,&qRotLS,&m_qPivotLS);
// 
// 		// È¨ÖØ¼ÆËã
// 		D3DXQUATERNION iden;
// 		D3DXQuaternionIdentity(&iden);
// 		D3DXQuaternionSlerp(&qRotLS,&iden,&qRotLS,m_fGain);
// 			
// 		maNodeTransform offsetTSFLS;
// 		maTransformSetIdentity(&offsetTSFLS);
// 		offsetTSFLS.m_qRot = qRotLS;
// 
// 		pNodePose->ApplyTransformLS(&offsetTSFLS,m_nBoneID);

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
		maMatrixAxis(&matPovitLS,&vFowardLS,&vSide,&vUp);
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
}

