#include "MathBase.h"

//------------------------------------------------------------------------------
//Vector2
//------------------------------------------------------------------------------

// non-inline
#ifdef __cplusplus
extern "C" {
#endif

	Vector2*  Vec2Normalize
		( Vector2 *pOut, const Vector2 *pV );

	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	Vector2*  Vec2Hermite
		( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pT1,
		const Vector2 *pV2, const Vector2 *pT2, float s );

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	Vector2*  Vec2CatmullRom
		( Vector2 *pOut, const Vector2 *pV0, const Vector2 *pV1,
		const Vector2 *pV2, const Vector2 *pV3, float s );

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	Vector2*  Vec2BaryCentric
		( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2,
		const Vector2 *pV3, float f, float g);

	// Transform (x, y, 0, 1) by matrix.
	Vector4*  Vec2Transform
		( Vector4 *pOut, const Vector2 *pV, const Matrix4x4 *pM );

	// Transform (x, y, 0, 1) by matrix, project result back into w=1.
	Vector2*  Vec2TransformCoord
		( Vector2 *pOut, const Vector2 *pV, const Matrix4x4 *pM );

	// Transform (x, y, 0, 0) by matrix.
	Vector2*  Vec2TransformNormal
		( Vector2 *pOut, const Vector2 *pV, const Matrix4x4 *pM );

	// Transform Array (x, y, 0, 1) by matrix.
	Vector4*  Vec2TransformArray
		( Vector4 *pOut, Uint OutStride, const Vector2 *pV, Uint VStride, const Matrix4x4 *pM, Uint n);

	// Transform Array (x, y, 0, 1) by matrix, project result back into w=1.
	Vector2*  Vec2TransformCoordArray
		( Vector2 *pOut, Uint OutStride, const Vector2 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );

	// Transform Array (x, y, 0, 0) by matrix.
	Vector2*  Vec2TransformNormalArray
		( Vector2 *pOut, Uint OutStride, const Vector2 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );

//------------------------------------------------------------------------------
//Vector3
//------------------------------------------------------------------------------

	COMMON_API Vector3*  Vec3Normalize
		( Vector3 *pOut, const Vector3 *pV )
	{
		float fLenSq = Vec3LengthSq(pV);
		if( fLenSq > FEPS )
		{
			const float Scale = InvSqrt(fLenSq);
			pOut->x = pV->x * Scale; 
			pOut->y = pV->y * Scale; 
			pOut->z = pV->z * Scale;
		}else{
			pOut->x = 1.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
		}
		return pOut;
	}

	// Hermite interpolation between position V1, tangent T1 (when s == 0)
	// and position V2, tangent T2 (when s == 1).
	Vector3*  Vec3Hermite
		( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pT1,
		const Vector3 *pV2, const Vector3 *pT2, float s );

	// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	Vector3*  Vec3CatmullRom
		( Vector3 *pOut, const Vector3 *pV0, const Vector3 *pV1,
		const Vector3 *pV2, const Vector3 *pV3, float s );

	// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	Vector3*  Vec3BaryCentric
		( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2,
		const Vector3 *pV3, float f, float g);

	// Transform (x, y, z, 1) by matrix.
	COMMON_API Vector4*  Vec3Transform
		( Vector4 *pOut, const Vector3 *pV, const Matrix4x4 *pM )
	{
		float x = pM->m[0][0] * pV->x + pM->m[1][0] * pV->y + pM->m[2][0] * pV->z + pM->m[3][0];
		float y = pM->m[0][1] * pV->x + pM->m[1][1] * pV->y + pM->m[2][1] * pV->z + pM->m[3][1];
		float z = pM->m[0][2] * pV->x + pM->m[1][2] * pV->y + pM->m[2][2] * pV->z + pM->m[3][2];
		float w = pM->m[0][3] * pV->x + pM->m[1][3] * pV->y + pM->m[2][3] * pV->z + pM->m[3][3];
		pOut->x = x;
		pOut->y = y;
		pOut->z = z;
		pOut->w = w;
		return pOut;
	}

	// Transform (x, y, z, 1) by matrix, project result back into w=1.
	COMMON_API Vector3*  Vec3TransformCoord
		( Vector3 *pOut, const Vector3 *pV, const Matrix4x4 *pM )
	{
		float w = 1.0f / (pM->m[0][3] * pV->x + pM->m[1][3] * pV->y + pM->m[2][3] * pV->z + pM->m[3][3]);
		float x = (pM->m[0][0] * pV->x + pM->m[1][0] * pV->y + pM->m[2][0] * pV->z + pM->m[3][0]) * w;
		float y = (pM->m[0][1] * pV->x + pM->m[1][1] * pV->y + pM->m[2][1] * pV->z + pM->m[3][1]) * w;
		float z = (pM->m[0][2] * pV->x + pM->m[1][2] * pV->y + pM->m[2][2] * pV->z + pM->m[3][2]) * w;
		pOut->x = x;
		pOut->y = y;
		pOut->z = z;
		return pOut;
	}

	// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
	// non-affine matrix, the matrix you pass to this function should be the 
	// transpose of the inverse of the matrix you would use to transform a coord.
	COMMON_API Vector3*  Vec3TransformNormal
		( Vector3 *pOut, const Vector3 *pV, const Matrix4x4 *pM )
	{
		float w = 1.0f / (pM->m[0][3] * pV->x + pM->m[1][3] * pV->y + pM->m[2][3] * pV->z + pM->m[3][3]);
		float x = (pM->m[0][0] * pV->x + pM->m[1][0] * pV->y + pM->m[2][0] * pV->z) * w;
		float y = (pM->m[0][1] * pV->x + pM->m[1][1] * pV->y + pM->m[2][1] * pV->z) * w;
		float z = (pM->m[0][2] * pV->x + pM->m[1][2] * pV->y + pM->m[2][2] * pV->z) * w;
		pOut->x = x;
		pOut->y = y;
		pOut->z = z;	
		return pOut;
	}


	// Transform Array (x, y, z, 1) by matrix. 
	Vector4*  Vec3TransformArray
		( Vector4 *pOut, Uint OutStride, const Vector3 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );

	// Transform Array (x, y, z, 1) by matrix, project result back into w=1.
	Vector3*  Vec3TransformCoordArray
		( Vector3 *pOut, Uint OutStride, const Vector3 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );

	// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
	// non-affine matrix, the matrix you pass to this function should be the 
	// transpose of the inverse of the matrix you would use to transform a coord.
	Vector3*  Vec3TransformNormalArray
		( Vector3 *pOut, Uint OutStride, const Vector3 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );

	// Project vector from object space into screen space
	Vector3*  Vec3Project
		( Vector3 *pOut, const Vector3 *pV, const Viewport *pViewport,
		const Matrix4x4 *pProjection, const Matrix4x4 *pView, const Matrix4x4 *pWorld);

	// Project vector from screen space into object space
	Vector3*  Vec3Unproject
		( Vector3 *pOut, const Vector3 *pV, const Viewport *pViewport,
		const Matrix4x4 *pProjection, const Matrix4x4 *pView, const Matrix4x4 *pWorld);

	// Project vector Array from object space into screen space
	Vector3*  Vec3ProjectArray
		( Vector3 *pOut, Uint OutStride,const Vector3 *pV, Uint VStride,const Viewport *pViewport,
		const Matrix4x4 *pProjection, const Matrix4x4 *pView, const Matrix4x4 *pWorld, Uint n);

	// Project vector Array from screen space into object space
	Vector3*  Vec3UnprojectArray
		( Vector3 *pOut, Uint OutStride, const Vector3 *pV, Uint VStride, const Viewport *pViewport,
		const Matrix4x4 *pProjection, const Matrix4x4 *pView, const Matrix4x4 *pWorld, Uint n);


//------------------------------------------------------------------------------
//Quaternion
//------------------------------------------------------------------------------

	// Compute a quaternin's axis and angle of rotation. Expects unit quaternions.
	void  QuaternionToAxisAngle
		( const Quaternion *pQ, Vector3 *pAxis, float *pAngle );

	// Build a quaternion from a rotation matrix.
	COMMON_API Quaternion*  QuaternionRotationMatrix
		( Quaternion *pOut, const Matrix4x4 *pM)
	{

		//const MeReal *const t = (MeReal *) tm;
		float	s;

		// Check diagonal (trace)
		const float tr = pM->m[0][0] + pM->m[1][1] + pM->m[2][2];

		if (tr > 0.0f) 
		{
			float InvS = InvSqrt(tr + 1.f);
			pOut->w = 0.5f * (1.f / InvS);
			s = 0.5f * InvS;

			pOut->x = (pM->m[1][2] - pM->m[2][1]) * s;
			pOut->y = (pM->m[2][0] - pM->m[0][2]) * s;
			pOut->z = (pM->m[0][1] - pM->m[1][0]) * s;
		} 
		else 
		{
			// diagonal is negative
			int i = 0;

			if (pM->m[1][1] > pM->m[0][0])
				i = 1;

			if (pM->m[2][2] > pM->m[i][i])
				i = 2;

			static const int nxt[3] = { 1, 2, 0 };
			const int j = nxt[i];
			const int k = nxt[j];

			s = pM->m[i][i] - pM->m[j][j] - pM->m[k][k] + 1.0f;

			float InvS = InvSqrt(s);

			float qt[4];
			qt[i] = 0.5f * (1.f / InvS);

			s = 0.5f * InvS;

			qt[3] = (pM->m[j][k] - pM->m[k][j]) * s;
			qt[j] = (pM->m[i][j] + pM->m[j][i]) * s;
			qt[k] = (pM->m[i][k] + pM->m[k][i]) * s;

			pOut->x = qt[0];
			pOut->y = qt[1];
			pOut->z = qt[2];
			pOut->w = qt[3];
		}

		return pOut;
	}

	// Rotation about arbitrary axis.
	COMMON_API Quaternion*  QuaternionRotationAxis
		( Quaternion *pOut, const Vector3 *pV, float Angle )
	{
		const float half_a = 0.5f * Angle;
		const float s = sin(half_a);
		const float c = cos(half_a);

		pOut->x = s * pV->x;
		pOut->y = s * pV->y;
		pOut->z = s * pV->z;
		pOut->w = c;
		return pOut;
	}

	// Yaw around the Y axis, a pitch around the X axis,
	// and a roll around the Z axis.
	Quaternion*  QuaternionRotationYawPitchRoll
		( Quaternion *pOut, float Yaw, float Pitch, float Roll );

	// Quaternion multiplication.  The result represents the rotation Q2
	// followed by the rotation Q1.  (Out = Q2 * Q1)
	COMMON_API Quaternion*  QuaternionMultiply
		( Quaternion *pOut, const Quaternion *pQ1,
		const Quaternion *pQ2 )
	{
	

		const float* A = (const float*)pQ2;
		const float* B = (const float*)pQ1;
		float* R = (float*)pOut;

		const float T0 = (A[2] - A[1]) * (B[1] - B[2]);
		const float T1 = (A[3] + A[0]) * (B[3] + B[0]);
		const float T2 = (A[3] - A[0]) * (B[1] + B[2]);
		const float T3 = (A[1] + A[2]) * (B[3] - B[0]);
		const float T4 = (A[2] - A[0]) * (B[0] - B[1]);
		const float T5 = (A[2] + A[0]) * (B[0] + B[1]);
		const float T6 = (A[3] + A[1]) * (B[3] - B[2]);
		const float T7 = (A[3] - A[1]) * (B[3] + B[2]);
		const float T8 = T5 + T6 + T7;
		const float T9 = 0.5f * (T4 + T8);

		R[0] = T1 + T9 - T8;
		R[1] = T2 + T9 - T7;
		R[2] = T3 + T9 - T6;
		R[3] = T0 + T9 - T5;

		return pOut;
	}

	COMMON_API Quaternion*  QuaternionNormalize
		( Quaternion *pOut, const Quaternion *pQ )
	{
		float len = QuaternionLength(pQ);
		float factor = 1.0f / sqrt(len);
		pOut->x = pQ->x * factor;
		pOut->y = pQ->y * factor;
		pOut->z = pQ->z * factor;
		pOut->w = pQ->w * factor;

		return pOut;
	}

	// Conjugate and re-norm
	COMMON_API Quaternion*  QuaternionInverse
		( Quaternion *pOut, const Quaternion *pQ )
	{
		float fLen = QuaternionLength(pQ);
		if ( fLen > 0.0 )
		{
			float fInvLen = 1.0f/fLen;
			pOut->x = - pQ->x * fInvLen;
			pOut->y = - pQ->y * fInvLen;
			pOut->z = - pQ->z * fInvLen;
			pOut->w = pQ->w * fInvLen;
		}
		else
		{
			// return an invalid result to flag the error
			pOut->x = - pQ->x;
			pOut->y = - pQ->y;
			pOut->z = - pQ->z;
			pOut->w = pQ->w;
		}

		return pOut;
	}

	// Expects unit quaternions.
	// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
	Quaternion*  QuaternionLn
		( Quaternion *pOut, const Quaternion *pQ );

	// Expects pure quaternions. (w == 0)  w is ignored in calculation.
	// if q = (0, theta * v); exp(q) = (cos(theta), sin(theta) * v)
	Quaternion*  QuaternionExp
		( Quaternion *pOut, const Quaternion *pQ );

	// Spherical linear interpolation between Q1 (t == 0) and Q2 (t == 1).
	// Expects unit quaternions.
	COMMON_API Quaternion*  QuaternionSlerp
		( Quaternion *pOut, const Quaternion *pQ1,
		const Quaternion *pQ2, float t )
	{
		
		const float CosAngle = Clamp(QuaternionDot(pQ1,pQ2), -1.f, 1.f);
		const float Angle = acos(CosAngle);

		//debugf( TEXT("CosAngle: %f Angle: %f"), CosAngle, Angle );

		if ( abs(Angle) < FEPS )
		{
			*pOut = *pQ1;
			return pOut;
		}

		const float SinAngle = sin(Angle);
		const float InvSinAngle = 1.f/SinAngle;

		const float Scale0 = sin((1.0f-t)*Angle)*InvSinAngle;
		const float Scale1 = sin(t*Angle)*InvSinAngle;

		pOut->x = pQ1->x * Scale0 + pQ2->x * Scale1;
		pOut->y = pQ1->y * Scale0 + pQ2->y * Scale1;
		pOut->z = pQ1->z * Scale0 + pQ2->z * Scale1;
		pOut->w = pQ1->w * Scale0 + pQ2->w * Scale1;

		return pOut;

	}

	// Spherical quadrangle interpolation.
	// Slerp(Slerp(Q1, C, t), Slerp(A, B, t), 2t(1-t))
	Quaternion*  QuaternionSquad
		( Quaternion *pOut, const Quaternion *pQ1,
		const Quaternion *pA, const Quaternion *pB,
		const Quaternion *pC, float t );

	// Setup control points for spherical quadrangle interpolation
	// from Q1 to Q2.  The control points are chosen in such a way 
	// to ensure the continuity of tangents with adjacent segments.
	void  QuaternionSquadSetup
		( Quaternion *pAOut, Quaternion *pBOut, Quaternion *pCOut,
		const Quaternion *pQ0, const Quaternion *pQ1, 
		const Quaternion *pQ2, const Quaternion *pQ3 );

	// Barycentric interpolation.
	// Slerp(Slerp(Q1, Q2, f+g), Slerp(Q1, Q3, f+g), g/(f+g))
	Quaternion*  QuaternionBaryCentric
		( Quaternion *pOut, const Quaternion *pQ1,
		const Quaternion *pQ2, const Quaternion *pQ3,
		float f, float g );


//------------------------------------------------------------------------------
//Matrix4x4
//------------------------------------------------------------------------------
COMMON_API float  MatrixDeterminant
( const Matrix4x4 *pM )
{
	float fA0 = pM->m[0][0]*pM->m[1][1] - pM->m[0][1]*pM->m[1][0];
	float fA1 = pM->m[0][0]*pM->m[1][2] - pM->m[0][2]*pM->m[1][0];
	float fA2 = pM->m[0][0]*pM->m[1][3] - pM->m[0][3]*pM->m[1][0];
	float fA3 = pM->m[0][1]*pM->m[1][2] - pM->m[0][2]*pM->m[1][1];
	float fA4 = pM->m[0][1]*pM->m[1][3] - pM->m[0][3]*pM->m[1][1];
	float fA5 = pM->m[0][2]*pM->m[1][3] - pM->m[0][3]*pM->m[1][2];
	float fB0 = pM->m[2][0]*pM->m[3][1] - pM->m[2][1]*pM->m[3][0];
	float fB1 = pM->m[2][0]*pM->m[3][2] - pM->m[2][2]*pM->m[3][0];
	float fB2 = pM->m[2][0]*pM->m[3][3] - pM->m[2][3]*pM->m[3][0];
	float fB3 = pM->m[2][1]*pM->m[3][2] - pM->m[2][2]*pM->m[3][1];
	float fB4 = pM->m[2][1]*pM->m[3][3] - pM->m[2][3]*pM->m[3][1];
	float fB5 = pM->m[2][2]*pM->m[3][3] - pM->m[2][3]*pM->m[3][2];
	return fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
}

//assume kQ is 3x3 matrix , use [4][4] for to avoid memcpy only.
//kD : float[3]
//kU : float[3]
void ExtractMatrix3(Matrix4x4* pOut,const Matrix4x4* pM) 
{
	pOut->_11 = pM->_11; pOut->_12 = pM->_12; pOut->_13 = pM->_13; pOut->_14 = 0.0f;
	pOut->_21 = pM->_21; pOut->_22 = pM->_22; pOut->_23 = pM->_23; pOut->_24 = 0.0f;
	pOut->_31 = pM->_31; pOut->_32 = pM->_32; pOut->_33 = pM->_33; pOut->_34 = 0.0f;
	pOut->_41 = 0.0f; pOut->_42 = 0.0f; pOut->_43 = 0.0f; pOut->_44 = 0.0f;
}

COMMON_API bool  MatrixDecompose
( Vector3 *pOutScale, Quaternion *pOutRotation, 
 Vector3 *pOutTranslation, const Matrix4x4 *pM )
{
	Matrix4x4 matRot;

	if (NULL == pOutScale || NULL == pOutRotation || NULL == pOutTranslation)
	{
		return false;
	}

	MatrixIdentity(&matRot);
	

	float (*kQ)[4] = matRot.m;
	
	float* kD = &pOutScale->x;
	float kU[3]; //shear;
	//UD decompose
	{

		// Factor M = QR = QDU where Q is orthogonal, D is diagonal,
		// and U is upper triangular with ones on its diagonal.  Algorithm uses
		// Gram-Schmidt orthogonalization (the QR algorithm).
		//
		// If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
		//
		//   q0 = m0/|m0|
		//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
		//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
		//
		// where |V| indicates length of vector V and A*B indicates dot
		// product of vectors A and B.  The matrix R has entries
		//
		//   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
		//   r10 = 0      r11 = q1*m1  r12 = q1*m2
		//   r20 = 0      r21 = 0      r22 = q2*m2
		//
		// so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
		// u02 = r02/r00, and u12 = r12/r11.

		// Q = rotation
		// D = scaling
		// U = shear

		// D stores the three diagonal entries r00, r11, r22
		// U stores the entries U[0] = u01, U[1] = u02, U[2] = u12

		// build orthogonal matrix Q
		float fInvLength = InvSqrt(pM->m[0][0]*pM->m[0][0]
		+ pM->m[1][0]*pM->m[1][0] +
			pM->m[2][0]*pM->m[2][0]);
		kQ[0][0] = pM->m[0][0]*fInvLength;
		kQ[1][0] = pM->m[1][0]*fInvLength;
		kQ[2][0] = pM->m[2][0]*fInvLength;

		float fDot = kQ[0][0]*pM->m[0][1] + kQ[1][0]*pM->m[1][1] +
			kQ[2][0]*pM->m[2][1];
		kQ[0][1] = pM->m[0][1]-fDot*kQ[0][0];
		kQ[1][1] = pM->m[1][1]-fDot*kQ[1][0];
		kQ[2][1] = pM->m[2][1]-fDot*kQ[2][0];
		fInvLength = InvSqrt(kQ[0][1]*kQ[0][1] + kQ[1][1]*kQ[1][1] +
			kQ[2][1]*kQ[2][1]);
		kQ[0][1] *= fInvLength;
		kQ[1][1] *= fInvLength;
		kQ[2][1] *= fInvLength;

		fDot = kQ[0][0]*pM->m[0][2] + kQ[1][0]*pM->m[1][2] +
			kQ[2][0]*pM->m[2][2];
		kQ[0][2] = pM->m[0][2]-fDot*kQ[0][0];
		kQ[1][2] = pM->m[1][2]-fDot*kQ[1][0];
		kQ[2][2] = pM->m[2][2]-fDot*kQ[2][0];
		fDot = kQ[0][1]*pM->m[0][2] + kQ[1][1]*pM->m[1][2] +
			kQ[2][1]*pM->m[2][2];
		kQ[0][2] -= fDot*kQ[0][1];
		kQ[1][2] -= fDot*kQ[1][1];
		kQ[2][2] -= fDot*kQ[2][1];
		fInvLength = InvSqrt(kQ[0][2]*kQ[0][2] + kQ[1][2]*kQ[1][2] +
			kQ[2][2]*kQ[2][2]);
		kQ[0][2] *= fInvLength;
		kQ[1][2] *= fInvLength;
		kQ[2][2] *= fInvLength;

		// guarantee that orthogonal matrix has determinant 1 (no reflections)
		float fDet = kQ[0][0]*kQ[1][1]*kQ[2][2] + kQ[0][1]*kQ[1][2]*kQ[2][0] +
			kQ[0][2]*kQ[1][0]*kQ[2][1] - kQ[0][2]*kQ[1][1]*kQ[2][0] -
			kQ[0][1]*kQ[1][0]*kQ[2][2] - kQ[0][0]*kQ[1][2]*kQ[2][1];

		if ( fDet < 0.0F )
		{
			for (size_t iRow = 0; iRow < 3; iRow++)
				for (size_t iCol = 0; iCol < 3; iCol++)
					kQ[iRow][iCol] = -kQ[iRow][iCol];
		}

		// build "right" matrix R;
		float kR[3][3];
		kR[0][0] = kQ[0][0]*pM->m[0][0] + kQ[1][0]*pM->m[1][0] +
			kQ[2][0]*pM->m[2][0];
		kR[0][1] = kQ[0][0]*pM->m[0][1] + kQ[1][0]*pM->m[1][1] +
			kQ[2][0]*pM->m[2][1];
		kR[1][1] = kQ[0][1]*pM->m[0][1] + kQ[1][1]*pM->m[1][1] +
			kQ[2][1]*pM->m[2][1];
		kR[0][2] = kQ[0][0]*pM->m[0][2] + kQ[1][0]*pM->m[1][2] +
			kQ[2][0]*pM->m[2][2];
		kR[1][2] = kQ[0][1]*pM->m[0][2] + kQ[1][1]*pM->m[1][2] +
			kQ[2][1]*pM->m[2][2];
		kR[2][2] = kQ[0][2]*pM->m[0][2] + kQ[1][2]*pM->m[1][2] +
			kQ[2][2]*pM->m[2][2];

		// the scaling component
		kD[0] = kR[0][0];
		kD[1] = kR[1][1];
		kD[2] = kR[2][2];

		// the shear component
		float fInvD0 = 1.0f/kD[0];
		kU[0] = kR[0][1]*fInvD0;
		kU[1] = kR[0][2]*fInvD0;
		kU[2] = kR[1][2]/kD[1];
	}


	QuaternionRotationMatrix(pOutRotation,&matRot);
	//pOutRotation->w = - pOutRotation->w;


	pOutTranslation->x = pM->_41;
	pOutTranslation->y = pM->_42;
	pOutTranslation->z = pM->_43;


	return 0;
}

COMMON_API Matrix4x4*  MatrixTranspose
( Matrix4x4 *pOut, const Matrix4x4 *pM )
{

	Matrix4x4 tmp;
	const Matrix4x4* pSrc;
	if (pOut == pM)
	{
		tmp = *pM;
		pSrc = &tmp;
	}else{
		pSrc = pM;
	}

	pOut->_11 = pSrc->_11; pOut->_12 = pSrc->_21; pOut->_13 = pSrc->_31; pOut->_14 = pSrc->_41;
	pOut->_21 = pSrc->_12; pOut->_22 = pSrc->_22; pOut->_23 = pSrc->_32; pOut->_24 = pSrc->_42;
	pOut->_31 = pSrc->_13; pOut->_32 = pSrc->_23; pOut->_33 = pSrc->_33; pOut->_34 = pSrc->_43;
	pOut->_41 = pSrc->_14; pOut->_42 = pSrc->_24; pOut->_43 = pSrc->_34; pOut->_44 = pSrc->_44;
	return pOut;
	
}

// Matrix4x4 multiplication.  The result represents the transformation M2
// followed by the transformation M1.  (Out = M1 * M2)
COMMON_API Matrix4x4*  MatrixMultiply
( Matrix4x4 *pOut, const Matrix4x4 *pM1, const Matrix4x4 *pM2 )
{
	Matrix4x4 dst;
	dst._11 = pM1->_11 * pM2->_11 + pM1->_12 * pM2->_21 + pM1->_13 * pM2->_31 + pM1->_14 * pM2->_41;
	dst._12 = pM1->_11 * pM2->_12 + pM1->_12 * pM2->_22 + pM1->_13 * pM2->_32 + pM1->_14 * pM2->_42;
	dst._13 = pM1->_11 * pM2->_13 + pM1->_12 * pM2->_23 + pM1->_13 * pM2->_33 + pM1->_14 * pM2->_43;
	dst._14 = pM1->_11 * pM2->_14 + pM1->_12 * pM2->_24 + pM1->_13 * pM2->_34 + pM1->_14 * pM2->_44;

	dst._21 = pM1->_21 * pM2->_11 + pM1->_22 * pM2->_21 + pM1->_23 * pM2->_31 + pM1->_24 * pM2->_41;
	dst._22 = pM1->_21 * pM2->_12 + pM1->_22 * pM2->_22 + pM1->_23 * pM2->_32 + pM1->_24 * pM2->_42;
	dst._23 = pM1->_21 * pM2->_13 + pM1->_22 * pM2->_23 + pM1->_23 * pM2->_33 + pM1->_24 * pM2->_43;
	dst._24 = pM1->_21 * pM2->_14 + pM1->_22 * pM2->_24 + pM1->_23 * pM2->_34 + pM1->_24 * pM2->_44;

	dst._31 = pM1->_31 * pM2->_11 + pM1->_32 * pM2->_21 + pM1->_33 * pM2->_31 + pM1->_34 * pM2->_41;
	dst._32 = pM1->_31 * pM2->_12 + pM1->_32 * pM2->_22 + pM1->_33 * pM2->_32 + pM1->_34 * pM2->_42;
	dst._33 = pM1->_31 * pM2->_13 + pM1->_32 * pM2->_23 + pM1->_33 * pM2->_33 + pM1->_34 * pM2->_43;
	dst._34 = pM1->_31 * pM2->_14 + pM1->_32 * pM2->_24 + pM1->_33 * pM2->_34 + pM1->_34 * pM2->_44;

	dst._41 = pM1->_41 * pM2->_11 + pM1->_42 * pM2->_21 + pM1->_43 * pM2->_31 + pM1->_44 * pM2->_41;
	dst._42 = pM1->_41 * pM2->_12 + pM1->_42 * pM2->_22 + pM1->_43 * pM2->_32 + pM1->_44 * pM2->_42;
	dst._43 = pM1->_41 * pM2->_13 + pM1->_42 * pM2->_23 + pM1->_43 * pM2->_33 + pM1->_44 * pM2->_43;
	dst._44 = pM1->_41 * pM2->_14 + pM1->_42 * pM2->_24 + pM1->_43 * pM2->_34 + pM1->_44 * pM2->_44;

	*pOut = dst; 
	return pOut;
}

// Matrix4x4 multiplication, followed by a transpose. (Out = T(M1 * M2))
Matrix4x4*  MatrixMultiplyTranspose
( Matrix4x4 *pOut, const Matrix4x4 *pM1, const Matrix4x4 *pM2 );

// Calculate inverse of matrix.  Inversion my fail, in which case NULL will
// be returned.  The determinant of pM is also returned it pfDeterminant
// is non-NULL.
COMMON_API Matrix4x4*  MatrixInverse
( Matrix4x4 *pOut, float *pDeterminant, const Matrix4x4 *pM )
{

	float fInvDet = 1.0f / MatrixDeterminant(pM);

	float fA0 = pM->m[0][0]*pM->m[1][1] - pM->m[0][1]*pM->m[1][0];
	float fA1 = pM->m[0][0]*pM->m[1][2] - pM->m[0][2]*pM->m[1][0];
	float fA2 = pM->m[0][0]*pM->m[1][3] - pM->m[0][3]*pM->m[1][0];
	float fA3 = pM->m[0][1]*pM->m[1][2] - pM->m[0][2]*pM->m[1][1];
	float fA4 = pM->m[0][1]*pM->m[1][3] - pM->m[0][3]*pM->m[1][1];
	float fA5 = pM->m[0][2]*pM->m[1][3] - pM->m[0][3]*pM->m[1][2];
	float fB0 = pM->m[2][0]*pM->m[3][1] - pM->m[2][1]*pM->m[3][0];
	float fB1 = pM->m[2][0]*pM->m[3][2] - pM->m[2][2]*pM->m[3][0];
	float fB2 = pM->m[2][0]*pM->m[3][3] - pM->m[2][3]*pM->m[3][0];
	float fB3 = pM->m[2][1]*pM->m[3][2] - pM->m[2][2]*pM->m[3][1];
	float fB4 = pM->m[2][1]*pM->m[3][3] - pM->m[2][3]*pM->m[3][1];
	float fB5 = pM->m[2][2]*pM->m[3][3] - pM->m[2][3]*pM->m[3][2];

	Matrix4x4 mat;
	mat.m[0][0] = + pM->m[1][1]*fB5 - pM->m[1][2]*fB4 + pM->m[1][3]*fB3;
	mat.m[1][0] = - pM->m[1][0]*fB5 + pM->m[1][2]*fB2 - pM->m[1][3]*fB1;
	mat.m[2][0] = + pM->m[1][0]*fB4 - pM->m[1][1]*fB2 + pM->m[1][3]*fB0;
	mat.m[3][0] = - pM->m[1][0]*fB3 + pM->m[1][1]*fB1 - pM->m[1][2]*fB0;
	mat.m[0][1] = - pM->m[0][1]*fB5 + pM->m[0][2]*fB4 - pM->m[0][3]*fB3;
	mat.m[1][1] = + pM->m[0][0]*fB5 - pM->m[0][2]*fB2 + pM->m[0][3]*fB1;
	mat.m[2][1] = - pM->m[0][0]*fB4 + pM->m[0][1]*fB2 - pM->m[0][3]*fB0;
	mat.m[3][1] = + pM->m[0][0]*fB3 - pM->m[0][1]*fB1 + pM->m[0][2]*fB0;
	mat.m[0][2] = + pM->m[3][1]*fA5 - pM->m[3][2]*fA4 + pM->m[3][3]*fA3;
	mat.m[1][2] = - pM->m[3][0]*fA5 + pM->m[3][2]*fA2 - pM->m[3][3]*fA1;
	mat.m[2][2] = + pM->m[3][0]*fA4 - pM->m[3][1]*fA2 + pM->m[3][3]*fA0;
	mat.m[3][2] = - pM->m[3][0]*fA3 + pM->m[3][1]*fA1 - pM->m[3][2]*fA0;
	mat.m[0][3] = - pM->m[2][1]*fA5 + pM->m[2][2]*fA4 - pM->m[2][3]*fA3;
	mat.m[1][3] = + pM->m[2][0]*fA5 - pM->m[2][2]*fA2 + pM->m[2][3]*fA1;
	mat.m[2][3] = - pM->m[2][0]*fA4 + pM->m[2][1]*fA2 - pM->m[2][3]*fA0;
	mat.m[3][3] = + pM->m[2][0]*fA3 - pM->m[2][1]*fA1 + pM->m[2][2]*fA0;

	*pOut = mat * fInvDet;
	return pOut;
}

// Build a matrix which scales by (sx, sy, sz)
COMMON_API Matrix4x4*  MatrixScaling
( Matrix4x4 *pOut, float sx, float sy, float sz )
{
	pOut->_11 = sx;   pOut->_12 = 0.0f; pOut->_13 = 0.0f; pOut->_14 = 0.0f;

	pOut->_21 = 0.0f; pOut->_22 = sy;	pOut->_23 = 0.0f; pOut->_24 = 0.0f;

	pOut->_31 = 0.0f; pOut->_32 = 0.0f; pOut->_33 = sz;	  pOut->_34 = 0.0f;

	pOut->_41 = 0.0f; pOut->_42 = 0.0f; pOut->_43 = 0.0f; pOut->_44 = 1.0f;
	return pOut;
}

// Build a matrix which translates by (x, y, z)
COMMON_API Matrix4x4*  MatrixTranslation
( Matrix4x4 *pOut, float x, float y, float z )
{
	pOut->_11 = 1.0f; pOut->_12 = 0.0f; pOut->_13 = 0.0f; pOut->_14 = 0.0f;

	pOut->_21 = 0.0f; pOut->_22 = 1.0f; pOut->_23 = 0.0f; pOut->_24 = 0.0f;

	pOut->_31 = 0.0f; pOut->_32 = 0.0f; pOut->_33 = 1.0f; pOut->_34 = 0.0f;

	pOut->_41 = x; pOut->_42 = y; pOut->_43 = z; pOut->_44 = 1.0f;

	return pOut;
}

// Build a matrix which rotates around the X axis
Matrix4x4*  MatrixRotationX
( Matrix4x4 *pOut, float Angle )
{
	const float cx = cosf(Angle);
	const float sx = sinf(Angle);

	pOut->_11 = 1.0f;
	pOut->_12 = 0.0f;
	pOut->_13 = 0.0f;
	pOut->_14 = 0.0f;

	pOut->_21 = 0.0f;
	pOut->_22 = cx;
	pOut->_23 = sx;
	pOut->_24 = 0.0f;

	pOut->_31 = 0.0f;
	pOut->_32 = -sx;
	pOut->_33 = cx;
	pOut->_34 = 0.0f;

	pOut->_41 = 0.0f;
	pOut->_42 = 0.0f;
	pOut->_43 = 0.0f;
	pOut->_44 = 1.0f;

	return pOut;
}

// Build a matrix which rotates around the Y axis
Matrix4x4*  MatrixRotationY
( Matrix4x4 *pOut, float Angle )
{

	const float cy = cosf(Angle);
	const float sy = sinf(Angle);
	pOut->_11 = cy;
	pOut->_12 = 0.0f;
	pOut->_13 = -sy;
	pOut->_14 = 0.0f;

	pOut->_21 = 0.0f;
	pOut->_22 = 1.0f;
	pOut->_23 = 0.0f;
	pOut->_24 = 0.0f;

	pOut->_31 = sy;
	pOut->_32 = 0.0f;
	pOut->_33 = cy;
	pOut->_34 = 0.0f;

	pOut->_41 = 0.0f;
	pOut->_42 = 0.0f;
	pOut->_43 = 0.0f;
	pOut->_44 = 1.0f;

	return pOut;
}

// Build a matrix which rotates around the Z axis
Matrix4x4*  MatrixRotationZ
( Matrix4x4 *pOut, float Angle )
{

	const float cz = cosf(Angle);
	const float sz = sinf(Angle);
	pOut->_11 = cz;
	pOut->_12 = sz;
	pOut->_13 = 0.0f;
	pOut->_14 = 0.0f;

	pOut->_21 = -sz;
	pOut->_22 = cz;
	pOut->_23 = 0.0f;
	pOut->_24 = 0.0f;

	pOut->_31 = 0.0f;
	pOut->_32 = 0.0f;
	pOut->_33 = 1.0f;
	pOut->_34 = 0.0f;

	pOut->_41 = 0.0f;
	pOut->_42 = 0.0f;
	pOut->_43 = 0.0f;
	pOut->_44 = 1.0f;

	return pOut;
}

// Build a matrix which rotates around an arbitrary axis
Matrix4x4*  MatrixRotationAxis
( Matrix4x4 *pOut, const Vector3 *pV, float Angle );


// Build a matrix from a quaternion
COMMON_API Matrix4x4*  MatrixRotationQuaternion
( Matrix4x4 *pOut, const Quaternion *pQ)
{
	const float x2 = pQ->x + pQ->x;  const float y2 = pQ->y + pQ->y;  const float z2 = pQ->z + pQ->z;
	const float xx = pQ->x * x2;   const float xy = pQ->x * y2;   const float xz = pQ->x * z2;
	const float yy = pQ->y * y2;   const float yz = pQ->y * z2;   const float zz = pQ->z * z2;
	const float wx = pQ->w * x2;   const float wy = pQ->w * y2;   const float wz = pQ->w * z2;

	pOut->m[0][0] = 1.0f - (yy + zz);	pOut->m[1][0] = xy - wz;				pOut->m[2][0] = xz + wy;			pOut->m[3][0] = 0.0f;
	pOut->m[0][1] = xy + wz;			pOut->m[1][1] = 1.0f - (xx + zz);		pOut->m[2][1] = yz - wx;			pOut->m[3][1] = 0.0f;
	pOut->m[0][2] = xz - wy;			pOut->m[1][2] = yz + wx;				pOut->m[2][2] = 1.0f - (xx + yy);	pOut->m[3][2] = 0.0f;
	pOut->m[0][3] = 0.0f;				pOut->m[1][3] = 0.0f;					pOut->m[2][3] = 0.0f;				pOut->m[3][3] = 1.0f;

	return pOut;
}

// Yaw around the Y axis, a pitch around the X axis,
// and a roll around the Z axis.
COMMON_API Matrix4x4*  MatrixRotationYawPitchRoll
( Matrix4x4 *pOut, float Yaw, float Pitch, float Roll )
{
	Matrix4x4 matX;
	Matrix4x4 matY;
	Matrix4x4 matZ;
	MatrixRotationX(&matX,Pitch);
	MatrixRotationY(&matY,Yaw);
	MatrixRotationZ(&matZ,Roll);

	MatrixMultiply(pOut,&matX,&matY);
	MatrixMultiply(pOut,pOut,&matZ);
	return pOut;
}

// Build transformation matrix.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
COMMON_API Matrix4x4*  MatrixTransformation
( Matrix4x4 *pOut, const Vector3 *pScalingCenter,
 const Quaternion *pScalingRotation, const Vector3 *pScaling,
 const Vector3 *pRotationCenter, const Quaternion *pRotation,
 const Vector3 *pTranslation)
{
	Matrix4x4 matTemp;

	// 1
	if(pScalingCenter != NULL)
	{
		MatrixTranslation(pOut, -pScalingCenter->x, -pScalingCenter->y, -pScalingCenter->z); 
	}
	else
	{
		MatrixIdentity(pOut);
	}

	// 2
	if(pScaling)
	{
		MatrixScaling(&matTemp, pScaling->x, pScaling->y, pScaling->z);
		MatrixMultiply(pOut, pOut, &matTemp);
	}

	// 3
	if(pScalingCenter)
	{
		Vec3Add((Vector3*)&pOut->_41, (Vector3*)&pOut->_41, pScalingCenter);
	}

	// 4
	if(pRotationCenter)
	{
		const Vector3 vTemp(-pRotationCenter->x, -pRotationCenter->y, -pRotationCenter->z);
		Vec3Add((Vector3*)&pOut->_41, (Vector3*)&pOut->_41, &vTemp);
	}

	// 5
	if(pRotation)
	{
		MatrixRotationQuaternion(&matTemp, pRotation);
		MatrixMultiply(pOut, pOut, &matTemp);
	}

	// 6
	if(pRotationCenter)
	{
		Vec3Add((Vector3*)&pOut->_41, (Vector3*)&pOut->_41, pRotationCenter);
	}

	// 7
	if(pTranslation)
	{
		::Vec3Add((Vector3*)&pOut->_41, (Vector3*)&pOut->_41, pTranslation);
	}
	return pOut;
}

// Build 2D transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
Matrix4x4*  MatrixTransformation2D
( Matrix4x4 *pOut, const Vector2* pScalingCenter, 
 float ScalingRotation, const Vector2* pScaling, 
 const Vector2* pRotationCenter, float Rotation, 
 const Vector2* pTranslation);

//// Build affine transformation matrix.  NULL arguments are treated as identity.
//// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
//Matrix4x4*  MatrixAffineTransformation
//( Matrix4x4 *pOut, float Scaling, const Vector3 *pRotationCenter,
// const Quaternion *pRotation, const Vector3 *pTranslation);

// Build 2D affine transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
Matrix4x4*  MatrixAffineTransformation2D
( Matrix4x4 *pOut, float Scaling, const Vector2* pRotationCenter, 
 float Rotation, const Vector2* pTranslation);

// Build a lookat matrix. (right-handed)
COMMON_API Matrix4x4*  MatrixLookAtRH
( Matrix4x4 *pOut, const Vector3 *pEye, const Vector3 *pAt,
 const Vector3 *pUp )
{
	if (pOut == NULL || pEye == NULL || pAt == NULL || pUp == NULL)
	{
		return NULL;
	}

	Vector3 vXAxis,vYAxis,vZAxis;
	vZAxis = *pEye - *pAt;
	Vec3Normalize(&vZAxis,&vZAxis);
	Vec3Cross(&vXAxis,pUp,&vZAxis);
	Vec3Normalize(&vXAxis,&vXAxis);
	Vec3Cross(&vYAxis,&vZAxis,&vXAxis);
	Vec3Normalize(&vYAxis,&vYAxis);

	pOut->_11 = vXAxis.x; pOut->_12 = vYAxis.x; pOut->_13 = vZAxis.x; pOut->_14 = 0.0f;

	pOut->_21 = vXAxis.y; pOut->_22 = vYAxis.y; pOut->_23 = vZAxis.y; pOut->_24 = 0.0f;

	pOut->_31 = vXAxis.z; pOut->_32 = vYAxis.z; pOut->_33 = vZAxis.z; pOut->_34 = 0.0f;

	pOut->_41 = - Vec3Dot(&vXAxis,pEye);
	pOut->_42 = - Vec3Dot(&vYAxis,pEye);
	pOut->_43 = - Vec3Dot(&vZAxis,pEye);
	pOut->_44 = 1.0f;

	return pOut;
}



// Build a lookat matrix. (left-handed)
Matrix4x4*  MatrixLookAtLH
( Matrix4x4 *pOut, const Vector3 *pEye, const Vector3 *pAt,
 const Vector3 *pUp )
{
	if (pOut == NULL || pEye == NULL || pAt == NULL || pUp == NULL)
	{
		return NULL;
	}

	Vector3 vXAxis,vYAxis,vZAxis;
	vZAxis = *pAt - *pEye;
	Vec3Normalize(&vZAxis,&vZAxis);
	Vec3Cross(&vXAxis,pUp,&vZAxis);
	Vec3Normalize(&vXAxis,&vXAxis);
	Vec3Cross(&vYAxis,&vZAxis,&vXAxis);
	Vec3Normalize(&vYAxis,&vYAxis);

	pOut->_11 = vXAxis.x; pOut->_12 = vYAxis.x; pOut->_13 = vZAxis.x; pOut->_14 = 0.0f;

	pOut->_21 = vXAxis.y; pOut->_22 = vYAxis.y; pOut->_23 = vZAxis.y; pOut->_24 = 0.0f;

	pOut->_31 = vXAxis.z; pOut->_32 = vYAxis.z; pOut->_33 = vZAxis.z; pOut->_34 = 0.0f;

	pOut->_41 = - Vec3Dot(&vXAxis,pEye);
	pOut->_42 = - Vec3Dot(&vYAxis,pEye);
	pOut->_43 = - Vec3Dot(&vZAxis,pEye);
	pOut->_44 = 1.0f;

	return pOut;
}

// Build a perspective projection matrix. (right-handed)
COMMON_API Matrix4x4*  MatrixPerspectiveRH
( Matrix4x4 *pOut, float w, float h, float zn, float zf )
{
	if(NULL == pOut)
	{
		return NULL;
	}

	float inv = 1.0f / (zn - zf);

	pOut->_11 = 2.0f * zn/w;	pOut->_12 = 0.0f;		pOut->_13 = 0.0f;			pOut->_14 = 0.0f;
	pOut->_21 = 0.0f;			pOut->_22 = 2.0f*zn/h;	pOut->_23 = 0.0f;			pOut->_24 = 0.0f;
	pOut->_31 = 0.0f;			pOut->_32 = 0.0f;		pOut->_33 = zf * inv;		pOut->_34 = -1.0f;
	pOut->_41 = 0.0f;			pOut->_42 = 0.0f;		pOut->_43 = zn*zf*inv;		pOut->_44 = 0.0f;

	return pOut;
}

// Build a perspective projection matrix. (left-handed)
COMMON_API Matrix4x4*  MatrixPerspectiveLH
( Matrix4x4 *pOut, float w, float h, float zn, float zf )
{
	if(NULL == pOut)
	{
		return NULL;
	}

	float inv = 1.0f / (zn - zf);

	pOut->_11 = 2.0f * zn/w;	pOut->_12 = 0.0f;		pOut->_13 = 0.0f;			pOut->_14 = 0.0f;
	pOut->_21 = 0.0f;			pOut->_22 = 2.0f*zn/h;	pOut->_23 = 0.0f;			pOut->_24 = 0.0f;
	pOut->_31 = 0.0f;			pOut->_32 = 0.0f;		pOut->_33 = - zf * inv;		pOut->_34 = 1.0f;
	pOut->_41 = 0.0f;			pOut->_42 = 0.0f;		pOut->_43 = zn*zf*inv;		pOut->_44 = 0.0f;

	return pOut;
}

// Build a perspective projection matrix. (right-handed)
COMMON_API Matrix4x4*  MatrixPerspectiveFovRH
( Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf )
{
	if (NULL == pOut)
	{
		return NULL;
	}

	float yScale = tan(0.5F*PI - fovy * 0.5f);
	float xScale = yScale / Aspect;
	float inv = 1.0f / (zn - zf);

	pOut->_11 = xScale;	pOut->_12 = 0.0f;	pOut->_13 = 0.0f;			pOut->_14 = 0.0f;
	pOut->_21 = 0.0f;	pOut->_22 = yScale;	pOut->_23 = 0.0f;			pOut->_24 = 0.0f;
	pOut->_31 = 0.0f;	pOut->_32 = 0.0f;	pOut->_33 = zf * inv;		pOut->_34 = -1.0f;
	pOut->_41 = 0.0f;	pOut->_42 = 0.0f;	pOut->_43 = zn*zf*inv;		pOut->_44 = 0.0f;

	return pOut;
}

// Build a perspective projection matrix. (left-handed)
COMMON_API Matrix4x4*  MatrixPerspectiveFovLH
( Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf )
{
	if (NULL == pOut)
	{
		return NULL;
	}

	float yScale = tan(0.5F*PI - fovy * 0.5f);
	float xScale = yScale / Aspect;
	float inv = 1.0f / (zn - zf);

	pOut->_11 = xScale;	pOut->_12 = 0.0f;	pOut->_13 = 0.0f;			pOut->_14 = 0.0f;
	pOut->_21 = 0.0f;	pOut->_22 = yScale;	pOut->_23 = 0.0f;			pOut->_24 = 0.0f;
	pOut->_31 = 0.0f;	pOut->_32 = 0.0f;	pOut->_33 = - zf * inv;		pOut->_34 = 1.0f;
	pOut->_41 = 0.0f;	pOut->_42 = 0.0f;	pOut->_43 = zn*zf*inv;		pOut->_44 = 0.0f;

	return pOut;
}

// Build a perspective projection matrix. (right-handed)
Matrix4x4*  MatrixPerspectiveOffCenterRH
( Matrix4x4 *pOut, float l, float r, float b, float t, float zn,
 float zf );

// Build a perspective projection matrix. (left-handed)
Matrix4x4*  MatrixPerspectiveOffCenterLH
( Matrix4x4 *pOut, float l, float r, float b, float t, float zn,
 float zf );

// Build an ortho projection matrix. (right-handed)
Matrix4x4*  MatrixOrthoRH
( Matrix4x4 *pOut, float w, float h, float zn, float zf );

// Build an ortho projection matrix. (left-handed)
Matrix4x4*  MatrixOrthoLH
( Matrix4x4 *pOut, float w, float h, float zn, float zf );

// Build an ortho projection matrix. (right-handed)
Matrix4x4*  MatrixOrthoOffCenterRH
( Matrix4x4 *pOut, float l, float r, float b, float t, float zn,
 float zf );

// Build an ortho projection matrix. (left-handed)
Matrix4x4*  MatrixOrthoOffCenterLH
( Matrix4x4 *pOut, float l, float r, float b, float t, float zn,
 float zf );

// Build a matrix which flattens geometry into a plane, as if casting
// a shadow from a light.
Matrix4x4*  MatrixShadow
( Matrix4x4 *pOut, const Vector4 *pLight,
 const Plane *pPlane );

// Build a matrix which reflects the coordinate system about a plane
Matrix4x4*  MatrixReflect
( Matrix4x4 *pOut, const Plane *pPlane );


#ifdef __cplusplus
}
#endif