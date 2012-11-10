#ifndef  _maMath__H__
#define  _maMath__H__

struct COMMON_API maNodeTransform
{
	D3DXVECTOR3		m_vPos;
	D3DXQUATERNION	m_qRot;
	float			m_fScale;

	//static maNodeTransform Identity();
};

// Quaternion
COMMON_API void  maQuaternionTransformVector(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXQUATERNION* pQuat);

COMMON_API void maQuaternionMad(D3DXQUATERNION* pOut, const D3DXQUATERNION* pQ1, const D3DXQUATERNION* pQ2, float fWeight);

// Transform
COMMON_API void maTransformSetIdentity(maNodeTransform* pTSF);

COMMON_API void maTransformInverse(maNodeTransform* pOut, const maNodeTransform* pTSF);

COMMON_API void maTransfromMul(maNodeTransform* pOut, const maNodeTransform* pTSFA, const maNodeTransform* pTSFB);

COMMON_API void maTransfromInvMul(maNodeTransform* pOut, const maNodeTransform* pTSFA, const maNodeTransform* pTSFB);

COMMON_API void maVec3TransformNormal(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const maNodeTransform* pTSF);

// Matrix
COMMON_API void maMatrixFromTransform(D3DXMATRIX* pMat, const maNodeTransform* pTSF);




#endif
