#ifndef __Math_h__
#define __Math_h__

#include "MathBase.h"





#define MIN_NODE_SCALE 0.0001f


struct COMMON_API maGUID
{
	Uint64 m_a;
	Uint64 m_b;


	void Clear();

	bool operator ==(const maGUID& rhs) const;

	bool operator !=(const maGUID& rhs) const;

	bool operator <(const maGUID& rhs) const;
};


struct COMMON_API RigidTransform
{
	Vector3		m_vPos;
	Quaternion	m_qRot;
};



struct COMMON_API  BoneTransform
{
	Vector3		m_vPos;
	Quaternion	m_qRot;
	float			m_fPropScale;
};


struct COMMON_API NodeTransform
{
	Vector3		m_vPos;
	Quaternion	m_qRot;
	float		m_fScale; //propagative scale

	Vector3		m_vLocalScale;

	static NodeTransform Identity();

};


struct COMMON_API Frame
{
	Vector3		m_vPos;
	Quaternion	m_qRot;
	float			m_fScale;

	void SetIdentity();

	void Zero();
};



inline void Lerp(Quaternion& out, const Quaternion& v0, const Quaternion& v1,float factor);


inline bool FloatEqual(float fa,float fb,float eps = FEPS);

inline bool IsNaN(Matrix4x4* pMat);

void Normalize(Vector3* pOut,const Vector3* pIn);

void Normalize(Quaternion* pOut,const Quaternion* pIn);

void Normalize(float* pOut,const float* pIn);

COMMON_API  void DbgDump(const RigidTransform& val);

//------------------------------------------------------------------------------
//Vector3
//------------------------------------------------------------------------------
Vector3 Vec3One();

Vector3 Vec3Zero();

Vector3 Vec3PositiveX();

Vector3 Vec3PositiveY();

Vector3 Vec3PositiveZ();

void Vec3Recip(Vector3* pOut,const Vector3* pIn);

void Vec3Add(Vector3* pOut,const Vector3* pV,float fVal);

void Vec3Mul(Vector3* pOut,const Vector3* pA,const Vector3* pB);

void Vec3Min(Vector3* pOut,const Vector3* pA,const Vector3* pB);

void Vec3Max(Vector3* pOut,const Vector3* pA,const Vector3* pB);

void Vec3Mad(Vector3* pV,const Vector3* pA,float f,const Vector3* pB);

void S3AVecClampLength(Vector3* outVec,const Vector3* inVec,float fMaxLen);

void Vec3TransformCoord(Vector3* pOut,const Vector3* pV,const RigidTransform* pRT);

void Vec3TransformNormal(Vector3* pOut,const Vector3* pV,const RigidTransform* pRT);

void Vec3TransformCoord(Vector3* pOut,const Vector3* pV,const NodeTransform* pRT);

void Vec3TransformNormal(Vector3* pOut,const Vector3* pV,const NodeTransform* pNode);




//------------------------------------------------------------------------------
//Vector4
//------------------------------------------------------------------------------
void Vec4SetPoint(Vector4* pVec4, const Vector3* pVec3);

void Vec4SetVector(Vector4* pVec4, const Vector3* pVec3);


//------------------------------------------------------------------------------
//EulerAngle
//------------------------------------------------------------------------------
COMMON_API void EulerAngleFromQuaternion(float* eulerVal,const Quaternion* pRot);
//
///*!
//\param fRoll
//alone Z axis
//\param fPitch
//alone X axis
//\param fYaw
//alone Y axis
//*/
//void EulerAngleFromQuaternion(float& fRoll,float& fPitch, float& fYaw,const Quaternion* inQua);

//axis is non zero
//x-roll = 0, y-pitch, z-yaw
void EulerAngleFromXToAxis(EulerAngleXYZ* pEuler,const Vector3* pAxis);

//------------------------------------------------------------------------------
//Quaternion
//------------------------------------------------------------------------------
Quaternion QuaternionIden();

COMMON_API void QuaternionFromEulerAngle(Quaternion* pRot,const float* eulerVal);

void QuaternionMad(Quaternion* pOut, const Quaternion* pQ1, const Quaternion* pQ2,float fWeight);

void QuaternionAdd(Quaternion* pOut, const Quaternion* pQ1, const Quaternion* pQ2);

void QuaternionTransformVector(Vector3* pOut,const Vector3* pV,const Quaternion* pQuat);

void TransformQuaternion(Quaternion* outQuat,const Quaternion* inQuat,const Matrix4x4* inMat);

void QuaternionLerp(Quaternion* out,const Quaternion* q0,const Quaternion* q1,float factor);


//pAxisFrom, pAxisTo should be normalized
void QuaternionFromAxisToAxis(Quaternion* pRot,const Vector3* pAxisFrom,const Vector3* pAxisTo);

void QuaternionFromAxisToAxis(Quaternion* pRot,const Vector3* pAxisFrom,const Vector3* pAxisTo,float fMaxAngle);


void QuaternionFromAxisToAxis(Quaternion* pRot,const Vector3* pAxisFrom,const Vector3* pAxisTo,const Vector3* pTangent);

void QuaternionFromAxisToAxis(Vector3* pAxisRot,float* pAngle,const Vector3* pAxisFrom,const Vector3* pAxisTo);



//------------------------------------------------------------------------------
//Node Transform
//------------------------------------------------------------------------------
void TransformSetIdentity(NodeTransform* pTSF);

void TransformFromMatrix(NodeTransform* pTSF,const Matrix4x4* pMat);

void TransformMul(NodeTransform* pOut,const NodeTransform* pTSFA,const NodeTransform* pTSFB);

void TransformMulLocalScale(NodeTransform* pOut,const NodeTransform* pTSFA,const NodeTransform* pTSFB);

//------------------------------------------------------------------------------
//Rigid Transform
//------------------------------------------------------------------------------
void TransformSetIdentity(RigidTransform* pRT);

void TransformFromMatrix(RigidTransform* pRT,const Matrix4x4* pMat);

void TransformInverse(RigidTransform* pOut,const RigidTransform* pRT);

void TransformMul(RigidTransform* pRT,const RigidTransform* pRTA,const RigidTransform* pRTB);

void Vec3TransformNormal(Vector3* pOut,const Vector3* pV,const NodeTransform* pNode);



//------------------------------------------------------------------------------
//Matrix4x4
//------------------------------------------------------------------------------
Matrix4x4 MatrixIdentity();

void MatrixAxis(Matrix4x4* pMat,const Vector3* pX,const Vector3* pY, const Vector3* pZ);

void MatrixTransform(Matrix4x4* outMat,const Quaternion* inRot,const Vector3* inPos);

COMMON_API bool MatrixDecompose(Vector3 *pOutScale, Quaternion *pOutRotation, Vector3 *pOutTranslation, const Matrix4x4 *pM);

void MatrixTransform(Matrix4x4* pOut,const Vector3* pScale,const Quaternion* pRot,const Vector3* pPos);

void MatrixTransform(Matrix4x4* outMat,const Quaternion* inRot,const Vector3* inPos);

void MatrixFromQuaternion(Matrix4x4* pMat,const Quaternion* pRot);

void MatrixFromFrame(Matrix4x4* pOut,const Frame* pFrame);


void BoneMatrixFromTransform( Matrix4x4* pMat,const NodeTransform* pTSF);

void NodeMatrixFromTransform( Matrix4x4* pMat,const NodeTransform* pTSF);


Vector3*		MatrixAsTranslation3(Matrix4x4* pMat);

const Vector3*	MatrixAsTranslation3(const Matrix4x4* pMat);

Vector3*		MatrixAsVector3(Matrix4x4* pMat,Uint nCol);

const Vector3*	MatrixAsVector3(const Matrix4x4* pMat,Uint nCol);

Vector4*		MatrixAsVector4(Matrix4x4* pMat,Uint nCol);

const Vector4*	MatrixAsVector4(const Matrix4x4* pMat,Uint nCol);

const Vector3*	MatrixAsTranslation(const Matrix4x4* pMat);

Vector3*		MatrixAsTranslation(Matrix4x4* pMat);


//------------------------------------------------------------------------------
//None inline Matrix4x4
//------------------------------------------------------------------------------
COMMON_API Matrix4x4* MatrixPerspectiveGL_RH(Matrix4x4* pOut,float w,float h,float zn,float zf);

COMMON_API Matrix4x4* MatrixPerspectiveFovGL_RH(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf);

COMMON_API Matrix4x4* MatrixPerspectiveGL_LH(Matrix4x4* pOut,float w,float h,float zn,float zf);

COMMON_API Matrix4x4* MatrixPerspectiveFovGL_LH(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf);

COMMON_API Matrix4x4* MatrixOrthoGL_LH(Matrix4x4* pOut,float w,float h,float zn,float zf);

COMMON_API Matrix4x4* MatrixOrthoOffCenterGL_LH(Matrix4x4* pOut,float l,float r, float b, float t, float zn,float zf);

//------------------------------------------------------------------------------
//Frame
//------------------------------------------------------------------------------

void FrameTransformPoint(Vector3* pOut,const Vector3* pP,const Frame* pFrame);


#ifdef WIN32
inline float round(float r)
{
	return (r > 0.0f) ? floor(r + 0.5f) : ceil(r - 0.5f);
}
#endif

#include "Math.inl"


#endif //__Math_h__