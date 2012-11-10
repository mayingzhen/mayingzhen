#include "Common/maMath.h"

// maNodeTransform::Identity()
// {
// 	maTransformSetIdentity(this);
// }


void maQuaternionTransformVector(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXQUATERNION* pQuat)
{
	D3DXVECTOR3 uv, uuv;
	D3DXVECTOR3 qvec(pQuat->x, pQuat->y, pQuat->z);
	D3DXVec3Cross(&uv, &qvec, pV);
	D3DXVec3Cross(&uuv, &qvec, &uv);
	uv *= (2.0f * pQuat->w);
	uuv *= 2.0f;
	*pOut = *pV + uv + uuv;
}

void maQuaternionMad(D3DXQUATERNION* pOut, const D3DXQUATERNION* pQ1, const D3DXQUATERNION* pQ2, float fWeight)
{
	if ( D3DXQuaternionDot(pQ1,pQ2) >= 0.0f )
	{
		*pOut = *pQ1 + *pQ2 * fWeight;
	}
	else
	{
		*pOut = *pQ1 - *pQ2 * fWeight;
	}
} 

void maTransformSetIdentity(maNodeTransform* pTSF)
{
	memset( &pTSF->m_vPos, 0, sizeof(D3DXVECTOR3) );
	D3DXQuaternionIdentity(&pTSF->m_qRot);
	pTSF->m_fScale = 1.0f;
}

void maTransfromMul(maNodeTransform* pOut, const maNodeTransform* pTSFA, const maNodeTransform* pTSFB)
{
	D3DXVECTOR3 vOpos = pTSFA->m_vPos * pTSFB->m_fScale;
	maQuaternionTransformVector(&vOpos, &vOpos, &pTSFB->m_qRot);
	pOut->m_vPos = vOpos + pTSFB->m_vPos;

	D3DXQuaternionMultiply(&pOut->m_qRot, &pTSFA->m_qRot, &pTSFB->m_qRot);

	pOut->m_fScale = pTSFA->m_fScale * pTSFB->m_fScale;
}

void maTransformInverse(maNodeTransform* pOut, const maNodeTransform* pTSF)
{
	float fScaleInv;

	fScaleInv = pTSF->m_fScale > F_EPS ? 1.0f / pTSF->m_fScale : 1.0f;

	D3DXQuaternionInverse(&pOut->m_qRot,&pTSF->m_qRot);

	pOut->m_vPos = pTSF->m_vPos * fScaleInv;
	maQuaternionTransformVector(&pOut->m_vPos, &pOut->m_vPos, &pOut->m_qRot);
	pOut->m_vPos = -pOut->m_vPos;

	pOut->m_fScale = fScaleInv;
}

void maTransfromInvMul(maNodeTransform* pOut, const maNodeTransform* pTSFA, const maNodeTransform* pTSFB)
{
	maNodeTransform tsfInv;
	maTransformInverse(&tsfInv, pTSFB);
	maTransfromMul(pOut, pTSFA, &tsfInv);
}

void maVec3TransformNormal(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const maNodeTransform* pTSF)
{
	maQuaternionTransformVector(pOut, pV, &pTSF->m_qRot);
}

void maMatrixFromTransform(D3DXMATRIX* pMat, const maNodeTransform* pTSF)
{
	D3DXQUATERNION qRot;
	D3DXQuaternionNormalize(&qRot, &pTSF->m_qRot);

	D3DXVECTOR3 vScale(pTSF->m_fScale, pTSF->m_fScale, pTSF->m_fScale);

	D3DXMatrixTransformation(pMat, NULL, NULL, &vScale, NULL, &qRot, &pTSF->m_vPos);
}
