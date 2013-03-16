#ifndef  _maMath__H__
#define  _maMath__H__

struct COMMON_API maGUID
{
	unsigned __int64 m_a;
	unsigned __int64 m_b;

	void Clear()
	{
		m_a = 0;
		m_b = 0;
	}

	bool operator ==(const maGUID& rhs) const
	{
		return (m_a == rhs.m_a) && (m_b == rhs.m_b);
	}

	bool operator !=(const maGUID& rhs) const
	{
		return !(*this == rhs);
	}

	bool operator <(const maGUID& rhs) const
	{
		return (m_a < rhs.m_a ? true 
			: (m_a == rhs.m_a ? m_b < rhs.m_b : false)
			);
	}
};

struct COMMON_API maNodeTransform
{
	D3DXVECTOR3		m_vPos;
	D3DXQUATERNION	m_qRot;
	float			m_fScale;

	//static maNodeTransform Identity();
};


//------------------------------------------------------------------------------
//EulerAngle
//------------------------------------------------------------------------------


//Rotate on x axis first, then y,z axis
//x-roll
//y-pitch
//z-yaw
struct COMMON_API EulerAngleXYZ
{
	float x;
	float y;
	float z;

	EulerAngleXYZ();

	EulerAngleXYZ(float fX,float fY,float fZ);

	EulerAngleXYZ operator+(const EulerAngleXYZ& rhs);

	EulerAngleXYZ& operator+=(const EulerAngleXYZ& rhs);

	EulerAngleXYZ& operator*=(float);

	void Normalize();
};


//axis is non zero
//x-roll = 0, y-pitch, z-yaw
COMMON_API void maEulerAngleFromXToAxis(EulerAngleXYZ* pEuler,const D3DXVECTOR3* pAxis);

COMMON_API void maEulerAngleFromQuaternion(EulerAngleXYZ* pEuler,const D3DXQUATERNION* pQua);

COMMON_API bool maEulerAngleXYZFromMatrix(EulerAngleXYZ* pEuler,const D3DXMATRIX* pMat);



// Quaternion
COMMON_API void maQuaternionTransformVector(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXQUATERNION* pQuat);

COMMON_API void maQuaternionMad(D3DXQUATERNION* pOut, const D3DXQUATERNION* pQ1, const D3DXQUATERNION* pQ2, float fWeight);

COMMON_API void maQuaternionFromEulerAngleXYZ(D3DXQUATERNION* pQuat,const EulerAngleXYZ* pEuler);

COMMON_API void	maQuaternionFromAxisToAxis(D3DXQUATERNION* pRot,const D3DXVECTOR3* pAxisFrom,const D3DXVECTOR3* pAxisTo);


template<class T>
inline void maLerp(T& out, const T& v0,const T& v1,float factor)
{
	out = v0*(1.0f-factor)+v1*factor;
}

inline float xmClamp(float fVal,float fMin,float fMax)
{
	fVal = fVal > fMin ? fVal : fMin;
	fVal = fVal < fMax ? fVal : fMax;
	return fVal;
}


// Transform
COMMON_API void maTransformSetIdentity(maNodeTransform* pTSF);

COMMON_API void maTransformInverse(maNodeTransform* pOut, const maNodeTransform* pTSF);

COMMON_API void maTransformMul(maNodeTransform* pOut, const maNodeTransform* pTSFA, const maNodeTransform* pTSFB);

COMMON_API void maTransformMad(maNodeTransform* pOut,maNodeTransform* pTSFA,maNodeTransform* pTSFB,float fWeight);

COMMON_API void maTransfromInvMul(maNodeTransform* pOut, const maNodeTransform* pTSFA, const maNodeTransform* pTSFB);

COMMON_API void maVec3TransformNormal(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const maNodeTransform* pTSF);

COMMON_API void maTransformPoint(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const maNodeTransform* pTSF);

COMMON_API void maTransformVector(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const maNodeTransform* pTSF);

COMMON_API void maTransformLerp(maNodeTransform* pOut,const maNodeTransform* pA,const maNodeTransform* pB,float fFactor);

COMMON_API void maTransformFromMatrix(maNodeTransform* pOut,const D3DXMATRIX& mat);

// Matrix
COMMON_API void maMatrixFromTransform(D3DXMATRIX* pMat, const maNodeTransform* pTSF);

COMMON_API void maMatrixAxis(D3DXMATRIX* pMat,const D3DXVECTOR3* pX,const D3DXVECTOR3* pY, const D3DXVECTOR3* pZ);

//COMMON_API const D3DXVECTOR3* maMatrixAsVector3(D3DXMATRIX* pMat,UINT uCol);

COMMON_API D3DXVECTOR3* maMatrixAsVector3(D3DXMATRIX* pMat,UINT uCol);

//COMMON_API const D3DXVECTOR4* maMatrixAsVector4(D3DXMATRIX* pMat,UINT uCol);

COMMON_API D3DXVECTOR4* maMatrixAsVector4(D3DXMATRIX* pMat,UINT uCol);

// xmInline void xmAPI xmEulerAngleFromQuaternion(EulerAngleXYZ* pEuler,const D3DXQUATERNION* pQua)
// {
// 
// 	D3DXMATRIX mat;
// 
// 	xmMatrixRotationQuaternion(&mat,pQua);
// 	xmEulerAngleXYZFromMatrix(pEuler,&mat);
// 
// 	D3DXMatrixRotationQuaternion()
// }
// 
// void  maMatrixFromQuaternion(D3DXMATRIX* pMat,const D3DXQUATERNION* pRot)
// {
// 	EulerAngleXYZ eRot;
// 	maEulerAngleFromQuaternion(&eRot,pRot);
// 	maMatrixFromEulerAngleXYZ(pMat,&eRot);
// }


template<class T>
T InvalidID()
{
	return T(-1);
}

template<class T> 
bool IsValidID(T ind)
{
	return ind != (T)(-1);
}

template<class T> 
bool IsInValidID(T ind)
{
	return ind == (T)(-1);
}

template<class T>
inline const T& maMax(const T& a,const T& b)
{
	return a > b ? a : b;
}



COMMON_API void Log(const char* fmt,...);
COMMON_API void SSERT_MSG(bool expr,const char* fmt,...);
COMMON_API void LogError(UINT nErrorCode,const char* fmt,...);

COMMON_API void  Vec3Min(D3DXVECTOR3* pOut,const D3DXVECTOR3* pA,const D3DXVECTOR3* pB);
COMMON_API void  Vec3Max(D3DXVECTOR3* pOut,const D3DXVECTOR3* pA,const D3DXVECTOR3* pB);
COMMON_API D3DXVECTOR3 Vec3Zero();




#endif
