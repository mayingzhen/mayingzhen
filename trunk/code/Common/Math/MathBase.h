#ifndef __MathBase_h__
#define __MathBase_h__


//#include "BaseType.h"

//------------------------------------------------------------------------------
//D3DX9 Portable
//------------------------------------------------------------------------------
//#define inline inline

#include <math.h>
#include <float.h>

#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4201) // anonymous unions warning




#if defined(_X86_) || defined(_IA64_)
#pragma pack(4)
#endif

// D3DCOLOR is equivalent to D3DFMT_A8R8G8B8

typedef Uint32 COLOR;


struct Viewport {
	Uint32       X;
	Uint32       Y;            /* Viewport Top left */
	Uint32       Width;
	Uint32       Height;       /* Viewport Dimensions */
	float       MinZ;         /* Min/max of clip Volume */
	float       MaxZ;
} ;

#pragma pack()



struct Vector2;
struct Vector3;
struct Vector4;
struct Quaternion;
struct EulerAngleXYZ;
struct Matrix4x4;



//===========================================================================
//
// Vectors
//
//===========================================================================


//--------------------------
// 2D Vector
//--------------------------
struct Vector2
{
public:
	Vector2() {};
	Vector2( const float * );
	Vector2( float x, float y );

	// casting
	operator float* ();
	operator const float* () const;

	// assignment operators
	Vector2& operator += ( const Vector2& );
	Vector2& operator -= ( const Vector2& );
	Vector2& operator *= ( float );
	Vector2& operator /= ( float );

	// unary operators
	Vector2 operator + () const;
	Vector2 operator - () const;

	// binary operators
	Vector2 operator + ( const Vector2& ) const;
	Vector2 operator - ( const Vector2& ) const;
	Vector2 operator * ( float ) const;
	Vector2 operator / ( float ) const;

	friend Vector2 operator * ( float, const Vector2& );

	bool operator == ( const Vector2& ) const;
	bool operator != ( const Vector2& ) const;

	bool isZero() const;

	void rotate(const Vector2& point, float angle);

	static Vector2 zero() {return Vector2(0.0f,0.0f);}


public:

	float x, y;
};




//--------------------------
// 3D Vector
//--------------------------
struct COMMON_API Vector3 
{
public:
	Vector3() {};
	Vector3( const float * );
	Vector3( const Vector3& );
	Vector3( float x, float y, float z );

	// casting
	operator float* ();
	operator const float* () const;

	// assignment operators
	Vector3& operator += ( const Vector3& );
	Vector3& operator -= ( const Vector3& );
	Vector3& operator *= ( float );
	Vector3& operator /= ( float );

	// unary operators
	Vector3 operator + () const;
	Vector3 operator - () const;

	// binary operators
	Vector3 operator + ( const Vector3& ) const;
	Vector3 operator - ( const Vector3& ) const;
	Vector3 operator * ( float ) const;
	Vector3 operator / ( float ) const;

	friend Vector3 operator * ( float, const struct Vector3& );

	bool operator == ( const Vector3& ) const;
	bool operator != ( const Vector3& ) const;

public:
	float x;
	float y;
	float z;
};



//LHS system
inline const Vector3& Vec3Right() { static Vector3 temp(1.0f,0.0f,0.0f); return temp; }
inline const Vector3& Vec3Up() {  static Vector3 temp(0.0f,1.0f,0.0f); return temp;}
inline const Vector3& Vec3Forward() { static Vector3 temp(0.0f,0.0f,1.0f); return temp;}

inline const Vector3& Vec3Zero() { static Vector3 temp(0.0f,0.0f,0.0f); return temp; }
inline const Vector3& Vec3One() { static Vector3 temp(1.0f,1.0f,1.0f); return temp;}

inline Vector3 Vec3fromColor(unsigned int color)
{
	float components[3];
	int componentIndex = 0;
	for (int i = 2; i >= 0; --i)
	{
		int component = (color >> i*8) & 0x0000ff;

		components[componentIndex++] = static_cast<float>(component) / 255.0f;
	}

	Vector3 value(components);
	return value;
}

//--------------------------
// 4D Vector
//--------------------------
struct COMMON_API Vector4
{

public:
	Vector4() {};
	Vector4( const float* );
	Vector4( const Vector3& xyz, float w );
	Vector4( float x, float y, float z, float w );

	// casting
	operator float* ();
	operator const float* () const;

	// assignment operators
	Vector4& operator += ( const Vector4& );
	Vector4& operator -= ( const Vector4& );
	Vector4& operator *= ( float );
	Vector4& operator /= ( float );

	// unary operators
	Vector4 operator + () const;
	Vector4 operator - () const;

	// binary operators
	Vector4 operator + ( const Vector4& ) const;
	Vector4 operator - ( const Vector4& ) const;
	Vector4 operator * ( float ) const;
	Vector4 operator / ( float ) const;

	friend Vector4 operator * ( float, const Vector4& );

	bool operator == ( const Vector4& ) const;
	bool operator != ( const Vector4& ) const;

public:

	float x, y, z, w;
};


inline const Vector4& Vec4One() 
{
	static Vector4 vOne(1.0f,1.0f,1.0f,1.0f);
	return vOne;
}

inline const Vector4& Vec4Zero() 
{ 
	static Vector4 vZero(0.0f,0.0f,0.0f,0.0f);
	return vZero;
}

inline Vector4 Vec4fromColor(unsigned int color)
{
	float components[4];
	int componentIndex = 0;
	for (int i = 3; i >= 0; --i)
	{
		int component = (color >> i*8) & 0x000000ff;

		components[componentIndex++] = static_cast<float>(component) / 255.0f;
	}

	Vector4 value(components);
	return value;
}



//===========================================================================
//
// Matrices
//
//===========================================================================

struct COMMON_API Matrix4x4 
{
public:
	Matrix4x4() {};
	Matrix4x4( const float * );
	Matrix4x4( const Matrix4x4& );
	Matrix4x4( float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44 );


	// access grants
	float& operator () ( Uint Row, Uint Col );
	float  operator () ( Uint Row, Uint Col ) const;

	static const Matrix4x4& identity();

	Vector3 GetRow(Uint Row) const {return Vector3(m[Row][0],m[Row][1],m[Row][2]);}

	// casting operators
	operator float* ();
	operator const float* () const;

	// assignment operators
	Matrix4x4& operator *= ( const Matrix4x4& );
	Matrix4x4& operator += ( const Matrix4x4& );
	Matrix4x4& operator -= ( const Matrix4x4& );
	Matrix4x4& operator *= ( float );
	Matrix4x4& operator /= ( float );

	// unary operators
	Matrix4x4 operator + () const;
	Matrix4x4 operator - () const;

	// binary operators
	Matrix4x4 operator * ( const Matrix4x4& ) const;
	Matrix4x4 operator + ( const Matrix4x4& ) const;
	Matrix4x4 operator - ( const Matrix4x4& ) const;
	Matrix4x4 operator * ( float ) const;
	Matrix4x4 operator / ( float ) const;

	friend Matrix4x4 operator * ( float, const Matrix4x4& );

	bool operator == ( const Matrix4x4& ) const;
	bool operator != ( const Matrix4x4& ) const;

	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};

};



#if _MSC_VER >= 1300  // VC7
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16  // Earlier compiler may not understand this, do nothing.
#endif

typedef ALIGN16 Matrix4x4 Matrix4x4A;



//===========================================================================
//
//    Quaternions
//
//===========================================================================
struct COMMON_API Quaternion
{

public:
	Quaternion() {}
	Quaternion( const float * );
	Quaternion( float x, float y, float z, float w );

	// casting
	operator float* ();
	operator const float* () const;

	// assignment operators
	Quaternion& operator += ( const Quaternion& );
	Quaternion& operator -= ( const Quaternion& );
	Quaternion& operator *= ( const Quaternion& );
	Quaternion& operator *= ( float );
	Quaternion& operator /= ( float );

	// unary operators
	Quaternion  operator + () const;
	Quaternion  operator - () const;

	// binary operators
	Quaternion operator + ( const Quaternion& ) const;
	Quaternion operator - ( const Quaternion& ) const;
	Quaternion operator * ( const Quaternion& ) const;
	Quaternion operator * ( float ) const;
	Quaternion operator / ( float ) const;

	friend Quaternion operator * (float, const Quaternion& );

	bool operator == ( const Quaternion& ) const;
	bool operator != ( const Quaternion& ) const;


	float x, y, z, w;
};


//------------------------------------------------------------------------------
//EulerAngle
//------------------------------------------------------------------------------


//Rotate on x axis first, then y,z axis
//x-roll
//y-pitch
//z-yaw
struct EulerAngleXYZ
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




//===========================================================================
//
// Planes
//
//===========================================================================
struct Plane
{

public:
	Plane() {}
	Plane( const float* );
	Plane( float a, float b, float c, float d );

	// casting
	operator float* ();
	operator const float* () const;

	// assignment operators
	Plane& operator *= ( float );
	Plane& operator /= ( float );

	// unary operators
	Plane operator + () const;
	Plane operator - () const;

	// binary operators
	Plane operator * ( float ) const;
	Plane operator / ( float ) const;

	friend Plane operator * ( float, const Plane& );

	bool operator == ( const Plane& ) const;
	bool operator != ( const Plane& ) const;


	float a, b, c, d;
};


//===========================================================================
//
// Colors
//
//===========================================================================

struct Color
{

public:
	Color() {}
	Color( Uint32 argb );
	Color( const float * );
	Color( float r, float g, float b, float a );

	// casting
	operator Uint32 () const;

	operator float* ();
	operator const float* () const;


	// assignment operators
	Color& operator += ( const Color& );
	Color& operator -= ( const Color& );
	Color& operator *= ( float );
	Color& operator /= ( float );

	// unary operators
	Color operator + () const;
	Color operator - () const;

	// binary operators
	Color operator + ( const Color& ) const;
	Color operator - ( const Color& ) const;
	Color operator * ( float ) const;
	Color operator / ( float ) const;

	friend Color operator * ( float, const Color& );

	bool operator == ( const Color& ) const;
	bool operator != ( const Color& ) const;


	float r, g, b, a;
};



//===========================================================================
//
// D3DX math functions:
//
// NOTE:
//  * All these functions can take the same object as in and out parameters.
//
//  * Out parameters are typically also returned as return values, so that
//    the output of one function may be used as a parameter to another.
//
//===========================================================================

//--------------------------
// 2D Vector
//--------------------------

// inline

float Vec2Length
( const Vector2 *pV );

float Vec2LengthSq
( const Vector2 *pV );

float Vec2Dot
( const Vector2 *pV1, const Vector2 *pV2 );

// Z component of ((x1,y1,0) cross (x2,y2,0))
float Vec2CCW
( const Vector2 *pV1, const Vector2 *pV2 );

Vector2* Vec2Add
( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2 );

Vector2* Vec2Subtract
( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2 );

// Minimize each component.  x = min(x1, x2), y = min(y1, y2)
Vector2* Vec2Minimize
( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2 );

// Maximize each component.  x = max(x1, x2), y = max(y1, y2)
Vector2* Vec2Maximize
( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2 );

Vector2* Vec2Scale
( Vector2 *pOut, const Vector2 *pV, float s );

// Linear interpolation. V1 + s(V2-V1)
Vector2* Vec2Lerp
( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2,
 float s );

// non-inline
#ifdef __cplusplus
extern "C" {
#endif

	//Vector2*  Vec2Normalize
	//	( Vector2 *pOut, const Vector2 *pV );

	//// Hermite interpolation between position V1, tangent T1 (when s == 0)
	//// and position V2, tangent T2 (when s == 1).
	//Vector2*  Vec2Hermite
	//	( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pT1,
	//	const Vector2 *pV2, const Vector2 *pT2, float s );

	//// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	//Vector2*  Vec2CatmullRom
	//	( Vector2 *pOut, const Vector2 *pV0, const Vector2 *pV1,
	//	const Vector2 *pV2, const Vector2 *pV3, float s );

	//// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	//Vector2*  Vec2BaryCentric
	//	( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2,
	//	const Vector2 *pV3, float f, float g);

	//// Transform (x, y, 0, 1) by matrix.
	//Vector4*  Vec2Transform
	//	( Vector4 *pOut, const Vector2 *pV, const Matrix4x4 *pM );

	//// Transform (x, y, 0, 1) by matrix, project result back into w=1.
	//Vector2*  Vec2TransformCoord
	//	( Vector2 *pOut, const Vector2 *pV, const Matrix4x4 *pM );

	//// Transform (x, y, 0, 0) by matrix.
	//Vector2*  Vec2TransformNormal
	//	( Vector2 *pOut, const Vector2 *pV, const Matrix4x4 *pM );

	//// Transform Array (x, y, 0, 1) by matrix.
	//Vector4*  Vec2TransformArray
	//	( Vector4 *pOut, Uint OutStride, const Vector2 *pV, Uint VStride, const Matrix4x4 *pM, Uint n);

	//// Transform Array (x, y, 0, 1) by matrix, project result back into w=1.
	//Vector2*  Vec2TransformCoordArray
	//	( Vector2 *pOut, Uint OutStride, const Vector2 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );

	//// Transform Array (x, y, 0, 0) by matrix.
	//Vector2*  Vec2TransformNormalArray
	//	( Vector2 *pOut, Uint OutStride, const Vector2 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );



#ifdef __cplusplus
}
#endif


//--------------------------
// 3D Vector
//--------------------------

// inline

float Vec3Length
( const Vector3 *pV );

float Vec3LengthSq
( const Vector3 *pV );

float Vec3Dot
( const Vector3 *pV1, const Vector3 *pV2 );

Vector3* Vec3Cross
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 );

Vector3* Vec3Add
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 );

Vector3* Vec3Subtract
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 );

// Minimize each component.  x = min(x1, x2), y = min(y1, y2), ...
Vector3* Vec3Minimize
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 );

// Maximize each component.  x = max(x1, x2), y = max(y1, y2), ...
Vector3* Vec3Maximize
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 );

Vector3* Vec3Scale
( Vector3 *pOut, const Vector3 *pV, float s);

// Linear interpolation. V1 + s(V2-V1)
Vector3* Vec3Lerp
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2,
 float s );

// non-inline
//#ifdef __cplusplus
//extern "C" {
//#endif

	COMMON_API Vector3* Vec3Normalize( Vector3 *pOut, const Vector3 *pV );
    
    COMMON_API Vector3& Vec3Normalize( Vector3& vOut, const Vector3& vIn );

	COMMON_API Vector3  Vec3Normalize(const Vector3& vIn);

	//// Hermite interpolation between position V1, tangent T1 (when s == 0)
	//// and position V2, tangent T2 (when s == 1).
	//Vector3*  Vec3Hermite
	//	( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pT1,
	//	const Vector3 *pV2, const Vector3 *pT2, float s );

	//// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	//Vector3*  Vec3CatmullRom
	//	( Vector3 *pOut, const Vector3 *pV0, const Vector3 *pV1,
	//	const Vector3 *pV2, const Vector3 *pV3, float s );

	//// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	//Vector3*  Vec3BaryCentric
	//	( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2,
	//	const Vector3 *pV3, float f, float g);

	// Transform (x, y, z, 1) by matrix.
	COMMON_API Vector4*  Vec3Transform
		( Vector4 *pOut, const Vector3 *pV, const Matrix4x4 *pM );

	// Transform (x, y, z, 1) by matrix, project result back into w=1.
	COMMON_API Vector3*  Vec3TransformCoord
		( Vector3 *pOut, const Vector3 *pV, const Matrix4x4 *pM );

	// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
	// non-affine matrix, the matrix you pass to this function should be the 
	// transpose of the inverse of the matrix you would use to transform a coord.
	COMMON_API Vector3*  Vec3TransformNormal
		( Vector3 *pOut, const Vector3 *pV, const Matrix4x4 *pM );


	//// Transform Array (x, y, z, 1) by matrix. 
	//Vector4*  Vec3TransformArray
	//	( Vector4 *pOut, Uint OutStride, const Vector3 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );

	//// Transform Array (x, y, z, 1) by matrix, project result back into w=1.
	//Vector3*  Vec3TransformCoordArray
	//	( Vector3 *pOut, Uint OutStride, const Vector3 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );

	//// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
	//// non-affine matrix, the matrix you pass to this function should be the 
	//// transpose of the inverse of the matrix you would use to transform a coord.
	//Vector3*  Vec3TransformNormalArray
	//	( Vector3 *pOut, Uint OutStride, const Vector3 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );

	//// Project vector from object space into screen space
	//Vector3*  Vec3Project
	//	( Vector3 *pOut, const Vector3 *pV, const Viewport *pViewport,
	//	const Matrix4x4 *pProjection, const Matrix4x4 *pView, const Matrix4x4 *pWorld);

	//// Project vector from screen space into object space
	//Vector3*  Vec3Unproject
	//	( Vector3 *pOut, const Vector3 *pV, const Viewport *pViewport,
	//	const Matrix4x4 *pProjection, const Matrix4x4 *pView, const Matrix4x4 *pWorld);

	//// Project vector Array from object space into screen space
	//Vector3*  Vec3ProjectArray
	//	( Vector3 *pOut, Uint OutStride,const Vector3 *pV, Uint VStride,const Viewport *pViewport,
	//	const Matrix4x4 *pProjection, const Matrix4x4 *pView, const Matrix4x4 *pWorld, Uint n);

	//// Project vector Array from screen space into object space
	//Vector3*  Vec3UnprojectArray
	//	( Vector3 *pOut, Uint OutStride, const Vector3 *pV, Uint VStride, const Viewport *pViewport,
	//	const Matrix4x4 *pProjection, const Matrix4x4 *pView, const Matrix4x4 *pWorld, Uint n);


//#ifdef __cplusplus
//}
//#endif



//--------------------------
// 4D Vector
//--------------------------

// inline

float Vec4Length
( const Vector4 *pV );

float Vec4LengthSq
( const Vector4 *pV );

float Vec4Dot
( const Vector4 *pV1, const Vector4 *pV2 );

Vector4* Vec4Add
( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2);

Vector4* Vec4Subtract
( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2);

// Minimize each component.  x = min(x1, x2), y = min(y1, y2), ...
Vector4* Vec4Minimize
( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2);

// Maximize each component.  x = max(x1, x2), y = max(y1, y2), ...
Vector4* Vec4Maximize
( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2);

Vector4* Vec4Scale
( Vector4 *pOut, const Vector4 *pV, float s);

// Linear interpolation. V1 + s(V2-V1)
Vector4* Vec4Lerp
( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2,
 float s );

// non-inline
#ifdef __cplusplus
extern "C" {
#endif

	//// Cross-product in 4 dimensions.
	//Vector4*  Vec4Cross
	//	( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2,
	//	const Vector4 *pV3);

	//Vector4*  Vec4Normalize
	//	( Vector4 *pOut, const Vector4 *pV );

	//// Hermite interpolation between position V1, tangent T1 (when s == 0)
	//// and position V2, tangent T2 (when s == 1).
	//Vector4*  Vec4Hermite
	//	( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pT1,
	//	const Vector4 *pV2, const Vector4 *pT2, float s );

	//// CatmullRom interpolation between V1 (when s == 0) and V2 (when s == 1)
	//Vector4*  Vec4CatmullRom
	//	( Vector4 *pOut, const Vector4 *pV0, const Vector4 *pV1,
	//	const Vector4 *pV2, const Vector4 *pV3, float s );

	//// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
	//Vector4*  Vec4BaryCentric
	//	( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2,
	//	const Vector4 *pV3, float f, float g);

	//// Transform vector by matrix.
	//Vector4*  Vec4Transform
	//	( Vector4 *pOut, const Vector4 *pV, const Matrix4x4 *pM );

	//// Transform vector array by matrix.
	//Vector4*  Vec4TransformArray
	//	( Vector4 *pOut, Uint OutStride, const Vector4 *pV, Uint VStride, const Matrix4x4 *pM, Uint n );

#ifdef __cplusplus
}
#endif


//--------------------------
// 4D Matrix4x4
//--------------------------

// inline

Matrix4x4* MatrixIdentity
( Matrix4x4 *pOut );

bool MatrixIsIdentity
( const Matrix4x4 *pM );

void  MatrixFromEulerAngleXYZ(Matrix4x4* pMat,const EulerAngleXYZ* pEuler);

void  MatrixFromQuaternion(Matrix4x4* pMat,const Quaternion* pRot);

void  MatrixAddTranslation(Matrix4x4* pMat,const Vector3* pDeltaTsl);

// non-inline
#ifdef __cplusplus
extern "C" {
#endif

	COMMON_API float  MatrixDeterminant
		( const Matrix4x4 *pM );

	COMMON_API bool  MatrixDecompose
		( Vector3 *pOutScale, Quaternion *pOutRotation, 
		Vector3 *pOutTranslation, const Matrix4x4 *pM );

	COMMON_API Matrix4x4*  MatrixTranspose
		( Matrix4x4 *pOut, const Matrix4x4 *pM );

	// Matrix4x4 multiplication.  The result represents the transformation M2
	// followed by the transformation M1.  (Out = M1 * M2)
	COMMON_API Matrix4x4*  MatrixMultiply
		( Matrix4x4 *pOut, const Matrix4x4 *pM1, const Matrix4x4 *pM2 );

	//// Matrix4x4 multiplication, followed by a transpose. (Out = T(M1 * M2))
	//Matrix4x4*  MatrixMultiplyTranspose
	//	( Matrix4x4 *pOut, const Matrix4x4 *pM1, const Matrix4x4 *pM2 );

	// Calculate inverse of matrix.  Inversion my fail, in which case NULL will
	// be returned.  The determinant of pM is also returned it pfDeterminant
	// is non-NULL.
	COMMON_API Matrix4x4*  MatrixInverse
		( Matrix4x4 *pOut, float *pDeterminant, const Matrix4x4 *pM );

	// Build a matrix which scales by (sx, sy, sz)
	COMMON_API Matrix4x4*  MatrixScaling
		( Matrix4x4 *pOut, float sx, float sy, float sz );

	// Build a matrix which translates by (x, y, z)
	COMMON_API Matrix4x4*  MatrixTranslation
		( Matrix4x4 *pOut, float x, float y, float z );

	// Build a matrix which rotates around the X axis
	COMMON_API Matrix4x4* MatrixRotationX
		( Matrix4x4 *pOut, float Angle );

	// Build a matrix which rotates around the Y axis
	COMMON_API Matrix4x4* MatrixRotationY
		( Matrix4x4 *pOut, float Angle );

	// Build a matrix which rotates around the Z axis
	COMMON_API Matrix4x4* MatrixRotationZ
		( Matrix4x4 *pOut, float Angle );

	//// Build a matrix which rotates around an arbitrary axis
	COMMON_API Matrix4x4*  MatrixRotationAxis
		( Matrix4x4 *pOut, const Vector3 *pV, float Angle );

	// Build a matrix from a quaternion
	COMMON_API Matrix4x4* MatrixRotationQuaternion
		( Matrix4x4 *pOut, const Quaternion *pQ);

	// Yaw around the Y axis, a pitch around the X axis,
	// and a roll around the Z axis.
	COMMON_API Matrix4x4* MatrixRotationYawPitchRoll
		( Matrix4x4 *pOut, float Yaw, float Pitch, float Roll );

	// Build transformation matrix.  NULL arguments are treated as identity.
	// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
	COMMON_API Matrix4x4*  MatrixTransformation
		( Matrix4x4 *pOut, const Vector3 *pScalingCenter,
		const Quaternion *pScalingRotation, const Vector3 *pScaling,
		const Vector3 *pRotationCenter, const Quaternion *pRotation,
		const Vector3 *pTranslation);

	////// Build 2D transformation matrix in XY plane.  NULL arguments are treated as identity.
	////// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
	////Matrix4x4*  MatrixTransformation2D
	////	( Matrix4x4 *pOut, const Vector2* pScalingCenter, 
	////	float ScalingRotation, const Vector2* pScaling, 
	////	const Vector2* pRotationCenter, float Rotation, 
	////	const Vector2* pTranslation);

	//// Build affine transformation matrix.  NULL arguments are treated as identity.
	//// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
	//Matrix4x4*  MatrixAffineTransformation
	//	( Matrix4x4 *pOut, float Scaling, const Vector3 *pRotationCenter,
	//	const Quaternion *pRotation, const Vector3 *pTranslation);

	//// Build 2D affine transformation matrix in XY plane.  NULL arguments are treated as identity.
	//// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
	//Matrix4x4*  MatrixAffineTransformation2D
	//	( Matrix4x4 *pOut, float Scaling, const Vector2* pRotationCenter, 
	//	float Rotation, const Vector2* pTranslation);

	// Build a lookat matrix. (right-handed)
	COMMON_API Matrix4x4*  MatrixLookAtRH
		( Matrix4x4 *pOut, const Vector3 *pEye, const Vector3 *pAt,
		const Vector3 *pUp );

	// Build a lookat matrix. (left-handed)
	COMMON_API Matrix4x4*  MatrixLookAtLH
		( Matrix4x4 *pOut, const Vector3 *pEye, const Vector3 *pAt,
		const Vector3 *pUp );

	// Build a perspective projection matrix. (right-handed)
	COMMON_API Matrix4x4*  MatrixPerspectiveRH
		( Matrix4x4 *pOut, float w, float h, float zn, float zf );

	//// Build a perspective projection matrix. (left-handed)
	//Matrix4x4*  MatrixPerspectiveLH
	//	( Matrix4x4 *pOut, float w, float h, float zn, float zf );

	// Build a perspective projection matrix. (right-handed)
	COMMON_API Matrix4x4*  MatrixPerspectiveFovRH
		( Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf );

	// Build a perspective projection matrix. (left-handed)
	COMMON_API Matrix4x4*  MatrixPerspectiveFovLH
		( Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf );

	//// Build a perspective projection matrix. (right-handed)
	//Matrix4x4*  MatrixPerspectiveOffCenterRH
	//	( Matrix4x4 *pOut, float l, float r, float b, float t, float zn,
	//	float zf );

	//// Build a perspective projection matrix. (left-handed)
	//Matrix4x4*  MatrixPerspectiveOffCenterLH
	//	( Matrix4x4 *pOut, float l, float r, float b, float t, float zn,
	//	float zf );

	//// Build an ortho projection matrix. (right-handed)
	//Matrix4x4*  MatrixOrthoRH
	//	( Matrix4x4 *pOut, float w, float h, float zn, float zf );

	//// Build an ortho projection matrix. (left-handed)
	//Matrix4x4*  MatrixOrthoLH
	//	( Matrix4x4 *pOut, float w, float h, float zn, float zf );

	//// Build an ortho projection matrix. (right-handed)
	//Matrix4x4*  MatrixOrthoOffCenterRH
	//	( Matrix4x4 *pOut, float l, float r, float b, float t, float zn,
	//	float zf );

	//// Build an ortho projection matrix. (left-handed)
	//Matrix4x4*  MatrixOrthoOffCenterLH
	//	( Matrix4x4 *pOut, float l, float r, float b, float t, float zn,
	//	float zf );

	//// Build a matrix which flattens geometry into a plane, as if casting
	//// a shadow from a light.
	//Matrix4x4*  MatrixShadow
	//	( Matrix4x4 *pOut, const Vector4 *pLight,
	//	const Plane *pPlane );

	//// Build a matrix which reflects the coordinate system about a plane
	//Matrix4x4*  MatrixReflect
	//	( Matrix4x4 *pOut, const Plane *pPlane );

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
//Scalar
//------------------------------------------------------------------------------

bool IsNaN(float fVal);

float Abs(float val);


template<class T>
inline T InvalidID()
{
	return T(-1);
}

template<class T>
inline bool IsValidID(T ind)
{
	return ind != (T)(-1);
}

template<class T>
inline bool IsInvalidID(T ind)
{
	return ind == (T)(-1);
}


template<class T>
inline const T& Min(const T& a,const T& b)
{
	return a < b ? a : b;
}

template<class T>
inline const T& Max(const T& a,const T& b)
{
	return a > b ? a : b;
}

template<class T>
inline T Lerp(const T& v0,const T& v1,float factor)
{
	return v0*(1.0f-factor)+v1*factor;
}

template<class T>
inline void Lerp(T& out, const T& v0,const T& v1,float factor)
{
	out = v0*(1.0f-factor)+v1*factor;
}


float InvSqrt(float v);

float Clamp(float val,float fMin,float fMax);


//--------------------------
// Quaternion
//--------------------------

// inline

float QuaternionLength
( const Quaternion *pQ );

// Length squared, or "norm"
float QuaternionLengthSq
( const Quaternion *pQ );

float QuaternionDot
( const Quaternion *pQ1, const Quaternion *pQ2 );

// (0, 0, 0, 1)
Quaternion* QuaternionIdentity
( Quaternion *pOut );

bool QuaternionIsIdentity
( const Quaternion *pQ );

// (-x, -y, -z, w)
Quaternion* QuaternionConjugate
( Quaternion *pOut, const Quaternion *pQ );


// non-inline
#ifdef __cplusplus
extern "C" {
#endif

	//// Compute a quaternin's axis and angle of rotation. Expects unit quaternions.
	//void  QuaternionToAxisAngle
	//	( const Quaternion *pQ, Vector3 *pAxis, float *pAngle );

	// Build a quaternion from a rotation matrix.
	COMMON_API Quaternion*  QuaternionRotationMatrix
		( Quaternion *pOut, const Matrix4x4 *pM);

	// Rotation about arbitrary axis.
	COMMON_API Quaternion*  QuaternionRotationAxis
		( Quaternion *pOut, const Vector3 *pV, float Angle );

	//// Yaw around the Y axis, a pitch around the X axis,
	//// and a roll around the Z axis.
	//Quaternion*  QuaternionRotationYawPitchRoll
	//	( Quaternion *pOut, float Yaw, float Pitch, float Roll );

	// Quaternion multiplication.  The result represents the rotation Q2
	// followed by the rotation Q1.  (Out = Q2 * Q1)
	COMMON_API Quaternion*  QuaternionMultiply
		( Quaternion *pOut, const Quaternion *pQ1,
		const Quaternion *pQ2 );

	COMMON_API Quaternion*  QuaternionNormalize
		( Quaternion *pOut, const Quaternion *pQ );

	// Conjugate and re-norm
	COMMON_API Quaternion*  QuaternionInverse
		( Quaternion *pOut, const Quaternion *pQ );

	//// Expects unit quaternions.
	//// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
	//Quaternion*  QuaternionLn
	//	( Quaternion *pOut, const Quaternion *pQ );

	//// Expects pure quaternions. (w == 0)  w is ignored in calculation.
	//// if q = (0, theta * v); exp(q) = (cos(theta), sin(theta) * v)
	//Quaternion*  QuaternionExp
	//	( Quaternion *pOut, const Quaternion *pQ );

	// Spherical linear interpolation between Q1 (t == 0) and Q2 (t == 1).
	// Expects unit quaternions.
	COMMON_API Quaternion*  QuaternionSlerp
		( Quaternion *pOut, const Quaternion *pQ1,
		const Quaternion *pQ2, float t );

	//// Spherical quadrangle interpolation.
	//// Slerp(Slerp(Q1, C, t), Slerp(A, B, t), 2t(1-t))
	//Quaternion*  QuaternionSquad
	//	( Quaternion *pOut, const Quaternion *pQ1,
	//	const Quaternion *pA, const Quaternion *pB,
	//	const Quaternion *pC, float t );

	//// Setup control points for spherical quadrangle interpolation
	//// from Q1 to Q2.  The control points are chosen in such a way 
	//// to ensure the continuity of tangents with adjacent segments.
	//void  QuaternionSquadSetup
	//	( Quaternion *pAOut, Quaternion *pBOut, Quaternion *pCOut,
	//	const Quaternion *pQ0, const Quaternion *pQ1, 
	//	const Quaternion *pQ2, const Quaternion *pQ3 );

	//// Barycentric interpolation.
	//// Slerp(Slerp(Q1, Q2, f+g), Slerp(Q1, Q3, f+g), g/(f+g))
	//Quaternion*  QuaternionBaryCentric
	//	( Quaternion *pOut, const Quaternion *pQ1,
	//	const Quaternion *pQ2, const Quaternion *pQ3,
	//	float f, float g );

#ifdef __cplusplus
}
#endif


//------------------------------------------------------------------------------
//Euler Angle
//------------------------------------------------------------------------------
void  QuaternionFromEulerAngleXYZ(Quaternion* pQuat,const EulerAngleXYZ* pEuler);

void  EulerAngleXYZIdentity(EulerAngleXYZ* pEuler);

void  EulerAngleFromQuaternion(EulerAngleXYZ* pEuler,const Quaternion* pQua);


//--------------------------
// Plane
//--------------------------

// inline

// ax + by + cz + dw
float PlaneDot
( const Plane *pP, const Vector4 *pV);

// ax + by + cz + d
float PlaneDotCoord
( const Plane *pP, const Vector3 *pV);

// ax + by + cz
float PlaneDotNormal
( const Plane *pP, const Vector3 *pV);

Plane* PlaneScale
(Plane *pOut, const Plane *pP, float s);

// non-inline
#ifdef __cplusplus
extern "C" {
#endif

	//// Normalize plane (so that |a,b,c| == 1)
	//Plane*  PlaneNormalize
	//	( Plane *pOut, const Plane *pP);

	//// Find the intersection between a plane and a line.  If the line is
	//// parallel to the plane, NULL is returned.
	//Vector3*  PlaneIntersectLine
	//	( Vector3 *pOut, const Plane *pP, const Vector3 *pV1,
	//	const Vector3 *pV2);

	//// Construct a plane from a point and a normal
	//Plane*  PlaneFromPointNormal
	//	( Plane *pOut, const Vector3 *pPoint, const Vector3 *pNormal);

	//// Construct a plane from 3 points
	//Plane*  PlaneFromPoints
	//	( Plane *pOut, const Vector3 *pV1, const Vector3 *pV2,
	//	const Vector3 *pV3);

	//// Transform a plane by a matrix.  The vector (a,b,c) must be normal.
	//// M should be the inverse transpose of the transformation desired.
	//Plane*  PlaneTransform
	//	( Plane *pOut, const Plane *pP, const Matrix4x4 *pM );

	//// Transform an array of planes by a matrix.  The vectors (a,b,c) must be normal.
	//// M should be the inverse transpose of the transformation desired.
	//Plane*  PlaneTransformArray
	//	( Plane *pOut, Uint OutStride, const Plane *pP, Uint PStride, const Matrix4x4 *pM, Uint n );

#ifdef __cplusplus
}
#endif


//--------------------------
// Color
//--------------------------

// inline

// (1-r, 1-g, 1-b, a)
Color* ColorNegative
(Color *pOut, const Color *pC);

Color* ColorAdd
(Color *pOut, const Color *pC1, const Color *pC2);

Color* ColorSubtract
(Color *pOut, const Color *pC1, const Color *pC2);

Color* ColorScale
(Color *pOut, const Color *pC, float s);

// (r1*r2, g1*g2, b1*b2, a1*a2)
Color* ColorModulate
(Color *pOut, const Color *pC1, const Color *pC2);

// Linear interpolation of r,g,b, and a. C1 + s(C2-C1)
Color* ColorLerp
(Color *pOut, const Color *pC1, const Color *pC2, float s);

// non-inline
#ifdef __cplusplus
extern "C" {
#endif

	//// Interpolate r,g,b between desaturated color and color.
	//// DesaturatedColor + s(Color - DesaturatedColor)
	//Color*  ColorAdjustSaturation
	//	(Color *pOut, const Color *pC, float s);

	//// Interpolate r,g,b between 50% grey and color.  Grey + s(Color - Grey)
	//Color*  ColorAdjustContrast
	//	(Color *pOut, const Color *pC, float c);

#ifdef __cplusplus
}
#endif





#include "MathBase.inl"

#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4201)
#endif


#endif //__MathBase_h__