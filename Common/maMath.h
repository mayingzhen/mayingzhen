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

// Quaternion
COMMON_API void maQuaternionTransformVector(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXQUATERNION* pQuat);

COMMON_API void maQuaternionMad(D3DXQUATERNION* pOut, const D3DXQUATERNION* pQ1, const D3DXQUATERNION* pQ2, float fWeight);

//COMMON_API void maQuaternionLerp(xmQuaternion* out,const xmQuaternion* q0,const xmQuaternion* q1,float factor);

// Transform
COMMON_API void maTransformSetIdentity(maNodeTransform* pTSF);

COMMON_API void maTransformInverse(maNodeTransform* pOut, const maNodeTransform* pTSF);

COMMON_API void maTransfromMul(maNodeTransform* pOut, const maNodeTransform* pTSFA, const maNodeTransform* pTSFB);

COMMON_API void maTransformMad(maNodeTransform* pOut,maNodeTransform* pTSFA,maNodeTransform* pTSFB,float fWeight);

COMMON_API void maTransfromInvMul(maNodeTransform* pOut, const maNodeTransform* pTSFA, const maNodeTransform* pTSFB);

COMMON_API void maVec3TransformNormal(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const maNodeTransform* pTSF);

// Matrix
COMMON_API void maMatrixFromTransform(D3DXMATRIX* pMat, const maNodeTransform* pTSF);




#endif
