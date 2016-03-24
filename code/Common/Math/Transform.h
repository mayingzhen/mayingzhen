#ifndef __Transform_H__
#define __Transform_H__

namespace ma
{

	struct Transform
	{
		Vector3		m_vPos;
		Quaternion	m_qRot;
		Vector3		m_vScale;

		Transform()
		{
			m_vPos = Vector3::ZERO;
			m_qRot = Quaternion::IDENTITY;
			m_vScale = Vector3::UNIT_SCALE;
		}
	};



	inline void TransformInverse(Transform* pOut,const Transform* pTSF)
	{
// 		Matrix4 mat;
// 		mat.makeTransform(pTSF->m_vPos,pTSF->m_vScale,pTSF->m_qRot);
// 
// 		mat = mat.inverse();
// 		mat[3][3] = 1;
// 
// 		mat.decomposition(pOut->m_vPos,pOut->m_vScale,pOut->m_qRot);
		
		pOut->m_vScale.x = 1.0f / pTSF->m_vScale.x;
		pOut->m_vScale.x = 1.0f / pTSF->m_vScale.y;
		pOut->m_vScale.x = 1.0f / pTSF->m_vScale.z;
		
		pOut->m_vPos = pTSF->m_vPos * pOut->m_vScale;

		pOut->m_qRot = pTSF->m_qRot.Inverse();
		pOut->m_vPos = pOut->m_qRot * pOut->m_vPos;
		pOut->m_vPos = -pOut->m_vPos;
	}

	inline void  TransformMul(Transform* pOut,const Transform* pTSFA,const Transform* pTSFB)
	{
// 		Matrix4 matA,matB;
// 		matA.makeTransform(pTSFA->m_vPos,pTSFA->m_vScale,pTSFA->m_qRot);
// 		matB.makeTransform(pTSFB->m_vPos,pTSFB->m_vScale,pTSFB->m_qRot);
// 
// 		Matrix4 matOut = matA * matB;
// 
// 		matOut.decomposition(pOut->m_vPos,pOut->m_vScale,pOut->m_qRot);

		Vector3 vNewPos = pTSFB->m_vPos * pTSFA->m_vScale;
		vNewPos = pTSFA->m_qRot * vNewPos;
		pOut->m_vPos = vNewPos + pTSFA->m_vPos;
		pOut->m_qRot = pTSFA->m_qRot * pTSFB->m_qRot;
		pOut->m_vScale = pTSFA->m_vScale * pTSFB->m_vScale;

		pOut->m_qRot.normalise();
	}

	inline void TransformMul(Transform& Out, const Transform& TSF,float fWeight)
	{
		Out.m_vPos = TSF.m_vPos * fWeight;
		Out.m_qRot = TSF.m_qRot * fWeight;
		Out.m_vScale = TSF.m_vScale * fWeight;;
	}

	inline void TransformInvMul(Transform* pOut,const Transform* pTSFA,const Transform* pTSFB)
	{
		Transform tsfAInv;
		TransformInverse(&tsfAInv,pTSFB);
		TransformMul(pOut,pTSFA,&tsfAInv);
	}

	inline void MatrixFromTransform(Matrix4* pMat,const Transform* pRT)
	{
		pMat->makeTransform(pRT->m_vPos,pRT->m_vScale,pRT->m_qRot);
	}

	inline void TransformFromMatrix(Transform* pRT,const Matrix4* pMat)
	{
		pMat->decomposition(pRT->m_vPos,pRT->m_vScale,pRT->m_qRot);
	}

	inline Quaternion QuaternionMad(const Quaternion& Q1, const Quaternion& Q2,float fWeight)
	{
		Quaternion Out; 
		if (Q1.Dot(Q2) >= 0.0f)
		{
			Out = Q1 + Q2 * fWeight;
		}
		else
		{
			Out = Q1 - Q2 * fWeight;
		}
		return Out;
	}

	inline void TransformMad(Transform& out,const Transform& pA,float f,const Transform& pB)
	{
 		out.m_vPos = Math::Mad(pA.m_vPos,pB.m_vPos,f);
 		out.m_qRot = QuaternionMad(pB.m_qRot,pA.m_qRot,f);
 		out.m_vScale = Math::Mad(pA.m_vScale,pB.m_vScale,f);

		out.m_qRot.normalise();
	}

	inline void TransformLerp(Transform& out,const Transform& pA,float f,const Transform& pB)
	{
		out.m_vPos = Math::Lerp(pA.m_vPos,pB.m_vPos,f);
		out.m_vScale = Math::Lerp(pA.m_vScale,pB.m_vScale,f);
		out.m_qRot = Quaternion::Slerp(f,pA.m_qRot,pB.m_qRot);
	}

}

#endif
