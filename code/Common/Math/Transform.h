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
		Matrix4 mat;
		mat.makeTransform(pTSF->m_vPos,pTSF->m_vScale,pTSF->m_qRot);

		mat = mat.inverse();
		mat[3][3] = 1;

		mat.decomposition(pOut->m_vPos,pOut->m_vScale,pOut->m_qRot);
	}

	inline void  TransformMul(Transform* pOut,const Transform* pTSFA,const Transform* pTSFB)
	{
		Matrix4 matA,matB;
		matA.makeTransform(pTSFA->m_vPos,pTSFA->m_vScale,pTSFA->m_qRot);
		matB.makeTransform(pTSFB->m_vPos,pTSFB->m_vScale,pTSFB->m_qRot);

		Matrix4 matOut = matA * matB;

		matOut.decomposition(pOut->m_vPos,pOut->m_vScale,pOut->m_qRot);
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

	inline void TransformMad(Transform& out,const Transform& pA,float f,const Transform& pB)
	{
		//S3AVec3Mad(&pOut->m_vPos,&pA->m_vPos,f,&pB->m_vPos);
		//S3AQuaternionMad(&pOut->m_qRot,&pB->m_qRot,&pA->m_qRot,f);
		//S3AVec3Mad(&pOut->m_vLocalScale,&pA->m_vLocalScale,f,&pB->m_vLocalScale);
		//pOut->m_fScale = pA->m_fScale * f + pB->m_fScale;

		//Quaternion addRot = Quaternion::Slerp(fWeight,Quaternion::IDENTITY,source.m_qRot);

		// apply relative addition to the result of the previous layer
		//result.m_vPos = dest.m_vPos + (source.m_vPos * fWeight);
		//result.m_qRot = dest.m_qRot * addRot;
		//result.m_fScale	 = Vector3::UNIT_SCALE * dest.m_fScale; //+ (relScale * fWeight);
	}

	inline void TransformLerp(Transform& out,const Transform& pA,float f,const Transform& pB)
	{
		out.m_vPos = Math::Lerp(pA.m_vPos,pB.m_vPos,f);
		out.m_vScale = Math::Lerp(pA.m_vScale,pB.m_vScale,f);
		out.m_qRot = Quaternion::Slerp(f,pA.m_qRot,pB.m_qRot);
	}

}

#endif
