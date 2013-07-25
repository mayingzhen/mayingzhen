#include "Math.h"


NodeTransform NodeTransform::Identity()
{
	NodeTransform iden;
	TransformSetIdentity(&iden);
	return iden;
}


void  EulerAngleFromQuaternion(float& fRoll,float& fPitch, float& fYaw,const Quaternion* inQua)
{
	EulerAngleXYZ ang;
	EulerAngleFromQuaternion((float*)&ang,inQua);
	fRoll = ang.x;
	fPitch = ang.y;
	fYaw = ang.z;
}

void  EulerAngleFromQuaternion(float* eulerVal, const Quaternion* pRot)
{
	EulerAngleXYZ* pEulerAngle = (EulerAngleXYZ*)eulerVal;
	EulerAngleFromQuaternion(pEulerAngle,pRot);

}

void  QuaternionFromEulerAngle(Quaternion* pRot,const float* eulerVal)
{
	const EulerAngleXYZ* pEulerAngle = (const EulerAngleXYZ*)eulerVal;
	QuaternionFromEulerAngleXYZ(pRot,pEulerAngle);
}

bool MatrixIsIdentity(Matrix4x4* pMat,float fThreahold = FEPS)
{
	Matrix4x4 matIden;
	MatrixIdentity(&matIden);
	for (Uint nRowCnt = 0; nRowCnt < 4; ++nRowCnt)
	{
		for(Uint nColCnt = 0; nColCnt < 4; ++nColCnt)
		{
			if (!FloatEqual(pMat->m[nRowCnt][nColCnt],matIden.m[nRowCnt][nColCnt],fThreahold))
			{
				return false;
			}
		}
	}
	return true;
}

bool QuaternionIsIdentity(Quaternion* pQ,float fThreahold = FEPS)
{
	return (FloatEqual(pQ->x,0.0f,fThreahold) 
		&& FloatEqual(pQ->y,0.0f,fThreahold)
		&& FloatEqual(pQ->z,0.0f,fThreahold) 
		&& FloatEqual(pQ->w,1.0f,fThreahold));
}

bool Vector3IsZero(Vector3* pV,float fThreahold = FEPS)
{
	return (FloatEqual(pV->x,0.0f,fThreahold) 
		&& FloatEqual(pV->y,0.0f,fThreahold)
		&& FloatEqual(pV->z,0.0f,fThreahold));
}

bool Vector3IsOne(Vector3* pV,float fThreahold = FEPS)
{
	return (FloatEqual(pV->x,1.0f,fThreahold) 
		&& FloatEqual(pV->y,1.0f,fThreahold)
		&& FloatEqual(pV->z,1.0f,fThreahold));
}




bool TransformIsIdentity(NodeTransform* pTSF,float fThreshold = FEPS)
{
	bool bPosIden = Vector3IsZero(&pTSF->m_vPos,fThreshold);
	bool bRotIden = QuaternionIsIdentity(&pTSF->m_qRot,fThreshold);
	//bool bScaleIden = Vector3IsOne(&pTSF->m_vLocalScale,fThreshold) && FloatEqual(pTSF->m_fScale,1.0f,fThreshold);
	return bPosIden && bRotIden /*&& bScaleIden*/;
}



COMMON_API Matrix4x4* MatrixPerspectiveGL_RH(Matrix4x4* pOut,float w,float h,float zn,float zf)
{
	if(NULL == pOut)
	{
		return NULL;
	}

	float inv = 1.0f / (zn - zf);

	pOut->_11 = 2.0f * zn/w;	pOut->_12 = 0.0f;		pOut->_13 = 0.0f;			pOut->_14 = 0.0f;
	pOut->_21 = 0.0f;			pOut->_22 = 2.0f*zn/h;	pOut->_23 = 0.0f;			pOut->_24 = 0.0f;
	pOut->_31 = 0.0f;			pOut->_32 = 0.0f;		pOut->_33 = (zn+zf)* inv;	pOut->_34 =-1.0f;
	pOut->_41 = 0.0f;			pOut->_42 = 0.0f;		pOut->_43 = 2.0f*zn*zf*inv;	pOut->_44 = 0.0f;

	return pOut;
}

COMMON_API Matrix4x4* MatrixPerspectiveFovGL_RH(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf)
{
	if(NULL == pOut)
	{
		return NULL;
	}

	float inv = 1.0f / (zn - zf);

	float yScale = tanf(0.5f* PI - 0.5f*fovy);
	float xScale = yScale / Aspect;

	pOut->_11 = xScale;			pOut->_12 = 0.0f;		pOut->_13 = 0.0f;			pOut->_14 = 0.0f;
	pOut->_21 = 0.0f;			pOut->_22 = yScale;		pOut->_23 = 0.0f;			pOut->_24 = 0.0f;
	pOut->_31 = 0.0f;			pOut->_32 = 0.0f;		pOut->_33 = (zn+zf)* inv;	pOut->_34 =-1.0f;
	pOut->_41 = 0.0f;			pOut->_42 = 0.0f;		pOut->_43 = 2.0f*zn*zf*inv;	pOut->_44 = 0.0f;

	return pOut;
}


COMMON_API Matrix4x4* MatrixPerspectiveGL_LH(Matrix4x4* pOut,float w,float h,float zn,float zf)
{
	if(NULL == pOut)
	{
		return NULL;
	}

	float inv = 1.0f / (zn - zf);

	pOut->_11 = 2.0f * zn/w;	pOut->_12 = 0.0f;		pOut->_13 = 0.0f;			pOut->_14 = 0.0f;
	pOut->_21 = 0.0f;			pOut->_22 = 2.0f*zn/h;	pOut->_23 = 0.0f;			pOut->_24 = 0.0f;
	pOut->_31 = 0.0f;			pOut->_32 = 0.0f;		pOut->_33 = -(zn+zf)* inv;	pOut->_34 = 1.0f;
	pOut->_41 = 0.0f;			pOut->_42 = 0.0f;		pOut->_43 = 2.0f*zn*zf*inv;	pOut->_44 = 0.0f;

	return pOut;
}

COMMON_API Matrix4x4* MatrixPerspectiveFovGL_LH(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf)
{
	if(NULL == pOut)
	{
		return NULL;
	}

	float inv = 1.0f / (zn - zf);

	float yScale = tanf(0.5f* PI - 0.5f*fovy);
	float xScale = yScale / Aspect;

	pOut->_11 = xScale;			pOut->_12 = 0.0f;		pOut->_13 = 0.0f;			pOut->_14 = 0.0f;
	pOut->_21 = 0.0f;			pOut->_22 = yScale;		pOut->_23 = 0.0f;			pOut->_24 = 0.0f;
	pOut->_31 = 0.0f;			pOut->_32 = 0.0f;		pOut->_33 = -(zn+zf)* inv;	pOut->_34 = 1.0f;
	pOut->_41 = 0.0f;			pOut->_42 = 0.0f;		pOut->_43 = 2.0f*zn*zf*inv;	pOut->_44 = 0.0f;

	return pOut; 
}

COMMON_API Matrix4x4* MatrixOrthoGL_LH(Matrix4x4* pOut,float w,float h,float zn,float zf)
{
	if(NULL == pOut)
	{
		return NULL;
	}

	float l = -w * 0.5f;
	float r = w * 0.5f;
	float b = -h * 0.5f;
	float t = h * 0.5f;

	pOut->_11 = 2.0f / (r-l);			pOut->_12 = 0.0f;					pOut->_13 = 0.0f;					pOut->_14 = 0.0f;
	pOut->_21 = 0.0f;					pOut->_22 = 2.0f / (t-b);			pOut->_23 = 0.0f;					pOut->_24 = 0.0f;
	pOut->_31 = 0.0f;					pOut->_32 = 0.0f;					pOut->_33 = 2.0f / (zf-zn);			pOut->_34 = 0.0f;
	pOut->_41 = (l+r) / (l-r);			pOut->_42 = (t+b) / (b-t);			pOut->_43 = (zf+zn) / (zn-zf);		pOut->_44 = 1.0f;

	return pOut;
}

COMMON_API Matrix4x4* MatrixOrthoOffCenterGL_LH(Matrix4x4* pOut,float l,float r, float b, float t, float zn,float zf)
{
	if(NULL == pOut)
	{
		return NULL;
	}

// 	float l = -w * 0.5f;
// 	float r = w * 0.5f;
// 	float b = -h * 0.5f;
// 	float t = h * 0.5f;

	pOut->_11 = 2.0f / (r-l);			pOut->_12 = 0.0f;					pOut->_13 = 0.0f;					pOut->_14 = 0.0f;
	pOut->_21 = 0.0f;					pOut->_22 = 2.0f / (t-b);			pOut->_23 = 0.0f;					pOut->_24 = 0.0f;
	pOut->_31 = 0.0f;					pOut->_32 = 0.0f;					pOut->_33 = 2.0f / (zf-zn);			pOut->_34 = 0.0f;
	pOut->_41 = (l+r) / (l-r);			pOut->_42 = (t+b) / (b-t);			pOut->_43 = (zf+zn) / (zn-zf);		pOut->_44 = 1.0f;

	return pOut;
}