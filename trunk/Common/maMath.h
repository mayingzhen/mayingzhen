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
struct COMMON_API xmEulerAngleXYZ
{
	xmFloat x;
	xmFloat y;
	xmFloat z;

	xmEulerAngleXYZ();

	xmEulerAngleXYZ(float fX,float fY,float fZ);

	xmEulerAngleXYZ operator+(const xmEulerAngleXYZ& rhs);

	xmEulerAngleXYZ& operator+=(const xmEulerAngleXYZ& rhs);

	xmEulerAngleXYZ& operator*=(float);

	void Normalize();
};


//axis is non zero
//x-roll = 0, y-pitch, z-yaw
COMMON_API void maEulerAngleFromXToAxis(xmEulerAngleXYZ* pEuler,const xmVector3* pAxis);


// Quaternion
COMMON_API void maQuaternionTransformVector(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXQUATERNION* pQuat);

COMMON_API void maQuaternionMad(D3DXQUATERNION* pOut, const D3DXQUATERNION* pQ1, const D3DXQUATERNION* pQ2, float fWeight);

COMMON_API void maQuaternionFromEulerAngleXYZ(xmQuaternion* pQuat,const xmEulerAngleXYZ* pEuler);



template<class T>
inline void maLerp(T& out, const T& v0,const T& v1,float factor)
{
	out = v0*(1.0f-factor)+v1*factor;
}


// Transform
COMMON_API void maTransformSetIdentity(maNodeTransform* pTSF);

COMMON_API void maTransformInverse(maNodeTransform* pOut, const maNodeTransform* pTSF);

COMMON_API void maTransfromMul(maNodeTransform* pOut, const maNodeTransform* pTSFA, const maNodeTransform* pTSFB);

COMMON_API void maTransformMad(maNodeTransform* pOut,maNodeTransform* pTSFA,maNodeTransform* pTSFB,float fWeight);

COMMON_API void maTransfromInvMul(maNodeTransform* pOut, const maNodeTransform* pTSFA, const maNodeTransform* pTSFB);

COMMON_API void maVec3TransformNormal(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const maNodeTransform* pTSF);

COMMON_API void maTransformPoint(xmVector3* pOut, const xmVector3* pV, const maNodeTransform* pTSF);

COMMON_API void maTransformLerp(maNodeTransform* pOut,const maNodeTransform* pA,const maNodeTransform* pB,float fFactor);

// Matrix
COMMON_API void maMatrixFromTransform(D3DXMATRIX* pMat, const maNodeTransform* pTSF);

COMMON_API void  maMatrixAxis(xmMatrix4x4* pMat,const xmVector3* pX,const xmVector3* pY, const xmVector3* pZ);




#endif
