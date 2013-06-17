#ifndef __MathPortable_INL__
#define __MathPortable_INL__



//===========================================================================
//
// Inline RTTIClass Methods
//
//===========================================================================

#ifdef __cplusplus


//--------------------------
// 2D Vector
//--------------------------

inline
Vector2::Vector2( const float *pf )
{
#ifdef _DEBUG
	if(!pf)
		return;
#endif

	x = pf[0];
	y = pf[1];
}

inline
Vector2::Vector2( float fx, float fy )
{
	x = fx;
	y = fy;
}


// casting
inline
Vector2::operator float* ()
{
	return (float *) &x;
}

inline
Vector2::operator const float* () const
{
	return (const float *) &x;
}


// assignment operators
inline Vector2&
Vector2::operator += ( const Vector2& v )
{
	x += v.x;
	y += v.y;
	return *this;
}

inline Vector2&
Vector2::operator -= ( const Vector2& v )
{
	x -= v.x;
	y -= v.y;
	return *this;
}

inline Vector2&
Vector2::operator *= ( float f )
{
	x *= f;
	y *= f;
	return *this;
}

inline Vector2&
Vector2::operator /= ( float f )
{
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	return *this;
}


// unary operators
inline Vector2
Vector2::operator + () const
{
	return *this;
}

inline Vector2
Vector2::operator - () const
{
	return Vector2(-x, -y);
}


// binary operators
inline Vector2
Vector2::operator + ( const Vector2& v ) const
{
	return Vector2(x + v.x, y + v.y);
}

inline Vector2
Vector2::operator - ( const Vector2& v ) const
{
	return Vector2(x - v.x, y - v.y);
}

inline Vector2
Vector2::operator * ( float f ) const
{
	return Vector2(x * f, y * f);
}

inline Vector2
Vector2::operator / ( float f ) const
{
	float fInv = 1.0f / f;
	return Vector2(x * fInv, y * fInv);
}

inline Vector2
operator * ( float f, const Vector2& v )
{
	return Vector2(f * v.x, f * v.y);
}

inline bool
Vector2::operator == ( const Vector2& v ) const
{
	return x == v.x && y == v.y;
}

inline bool
Vector2::operator != ( const Vector2& v ) const
{
	return x != v.x || y != v.y;
}

inline bool Vector2::isZero() const
{
	return x == 0.0f && y == 0.0f;
}

inline void Vector2::rotate(const Vector2& point, float angle)
{
	float sinAngle = sin(angle);
	float cosAngle = cos(angle);

	if (point.isZero())
	{
		float tempX = x * cosAngle - y * sinAngle;
		y = y * cosAngle + x * sinAngle;
		x = tempX;
	}
	else
	{
		float tempX = x - point.x;
		float tempY = y - point.y;

		x = tempX * cosAngle - tempY * sinAngle + point.x;
		y = tempY * cosAngle + tempX * sinAngle + point.y;
	}
}


//--------------------------
// 3D Vector
//--------------------------
inline
Vector3::Vector3( const float *pf )
{
#ifdef _DEBUG
	if(!pf)
		return;
#endif

	x = pf[0];
	y = pf[1];
	z = pf[2];
}

inline
Vector3::Vector3( const Vector3& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}


inline
Vector3::Vector3( float fx, float fy, float fz )
{
	x = fx;
	y = fy;
	z = fz;
}


// casting
inline
Vector3::operator float* ()
{
	return (float *) &x;
}

inline
Vector3::operator const float* () const
{
	return (const float *) &x;
}


// assignment operators
inline Vector3&
Vector3::operator += ( const Vector3& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline Vector3&
Vector3::operator -= ( const Vector3& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline Vector3&
Vector3::operator *= ( float f )
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

inline Vector3&
Vector3::operator /= ( float f )
{
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	return *this;
}


// unary operators
inline Vector3
Vector3::operator + () const
{
	return *this;
}

inline Vector3
Vector3::operator - () const
{
	return Vector3(-x, -y, -z);
}


// binary operators
inline Vector3
Vector3::operator + ( const Vector3& v ) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

inline Vector3
Vector3::operator - ( const Vector3& v ) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

inline Vector3
Vector3::operator * ( float f ) const
{
	return Vector3(x * f, y * f, z * f);
}

inline Vector3
Vector3::operator / ( float f ) const
{
	float fInv = 1.0f / f;
	return Vector3(x * fInv, y * fInv, z * fInv);
}


inline Vector3
operator * ( float f, const struct Vector3& v )
{
	return Vector3(f * v.x, f * v.y, f * v.z);
}


inline bool
Vector3::operator == ( const Vector3& v ) const
{
	return x == v.x && y == v.y && z == v.z;
}

inline bool
Vector3::operator != ( const Vector3& v ) const
{
	return x != v.x || y != v.y || z != v.z;
}

//--------------------------
// 4D Vector
//--------------------------
inline
Vector4::Vector4( const float *pf )
{
#ifdef _DEBUG
	if(!pf)
		return;
#endif

	x = pf[0];
	y = pf[1];
	z = pf[2];
	w = pf[3];
}


inline
Vector4::Vector4( const Vector3& v, float f )
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = f;
}

inline
Vector4::Vector4( float fx, float fy, float fz, float fw )
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}


// casting
inline
Vector4::operator float* ()
{
	return (float *) &x;
}

inline
Vector4::operator const float* () const
{
	return (const float *) &x;
}


// assignment operators
inline Vector4&
Vector4::operator += ( const Vector4& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

inline Vector4&
Vector4::operator -= ( const Vector4& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

inline Vector4&
Vector4::operator *= ( float f )
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

inline Vector4&
Vector4::operator /= ( float f )
{
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	w *= fInv;
	return *this;
}


// unary operators
inline Vector4
Vector4::operator + () const
{
	return *this;
}

inline Vector4
Vector4::operator - () const
{
	return Vector4(-x, -y, -z, -w);
}


// binary operators
inline Vector4
Vector4::operator + ( const Vector4& v ) const
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

inline Vector4
Vector4::operator - ( const Vector4& v ) const
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

inline Vector4
Vector4::operator * ( float f ) const
{
	return Vector4(x * f, y * f, z * f, w * f);
}

inline Vector4
Vector4::operator / ( float f ) const
{
	float fInv = 1.0f / f;
	return Vector4(x * fInv, y * fInv, z * fInv, w * fInv);
}

inline Vector4
operator * ( float f, const Vector4& v )
{
	return Vector4(f * v.x, f * v.y, f * v.z, f * v.w);
}


inline bool
Vector4::operator == ( const Vector4& v ) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

inline bool
Vector4::operator != ( const Vector4& v ) const
{
	return x != v.x || y != v.y || z != v.z || w != v.w;
}



//--------------------------
// Matrix4x4
//--------------------------
inline
Matrix4x4::Matrix4x4( const float* pf )
{
#ifdef _DEBUG
	if(!pf)
		return;
#endif

	memcpy(&_11, pf, sizeof(Matrix4x4));
}

inline
Matrix4x4::Matrix4x4( const Matrix4x4& mat )
{
	memcpy(&_11, &mat, sizeof(Matrix4x4));
}


inline
Matrix4x4::Matrix4x4( float f11, float f12, float f13, float f14,
						 float f21, float f22, float f23, float f24,
						 float f31, float f32, float f33, float f34,
						 float f41, float f42, float f43, float f44 )
{
	_11 = f11; _12 = f12; _13 = f13; _14 = f14;
	_21 = f21; _22 = f22; _23 = f23; _24 = f24;
	_31 = f31; _32 = f32; _33 = f33; _34 = f34;
	_41 = f41; _42 = f42; _43 = f43; _44 = f44;
}



// access grants
inline float&
Matrix4x4::operator () ( Uint iRow, Uint iCol )
{
	return m[iRow][iCol];
}

inline float
Matrix4x4::operator () ( Uint iRow, Uint iCol ) const
{
	return m[iRow][iCol];
}


// casting operators
inline
Matrix4x4::operator float* ()
{
	return (float *) &_11;
}

inline
Matrix4x4::operator const float* () const
{
	return (const float *) &_11;
}


// assignment operators
inline Matrix4x4&
Matrix4x4::operator *= ( const Matrix4x4& mat )
{
	MatrixMultiply(this, this, &mat);
	return *this;
}

inline Matrix4x4&
Matrix4x4::operator += ( const Matrix4x4& mat )
{
	_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
	_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
	_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
	_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
	return *this;
}

inline Matrix4x4&
Matrix4x4::operator -= ( const Matrix4x4& mat )
{
	_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
	_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
	_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
	_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
	return *this;
}

inline Matrix4x4&
Matrix4x4::operator *= ( float f )
{
	_11 *= f; _12 *= f; _13 *= f; _14 *= f;
	_21 *= f; _22 *= f; _23 *= f; _24 *= f;
	_31 *= f; _32 *= f; _33 *= f; _34 *= f;
	_41 *= f; _42 *= f; _43 *= f; _44 *= f;
	return *this;
}

inline Matrix4x4&
Matrix4x4::operator /= ( float f )
{
	float fInv = 1.0f / f;
	_11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
	_21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
	_31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
	_41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
	return *this;
}


// unary operators
inline Matrix4x4
Matrix4x4::operator + () const
{
	return *this;
}

inline Matrix4x4
Matrix4x4::operator - () const
{
	return Matrix4x4(-_11, -_12, -_13, -_14,
		-_21, -_22, -_23, -_24,
		-_31, -_32, -_33, -_34,
		-_41, -_42, -_43, -_44);
}


// binary operators
inline Matrix4x4
Matrix4x4::operator * ( const Matrix4x4& mat ) const
{
	Matrix4x4 matT;
	MatrixMultiply(&matT, this, &mat);
	return matT;
}

inline Matrix4x4
Matrix4x4::operator + ( const Matrix4x4& mat ) const
{
	return Matrix4x4(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
		_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
		_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
		_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
}

inline Matrix4x4
Matrix4x4::operator - ( const Matrix4x4& mat ) const
{
	return Matrix4x4(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
		_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
		_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
		_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
}

inline Matrix4x4
Matrix4x4::operator * ( float f ) const
{
	return Matrix4x4(_11 * f, _12 * f, _13 * f, _14 * f,
		_21 * f, _22 * f, _23 * f, _24 * f,
		_31 * f, _32 * f, _33 * f, _34 * f,
		_41 * f, _42 * f, _43 * f, _44 * f);
}

inline Matrix4x4
Matrix4x4::operator / ( float f ) const
{
	float fInv = 1.0f / f;
	return Matrix4x4(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
		_21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
		_31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
		_41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);
}


inline Matrix4x4
operator * ( float f, const Matrix4x4& mat )
{
	return Matrix4x4(f * mat._11, f * mat._12, f * mat._13, f * mat._14,
		f * mat._21, f * mat._22, f * mat._23, f * mat._24,
		f * mat._31, f * mat._32, f * mat._33, f * mat._34,
		f * mat._41, f * mat._42, f * mat._43, f * mat._44);
}


inline bool
Matrix4x4::operator == ( const Matrix4x4& mat ) const
{
	return 0 == memcmp(this, &mat, sizeof(Matrix4x4));
}

inline bool
Matrix4x4::operator != ( const Matrix4x4& mat ) const
{
	return 0 != memcmp(this, &mat, sizeof(Matrix4x4));
}



typedef unsigned long ULONG_PTR;
typedef ULONG_PTR SIZE_T;


#ifndef SIZE_MAX
#define SIZE_MAX ((SIZE_T)-1)
#endif



//--------------------------
// Quaternion
//--------------------------

inline
Quaternion::Quaternion( const float* pf )
{
#ifdef _DEBUG
	if(!pf)
		return;
#endif

	x = pf[0];
	y = pf[1];
	z = pf[2];
	w = pf[3];
}


inline
Quaternion::Quaternion( float fx, float fy, float fz, float fw )
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}


// casting
inline
Quaternion::operator float* ()
{
	return (float *) &x;
}

inline
Quaternion::operator const float* () const
{
	return (const float *) &x;
}


// assignment operators
inline Quaternion&
Quaternion::operator += ( const Quaternion& q )
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

inline Quaternion&
Quaternion::operator -= ( const Quaternion& q )
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

inline Quaternion&
Quaternion::operator *= ( const Quaternion& q )
{
	QuaternionMultiply(this, this, &q);
	return *this;
}

inline Quaternion&
Quaternion::operator *= ( float f )
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

inline Quaternion&
Quaternion::operator /= ( float f )
{
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	w *= fInv;
	return *this;
}


// unary operators
inline Quaternion
Quaternion::operator + () const
{
	return *this;
}

inline Quaternion
Quaternion::operator - () const
{
	return Quaternion(-x, -y, -z, -w);
}


// binary operators
inline Quaternion
Quaternion::operator + ( const Quaternion& q ) const
{
	return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

inline Quaternion
Quaternion::operator - ( const Quaternion& q ) const
{
	return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

inline Quaternion
Quaternion::operator * ( const Quaternion& q ) const
{
	Quaternion qT;
	QuaternionMultiply(&qT, this, &q);
	return qT;
}

inline Quaternion
Quaternion::operator * ( float f ) const
{
	return Quaternion(x * f, y * f, z * f, w * f);
}

inline Quaternion
Quaternion::operator / ( float f ) const
{
	float fInv = 1.0f / f;
	return Quaternion(x * fInv, y * fInv, z * fInv, w * fInv);
}


inline Quaternion
operator * (float f, const Quaternion& q )
{
	return Quaternion(f * q.x, f * q.y, f * q.z, f * q.w);
}


inline bool
Quaternion::operator == ( const Quaternion& q ) const
{
	return x == q.x && y == q.y && z == q.z && w == q.w;
}

inline bool
Quaternion::operator != ( const Quaternion& q ) const
{
	return x != q.x || y != q.y || z != q.z || w != q.w;
}



//--------------------------
// Plane
//--------------------------

inline
Plane::Plane( const float* pf )
{
#ifdef _DEBUG
	if(!pf)
		return;
#endif

	a = pf[0];
	b = pf[1];
	c = pf[2];
	d = pf[3];
}

inline
Plane::Plane( float fa, float fb, float fc, float fd )
{
	a = fa;
	b = fb;
	c = fc;
	d = fd;
}


// casting
inline
Plane::operator float* ()
{
	return (float *) &a;
}

inline
Plane::operator const float* () const
{
	return (const float *) &a;
}


// assignment operators
inline Plane&
Plane::operator *= ( float f )
{
	a *= f;
	b *= f;
	c *= f;
	d *= f;
	return *this;
}

inline Plane&
Plane::operator /= ( float f )
{
	float fInv = 1.0f / f;
	a *= fInv;
	b *= fInv;
	c *= fInv;
	d *= fInv;
	return *this;
}


// unary operators
inline Plane
Plane::operator + () const
{
	return *this;
}

inline Plane
Plane::operator - () const
{
	return Plane(-a, -b, -c, -d);
}


// binary operators
inline Plane
Plane::operator * ( float f ) const
{
	return Plane(a * f, b * f, c * f, d * f);
}

inline Plane
Plane::operator / ( float f ) const
{
	float fInv = 1.0f / f;
	return Plane(a * fInv, b * fInv, c * fInv, d * fInv);
}

inline Plane
operator * (float f, const Plane& p )
{
	return Plane(f * p.a, f * p.b, f * p.c, f * p.d);
}

inline bool
Plane::operator == ( const Plane& p ) const
{
	return a == p.a && b == p.b && c == p.c && d == p.d;
}

inline bool
Plane::operator != ( const Plane& p ) const
{
	return a != p.a || b != p.b || c != p.c || d != p.d;
}




//--------------------------
// Color
//--------------------------

inline
Color::Color( Uint32 dw )
{
	const float f = 1.0f / 255.0f;
	r = f * (float) (unsigned char) (dw >> 16);
	g = f * (float) (unsigned char) (dw >>  8);
	b = f * (float) (unsigned char) (dw >>  0);
	a = f * (float) (unsigned char) (dw >> 24);
}

inline
Color::Color( const float* pf )
{
#ifdef _DEBUG
	if(!pf)
		return;
#endif

	r = pf[0];
	g = pf[1];
	b = pf[2];
	a = pf[3];
}


inline
Color::Color( float fr, float fg, float fb, float fa )
{
	r = fr;
	g = fg;
	b = fb;
	a = fa;
}


// casting
inline
Color::operator Uint32 () const
{
	Uint32 dwR = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : (Uint32) (r * 255.0f + 0.5f);
	Uint32 dwG = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : (Uint32) (g * 255.0f + 0.5f);
	Uint32 dwB = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : (Uint32) (b * 255.0f + 0.5f);
	Uint32 dwA = a >= 1.0f ? 0xff : a <= 0.0f ? 0x00 : (Uint32) (a * 255.0f + 0.5f);

	return (dwA << 24) | (dwR << 16) | (dwG << 8) | dwB;
}


inline
Color::operator float * ()
{
	return (float *) &r;
}

inline
Color::operator const float * () const
{
	return (const float *) &r;
}


// assignment operators
inline Color&
Color::operator += ( const Color& c )
{
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;
	return *this;
}

inline Color&
Color::operator -= ( const Color& c )
{
	r -= c.r;
	g -= c.g;
	b -= c.b;
	a -= c.a;
	return *this;
}

inline Color&
Color::operator *= ( float f )
{
	r *= f;
	g *= f;
	b *= f;
	a *= f;
	return *this;
}

inline Color&
Color::operator /= ( float f )
{
	float fInv = 1.0f / f;
	r *= fInv;
	g *= fInv;
	b *= fInv;
	a *= fInv;
	return *this;
}


// unary operators
inline Color
Color::operator + () const
{
	return *this;
}

inline Color
Color::operator - () const
{
	return Color(-r, -g, -b, -a);
}


// binary operators
inline Color
Color::operator + ( const Color& c ) const
{
	return Color(r + c.r, g + c.g, b + c.b, a + c.a);
}

inline Color
Color::operator - ( const Color& c ) const
{
	return Color(r - c.r, g - c.g, b - c.b, a - c.a);
}

inline Color
Color::operator * ( float f ) const
{
	return Color(r * f, g * f, b * f, a * f);
}

inline Color
Color::operator / ( float f ) const
{
	float fInv = 1.0f / f;
	return Color(r * fInv, g * fInv, b * fInv, a * fInv);
}


inline Color
operator * (float f, const Color& c )
{
	return Color(f * c.r, f * c.g, f * c.b, f * c.a);
}


inline bool
Color::operator == ( const Color& c ) const
{
	return r == c.r && g == c.g && b == c.b && a == c.a;
}

inline bool
Color::operator != ( const Color& c ) const
{
	return r != c.r || g != c.g || b != c.b || a != c.a;
}


#endif //__cplusplus



//===========================================================================
//
// Inline functions
//
//===========================================================================


//------------------------------------------------------------------------------
//Scalar
//------------------------------------------------------------------------------

inline bool IsNaN(float fVal)
{
	return fVal != fVal;
}

inline float Abs(float val)
{
	return fabsf(val);
}

inline float InvSqrt(float v)
{
	return 1.0f / sqrtf(v);
}


inline float Clamp(float fVal,float fMin,float fMax)
{
	fVal = fVal > fMin ? fVal : fMin;
	fVal = fVal < fMax ? fVal : fMax;
	return fVal;
}

//--------------------------
// 2D Vector
//--------------------------

inline float Vec2Length
( const Vector2 *pV )
{
#ifdef _DEBUG
	if(!pV)
		return 0.0f;
#endif

#ifdef __cplusplus
	return sqrtf(pV->x * pV->x + pV->y * pV->y);
#else
	return (float) sqrt(pV->x * pV->x + pV->y * pV->y);
#endif
}

inline float Vec2LengthSq
( const Vector2 *pV )
{
#ifdef _DEBUG
	if(!pV)
		return 0.0f;
#endif

	return pV->x * pV->x + pV->y * pV->y;
}

inline float Vec2Dot
( const Vector2 *pV1, const Vector2 *pV2 )
{
#ifdef _DEBUG
	if(!pV1 || !pV2)
		return 0.0f;
#endif

	return pV1->x * pV2->x + pV1->y * pV2->y;
}

inline float Vec2CCW
( const Vector2 *pV1, const Vector2 *pV2 )
{
#ifdef _DEBUG
	if(!pV1 || !pV2)
		return 0.0f;
#endif

	return pV1->x * pV2->y - pV1->y * pV2->x;
}

inline Vector2* Vec2Add
( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2 )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;
	return pOut;
}

inline Vector2* Vec2Subtract
( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2 )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	return pOut;
}

inline Vector2* Vec2Minimize
( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2 )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x < pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y < pV2->y ? pV1->y : pV2->y;
	return pOut;
}

inline Vector2* Vec2Maximize
( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2 )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x > pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y > pV2->y ? pV1->y : pV2->y;
	return pOut;
}

inline Vector2* Vec2Scale
( Vector2 *pOut, const Vector2 *pV, float s )
{
#ifdef _DEBUG
	if(!pOut || !pV)
		return NULL;
#endif

	pOut->x = pV->x * s;
	pOut->y = pV->y * s;
	return pOut;
}

inline Vector2* Vec2Lerp
( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2,
 float s )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x + s * (pV2->x - pV1->x);
	pOut->y = pV1->y + s * (pV2->y - pV1->y);
	return pOut;
}


//--------------------------
// 3D Vector
//--------------------------

inline float Vec3Length
( const Vector3 *pV )
{
#ifdef _DEBUG
	if(!pV)
		return 0.0f;
#endif

#ifdef __cplusplus
	return sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
#else
	return (float) sqrt(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
#endif
}

inline float Vec3LengthSq
( const Vector3 *pV )
{
#ifdef _DEBUG
	if(!pV)
		return 0.0f;
#endif

	return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z;
}

inline float Vec3Dot
( const Vector3 *pV1, const Vector3 *pV2 )
{
#ifdef _DEBUG
	if(!pV1 || !pV2)
		return 0.0f;
#endif

	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

inline Vector3* Vec3Cross
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 )
{
	Vector3 v;

#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	v.x = pV1->y * pV2->z - pV1->z * pV2->y;
	v.y = pV1->z * pV2->x - pV1->x * pV2->z;
	v.z = pV1->x * pV2->y - pV1->y * pV2->x;

	*pOut = v;
	return pOut;
}

inline Vector3* Vec3Add
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;
	pOut->z = pV1->z + pV2->z;
	return pOut;
}

inline Vector3* Vec3Subtract
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
	return pOut;
}

inline Vector3* Vec3Minimize
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x < pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y < pV2->y ? pV1->y : pV2->y;
	pOut->z = pV1->z < pV2->z ? pV1->z : pV2->z;
	return pOut;
}

inline Vector3* Vec3Maximize
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x > pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y > pV2->y ? pV1->y : pV2->y;
	pOut->z = pV1->z > pV2->z ? pV1->z : pV2->z;
	return pOut;
}

inline Vector3* Vec3Scale
( Vector3 *pOut, const Vector3 *pV, float s)
{
#ifdef _DEBUG
	if(!pOut || !pV)
		return NULL;
#endif

	pOut->x = pV->x * s;
	pOut->y = pV->y * s;
	pOut->z = pV->z * s;
	return pOut;
}

inline Vector3* Vec3Lerp
( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2,
 float s )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x + s * (pV2->x - pV1->x);
	pOut->y = pV1->y + s * (pV2->y - pV1->y);
	pOut->z = pV1->z + s * (pV2->z - pV1->z);
	return pOut;
}


//--------------------------
// 4D Vector
//--------------------------

inline float Vec4Length
( const Vector4 *pV )
{
#ifdef _DEBUG
	if(!pV)
		return 0.0f;
#endif

#ifdef __cplusplus
	return sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w);
#else
	return (float) sqrt(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w);
#endif
}

inline float Vec4LengthSq
( const Vector4 *pV )
{
#ifdef _DEBUG
	if(!pV)
		return 0.0f;
#endif

	return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w;
}

inline float Vec4Dot
( const Vector4 *pV1, const Vector4 *pV2 )
{
#ifdef _DEBUG
	if(!pV1 || !pV2)
		return 0.0f;
#endif

	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z + pV1->w * pV2->w;
}

inline Vector4* Vec4Add
( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2)
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;
	pOut->z = pV1->z + pV2->z;
	pOut->w = pV1->w + pV2->w;
	return pOut;
}

inline Vector4* Vec4Subtract
( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2)
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
	pOut->w = pV1->w - pV2->w;
	return pOut;
}

inline Vector4* Vec4Minimize
( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2)
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x < pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y < pV2->y ? pV1->y : pV2->y;
	pOut->z = pV1->z < pV2->z ? pV1->z : pV2->z;
	pOut->w = pV1->w < pV2->w ? pV1->w : pV2->w;
	return pOut;
}

inline Vector4* Vec4Maximize
( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2)
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x > pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y > pV2->y ? pV1->y : pV2->y;
	pOut->z = pV1->z > pV2->z ? pV1->z : pV2->z;
	pOut->w = pV1->w > pV2->w ? pV1->w : pV2->w;
	return pOut;
}

inline Vector4* Vec4Scale
( Vector4 *pOut, const Vector4 *pV, float s)
{
#ifdef _DEBUG
	if(!pOut || !pV)
		return NULL;
#endif

	pOut->x = pV->x * s;
	pOut->y = pV->y * s;
	pOut->z = pV->z * s;
	pOut->w = pV->w * s;
	return pOut;
}

inline Vector4* Vec4Lerp
( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2,
 float s )
{
#ifdef _DEBUG
	if(!pOut || !pV1 || !pV2)
		return NULL;
#endif

	pOut->x = pV1->x + s * (pV2->x - pV1->x);
	pOut->y = pV1->y + s * (pV2->y - pV1->y);
	pOut->z = pV1->z + s * (pV2->z - pV1->z);
	pOut->w = pV1->w + s * (pV2->w - pV1->w);
	return pOut;
}


//--------------------------
// 4D Matrix4x4
//--------------------------

inline Matrix4x4* MatrixIdentity
( Matrix4x4 *pOut )
{
#ifdef _DEBUG
	if(!pOut)
		return NULL;
#endif

	pOut->m[0][1] = pOut->m[0][2] = pOut->m[0][3] =
		pOut->m[1][0] = pOut->m[1][2] = pOut->m[1][3] =
		pOut->m[2][0] = pOut->m[2][1] = pOut->m[2][3] =
		pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;

	pOut->m[0][0] = pOut->m[1][1] = pOut->m[2][2] = pOut->m[3][3] = 1.0f;
	return pOut;
}


inline bool MatrixIsIdentity
( const Matrix4x4 *pM )
{
#ifdef _DEBUG
	if(!pM)
		return FALSE;
#endif

	return pM->m[0][0] == 1.0f && pM->m[0][1] == 0.0f && pM->m[0][2] == 0.0f && pM->m[0][3] == 0.0f &&
		pM->m[1][0] == 0.0f && pM->m[1][1] == 1.0f && pM->m[1][2] == 0.0f && pM->m[1][3] == 0.0f &&
		pM->m[2][0] == 0.0f && pM->m[2][1] == 0.0f && pM->m[2][2] == 1.0f && pM->m[2][3] == 0.0f &&
		pM->m[3][0] == 0.0f && pM->m[3][1] == 0.0f && pM->m[3][2] == 0.0f && pM->m[3][3] == 1.0f;
}


inline void  MatrixFromEulerAngleXYZ(Matrix4x4* pMat,const EulerAngleXYZ* pEuler)
{
	//Matrix4x4 matRotX;
	//Matrix4x4 matRotY;
	//Matrix4x4 matRotZ;

	//MatrixRotationX(&matRotX,pEuler->x);
	//MatrixRotationY(&matRotY,pEuler->y);
	//MatrixRotationZ(&matRotZ,pEuler->z);

	//MatrixMultiply(pMat,&matRotX,&matRotY);
	//MatrixMultiply(pMat,pMat,&matRotZ);

	// rot  = cy*cz           cy*sz           -sy
	//        cz*sx*sy-cx*sz  cx*cz+sx*sy*sz  cy*sx
	//        cx*cz*sy+sx*sz  -cz*sx+cx*sy*sz  cx*cy
	const float cx = cosf(pEuler->x);
	const float sx = sinf(pEuler->x);
	const float cy = cosf(pEuler->y);
	const float sy = sinf(pEuler->y);
	const float cz = cosf(pEuler->z);
	const float sz = sinf(pEuler->z);
	pMat->_11 = cy*cz;
	pMat->_12 = cy*sz;
	pMat->_13 = -sy;
	pMat->_14 = 0.0f;

	pMat->_21 = cz*sx*sy-cx*sz;
	pMat->_22 = cx*cz+sx*sy*sz;
	pMat->_23 = cy*sx;
	pMat->_24 = 0.0f;

	pMat->_31 = cx*cz*sy+sx*sz;
	pMat->_32 = -cz*sx+cx*sy*sz;
	pMat->_33 = cx*cy;
	pMat->_34 = 0.0f;

	pMat->_41 = 0.0f;
	pMat->_42 = 0.0f;
	pMat->_43 = 0.0f;
	pMat->_44 = 1.0f;
}

inline void  MatrixFromQuaternion(Matrix4x4* pMat,const Quaternion* pRot)
{
	EulerAngleXYZ eRot;
	EulerAngleFromQuaternion(&eRot,pRot);
	MatrixFromEulerAngleXYZ(pMat,&eRot);
}

inline void  MatrixAddTranslation(Matrix4x4* pMat,const Vector3* pDeltaTsl)
{
	Vector3* pTsl = (Vector3*)&pMat->_41;
	*pTsl += *pDeltaTsl;
}


//--------------------------
// Quaternion
//--------------------------
inline void  QuaternionFromEulerAngleXYZ(Quaternion* pQuat,const EulerAngleXYZ* pEuler)
{

	float fHalfX = 0.5f*pEuler->x;
	float fHalfY = 0.5f*pEuler->y;
	float fHalfZ = 0.5f*pEuler->z;
	Quaternion qRotX(sinf(fHalfX),0.0f,0.0f,cosf(fHalfX));
	Quaternion qRotY(0.0f,sinf(fHalfY),0.0f,cosf(fHalfY));
	Quaternion qRotZ(0.0f,0.0f,sinf(fHalfZ),cosf(fHalfZ));
	*pQuat = qRotX * qRotY;
	*pQuat = *pQuat * qRotZ;

}


inline float QuaternionLength
( const Quaternion *pQ )
{
#ifdef _DEBUG
	if(!pQ)
		return 0.0f;
#endif

#ifdef __cplusplus
	return sqrtf(pQ->x * pQ->x + pQ->y * pQ->y + pQ->z * pQ->z + pQ->w * pQ->w);
#else
	return (float) sqrt(pQ->x * pQ->x + pQ->y * pQ->y + pQ->z * pQ->z + pQ->w * pQ->w);
#endif
}

inline float QuaternionLengthSq
( const Quaternion *pQ )
{
#ifdef _DEBUG
	if(!pQ)
		return 0.0f;
#endif

	return pQ->x * pQ->x + pQ->y * pQ->y + pQ->z * pQ->z + pQ->w * pQ->w;
}

inline float QuaternionDot
( const Quaternion *pQ1, const Quaternion *pQ2 )
{
#ifdef _DEBUG
	if(!pQ1 || !pQ2)
		return 0.0f;
#endif

	return pQ1->x * pQ2->x + pQ1->y * pQ2->y + pQ1->z * pQ2->z + pQ1->w * pQ2->w;
}


inline Quaternion* QuaternionIdentity
( Quaternion *pOut )
{
#ifdef _DEBUG
	if(!pOut)
		return NULL;
#endif

	pOut->x = pOut->y = pOut->z = 0.0f;
	pOut->w = 1.0f;
	return pOut;
}

inline bool QuaternionIsIdentity
( const Quaternion *pQ )
{
#ifdef _DEBUG
	if(!pQ)
		return FALSE;
#endif

	return pQ->x == 0.0f && pQ->y == 0.0f && pQ->z == 0.0f && pQ->w == 1.0f;
}


inline Quaternion* QuaternionConjugate
( Quaternion *pOut, const Quaternion *pQ )
{
#ifdef _DEBUG
	if(!pOut || !pQ)
		return NULL;
#endif

	pOut->x = -pQ->x;
	pOut->y = -pQ->y;
	pOut->z = -pQ->z;
	pOut->w =  pQ->w;
	return pOut;
}

//------------------------------------------------------------------------------
//Euler Angle
//------------------------------------------------------------------------------

inline EulerAngleXYZ::EulerAngleXYZ()
{

}

inline EulerAngleXYZ::EulerAngleXYZ(float fX,float fY,float fZ)
:x(fX)
,y(fY)
,z(fZ)
{

}


inline EulerAngleXYZ EulerAngleXYZ::operator +(const EulerAngleXYZ& rhs)
{
	return EulerAngleXYZ(x + rhs.x,y + rhs.y, z + rhs.z);
}

inline EulerAngleXYZ& EulerAngleXYZ::operator +=(const EulerAngleXYZ& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

inline EulerAngleXYZ& EulerAngleXYZ::operator *=(float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}


inline void  EulerAngleXYZ::Normalize()
{
	x = fmodf(x,TWO_PI);
	y = fmodf(y,TWO_PI);
	z = fmodf(z,TWO_PI);
}



inline void  EulerAngleXYZIdentity(EulerAngleXYZ* pEuler)
{
	memset(pEuler,0,sizeof(EulerAngleXYZ));
}

inline bool EulerAngleXYZFromMatrix(EulerAngleXYZ* pEuler,const Matrix4x4* pMat)
{
	// rot  = cy*cz           cy*sz           -sy
	//        cz*sx*sy-cx*sz  cx*cz+sx*sy*sz  cy*sx
	//        cx*cz*sy+sx*sz  -cz*sx+cx*sy*sz  cx*cy

	float m02 = Clamp(pMat->m[0][2],-1.0f,1.0f);
	pEuler->y = -asinf(m02); //asinf(x) has error when x is outside of range[-1,1]

	if ( pEuler->y < HALF_PI )
	{
		if ( pEuler->y > -HALF_PI )
		{
			pEuler->x = atan2f(pMat->m[1][2],pMat->m[2][2]);
			pEuler->z = atan2f(pMat->m[0][1],pMat->m[0][0]);
			return true;
		}
		else
		{
			// WARNING.  Not a unique solution.
			float fRmY = atan2f(pMat->m[1][0],pMat->m[1][1]);
			pEuler->z = (0.0f);  // any angle works
			pEuler->x = pEuler->z - fRmY;
			return false;
		}
	}
	else
	{
		// WARNING.  Not a unique solution.
		float fRpY = atan2f(pMat->m[1][0],pMat->m[1][1]);
		pEuler->z = (0.0f);  // any angle works
		pEuler->x = fRpY - pEuler->z;
		return false;
	}
}

inline void  EulerAngleFromQuaternion(EulerAngleXYZ* pEuler,const Quaternion* pQua)
{

	Matrix4x4 mat;
	MatrixRotationQuaternion(&mat,pQua);
	EulerAngleXYZFromMatrix(pEuler,&mat);

}

//axis is non zero
//x-roll = 0, y-pitch, z-yaw
inline void  EulerAngleFromXToAxis(EulerAngleXYZ* pEuler,const Vector3* pAxis)
{
	float r = sqrtf(pAxis->x*pAxis->x+pAxis->y*pAxis->y);
	pEuler->x = 0.0f;
	pEuler->z = atan2f(pAxis->y,pAxis->x);	//yaw
	pEuler->y = - atan2f(pAxis->z,r);		//pitch

}



//--------------------------
// Plane
//--------------------------

inline float PlaneDot
( const Plane *pP, const Vector4 *pV)
{
#ifdef _DEBUG
	if(!pP || !pV)
		return 0.0f;
#endif

	return pP->a * pV->x + pP->b * pV->y + pP->c * pV->z + pP->d * pV->w;
}

inline float PlaneDotCoord
( const Plane *pP, const Vector3 *pV)
{
#ifdef _DEBUG
	if(!pP || !pV)
		return 0.0f;
#endif

	return pP->a * pV->x + pP->b * pV->y + pP->c * pV->z + pP->d;
}

inline float PlaneDotNormal
( const Plane *pP, const Vector3 *pV)
{
#ifdef _DEBUG
	if(!pP || !pV)
		return 0.0f;
#endif

	return pP->a * pV->x + pP->b * pV->y + pP->c * pV->z;
}

inline Plane* PlaneScale
(Plane *pOut, const Plane *pP, float s)
{
#ifdef _DEBUG
	if(!pOut || !pP)
		return NULL;
#endif

	pOut->a = pP->a * s;
	pOut->b = pP->b * s;
	pOut->c = pP->c * s;
	pOut->d = pP->d * s;
	return pOut;
}


//--------------------------
// Color
//--------------------------

inline Color* ColorNegative
(Color *pOut, const Color *pC)
{
#ifdef _DEBUG
	if(!pOut || !pC)
		return NULL;
#endif

	pOut->r = 1.0f - pC->r;
	pOut->g = 1.0f - pC->g;
	pOut->b = 1.0f - pC->b;
	pOut->a = pC->a;
	return pOut;
}

inline Color* ColorAdd
(Color *pOut, const Color *pC1, const Color *pC2)
{
#ifdef _DEBUG
	if(!pOut || !pC1 || !pC2)
		return NULL;
#endif

	pOut->r = pC1->r + pC2->r;
	pOut->g = pC1->g + pC2->g;
	pOut->b = pC1->b + pC2->b;
	pOut->a = pC1->a + pC2->a;
	return pOut;
}

inline Color* ColorSubtract
(Color *pOut, const Color *pC1, const Color *pC2)
{
#ifdef _DEBUG
	if(!pOut || !pC1 || !pC2)
		return NULL;
#endif

	pOut->r = pC1->r - pC2->r;
	pOut->g = pC1->g - pC2->g;
	pOut->b = pC1->b - pC2->b;
	pOut->a = pC1->a - pC2->a;
	return pOut;
}

inline Color* ColorScale
(Color *pOut, const Color *pC, float s)
{
#ifdef _DEBUG
	if(!pOut || !pC)
		return NULL;
#endif

	pOut->r = pC->r * s;
	pOut->g = pC->g * s;
	pOut->b = pC->b * s;
	pOut->a = pC->a * s;
	return pOut;
}

inline Color* ColorModulate
(Color *pOut, const Color *pC1, const Color *pC2)
{
#ifdef _DEBUG
	if(!pOut || !pC1 || !pC2)
		return NULL;
#endif

	pOut->r = pC1->r * pC2->r;
	pOut->g = pC1->g * pC2->g;
	pOut->b = pC1->b * pC2->b;
	pOut->a = pC1->a * pC2->a;
	return pOut;
}

inline Color* ColorLerp
(Color *pOut, const Color *pC1, const Color *pC2, float s)
{
#ifdef _DEBUG
	if(!pOut || !pC1 || !pC2)
		return NULL;
#endif

	pOut->r = pC1->r + s * (pC2->r - pC1->r);
	pOut->g = pC1->g + s * (pC2->g - pC1->g);
	pOut->b = pC1->b + s * (pC2->b - pC1->b);
	pOut->a = pC1->a + s * (pC2->a - pC1->a);
	return pOut;
}


#endif //__MathPortable_INL__