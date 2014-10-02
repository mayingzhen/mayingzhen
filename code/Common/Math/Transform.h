#pragma once

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



	inline void  TransformInverse(Transform* pOut,const Transform* pTSF)
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

	inline void  TransformInvMul(Transform* pOut,const Transform* pTSFA,const Transform* pTSFB)
	{
		Transform tsfAInv;
		TransformInverse(&tsfAInv,pTSFB);
		TransformMul(pOut,pTSFA,&tsfAInv);
	}

	inline void  MatrixFromTransform(Matrix4* pMat,const Transform* pRT)
	{
		pMat->makeTransform(pRT->m_vPos,pRT->m_vScale,pRT->m_qRot);
	}

	inline void  TransformFromMatrix(Transform* pRT,const Matrix4* pMat)
	{
		pMat->decomposition(pRT->m_vPos,pRT->m_vScale,pRT->m_qRot);
	}


}