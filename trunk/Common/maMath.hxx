#include "Common/maMath.h"

// maNodeTransform::Identity()
// {
// 	maTransformSetIdentity(this);
// }


//------------------------------------------------------------------------------
//Euler Angle
//------------------------------------------------------------------------------

xmEulerAngleXYZ::xmEulerAngleXYZ()
{

}

xmEulerAngleXYZ::xmEulerAngleXYZ(float fX,float fY,float fZ)
:x(fX)
,y(fY)
,z(fZ)
{

}


xmEulerAngleXYZ xmEulerAngleXYZ::operator +(const xmEulerAngleXYZ& rhs)
{
	return xmEulerAngleXYZ(x + rhs.x,y + rhs.y, z + rhs.z);
}

xmEulerAngleXYZ& xmEulerAngleXYZ::operator +=(const xmEulerAngleXYZ& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

xmEulerAngleXYZ& xmEulerAngleXYZ::operator *=(float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}


void xmEulerAngleXYZ::Normalize()
{
	x = fmodf(x,2.0f*D3DX_PI);
	y = fmodf(y,2.0f*D3DX_PI);
	z = fmodf(z,2.0f*D3DX_PI);
}


//axis is non zero
//x-roll = 0, y-pitch, z-yaw
void maEulerAngleFromXToAxis(xmEulerAngleXYZ* pEuler,const D3DXVECTOR3* pAxis)
{
	float r = sqrtf(pAxis->x*pAxis->x+pAxis->y*pAxis->y);
	pEuler->x = 0.0f;
	pEuler->z = atan2f(pAxis->y,pAxis->x);	//yaw
	pEuler->y = - atan2f(pAxis->z,r);		//pitch

}


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

void maTransformMad(maNodeTransform* pOut,maNodeTransform* pTSFA,maNodeTransform* pTSFB,float fWeight)
{
	pOut->m_fScale = pTSFA->m_fScale * pTSFB->m_fScale * fWeight;
	
	//maQuaternionMad(&pOut->m_qRot,&pTSFA->m_qRot,&pTSFB.m_qRot,fWeight);
	D3DXQuaternionMultiply(&pOut->m_qRot,&pTSFA->m_qRot,&(pTSFB->m_qRot * fWeight));

	pOut->m_vPos = pTSFA->m_vPos +  pTSFB->m_vPos * fWeight;
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

void maTransformPoint(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const maNodeTransform* pTSF)
{
	maQuaternionTransformVector(pOut,pV,&pTSF->m_qRot);
	*pOut += pTSF->m_vPos;
}

void maTransformVector(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const maNodeTransform* pTSF)
{
	maQuaternionTransformVector(pOut,pV,&pTSF->m_qRot);	
}

void maMatrixFromTransform(D3DXMATRIX* pMat, const maNodeTransform* pTSF)
{
	D3DXQUATERNION qRot;
	D3DXQuaternionNormalize(&qRot, &pTSF->m_qRot);

	D3DXVECTOR3 vScale(pTSF->m_fScale, pTSF->m_fScale, pTSF->m_fScale);

	D3DXMatrixTransformation(pMat, NULL, NULL, &vScale, NULL, &qRot, &pTSF->m_vPos);
}

void  maVec4SetVector(D3DXVECTOR4* pVec4, const D3DXVECTOR3* pVec3)
{
	pVec4->x = pVec3->x;
	pVec4->y = pVec3->y;
	pVec4->z = pVec3->z;
	pVec4->w = 0.0f;
}

D3DXVECTOR4* maMatrixAsVector4(D3DXMATRIX* pMat,xmUint nCol)
{
	return reinterpret_cast<D3DXVECTOR4*>(&pMat->m[nCol][0]);
}

// const D3DXVECTOR4* maMatrixAsVector4(const D3DXMATRIX* pMat,xmUint uCol)
// {
// 	return reinterpret_cast<const D3DXVECTOR4*>(&pMat->m[uCol][0]);
// }
// 
// const D3DXVECTOR3* maMatrixAsVector3(D3DXMATRIX* pMat,UINT uCol)
// {
// 	return reinterpret_cast<const D3DXVECTOR3*>(&pMat->m[uCol][0]);
// }

D3DXVECTOR3* maMatrixAsVector3(D3DXMATRIX* pMat,UINT uCol)
{
	return reinterpret_cast<D3DXVECTOR3*>(&pMat->m[uCol][0]);
}


void  maMatrixAxis(D3DXMATRIX* pMat,const D3DXVECTOR3* pX,const D3DXVECTOR3* pY, const D3DXVECTOR3* pZ)
{
	maVec4SetVector(maMatrixAsVector4(pMat,0),pX);
	maVec4SetVector(maMatrixAsVector4(pMat,1),pY);
	maVec4SetVector(maMatrixAsVector4(pMat,2),pZ);
	*maMatrixAsVector4(pMat,3) = D3DXVECTOR4(0.0f,0.0f,0.0f,1.0f);
}


void maQuaternionFromEulerAngleXYZ(D3DXQUATERNION* pQuat,const xmEulerAngleXYZ* pEuler)
{
	float fHalfX = 0.5f*pEuler->x;
	float fHalfY = 0.5f*pEuler->y;
	float fHalfZ = 0.5f*pEuler->z;
	D3DXQUATERNION qRotX(sinf(fHalfX),0.0f,0.0f,cosf(fHalfX));
	D3DXQUATERNION qRotY(0.0f,sinf(fHalfY),0.0f,cosf(fHalfY));
	D3DXQUATERNION qRotZ(0.0f,0.0f,sinf(fHalfZ),cosf(fHalfZ));
	*pQuat = qRotX * qRotY;
	*pQuat = *pQuat * qRotZ;

}

void maQuaternionFromAxisToAxis(D3DXQUATERNION* pRot,const D3DXVECTOR3* pAxisFrom,const D3DXVECTOR3* pAxisTo)
{	
	D3DXVECTOR3 vAxisRot;
	D3DXVec3Cross(&vAxisRot,pAxisFrom,pAxisTo);
	float fSinAlpha = D3DXVec3Length(&vAxisRot);
	float fCosAlpha = D3DXVec3Dot(pAxisFrom,pAxisTo);
	float fAlpha = atan2f(fSinAlpha,fCosAlpha);
	if ( abs(fSinAlpha - 0.0f) < F_EPS )
	{
		if (fCosAlpha < 0.0f)
		{
			D3DXVECTOR3 vAxisTangent(pAxisFrom->y,pAxisFrom->z,pAxisFrom->x);
			D3DXVec3Cross(&vAxisRot,pAxisFrom,&vAxisTangent);
			fAlpha = D3DX_PI;
		}
		else 
		{
			*pRot = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
			return;
		}
	}

	D3DXVec3Normalize(&vAxisRot,&vAxisRot);
	D3DXQuaternionRotationAxis(pRot,&vAxisRot,fAlpha);
}

void maTransformLerp(maNodeTransform* pOut,const maNodeTransform* pA,const maNodeTransform* pB,float fFactor)
{
	maLerp(pOut->m_vPos,pA->m_vPos,pB->m_vPos,fFactor);
	D3DXQuaternionSlerp(&pOut->m_qRot,&pA->m_qRot,&pB->m_qRot,fFactor);
	maLerp(pOut->m_fScale,pA->m_fScale,pB->m_fScale,fFactor);
}

void maTransformFromMatrix(maNodeTransform* pOut,const D3DXMATRIX& mat)
{
	D3DXVECTOR3 vScale;
	D3DXMatrixDecompose(&vScale,&pOut->m_qRot,&pOut->m_vPos,&mat);
}