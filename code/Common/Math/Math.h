/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __Math_H__
#define __Math_H__

#include "../BaseType.h"
#include "../Macro.h"
#include "Angle.h"

namespace ma
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/
	/** Wrapper class which indicates a given angle value is in Radians.
    @remarks
        Radian values are interchangeable with Degree values, and conversions
        will be done automatically between them.
    */
	class Vector2;
	class Vector3;
	class Vector4;
	class Ray;
	class Matrix3;
	class Matrix4;
	class Plane;
	class PlaneBoundedVolume;
	class Quaternion;
	class Radian;
	class Sphere;
	class AABB;

	        /** Structure used to define a box in a 3-D integer space.
         	Note that the left, top, and front edges are included but the right, 
         	bottom and back ones are not.
         */
        struct Box
        {
            size_t left, top, right, bottom, front, back;
			/// Parameterless constructor for setting the members manually
            Box()
				: left(0), top(0), right(1), bottom(1), front(0), back(1)
            {
            }
            /** Define a box from left, top, right and bottom coordinates
            	This box will have depth one (front=0 and back=1).
            	@param	l	x value of left edge
            	@param	t	y value of top edge
            	@param	r	x value of right edge
            	@param	b	y value of bottom edge
            	@note Note that the left, top, and front edges are included 
 		           	but the right, bottom and back ones are not.
            */
            Box( size_t l, size_t t, size_t r, size_t b ):
                left(l),
                top(t),   
                right(r),
                bottom(b),
                front(0),
                back(1)
            {
          		assert(right >= left && bottom >= top && back >= front);
            }
            /** Define a box from left, top, front, right, bottom and back
            	coordinates.
            	@param	l	x value of left edge
            	@param	t	y value of top edge
            	@param  ff  z value of front edge
            	@param	r	x value of right edge
            	@param	b	y value of bottom edge
            	@param  bb  z value of back edge
            	@note Note that the left, top, and front edges are included 
 		           	but the right, bottom and back ones are not.
            */
            Box( size_t l, size_t t, size_t ff, size_t r, size_t b, size_t bb ):
                left(l),
                top(t),   
                right(r),
                bottom(b),
                front(ff),
                back(bb)
            {
          		assert(right >= left && bottom >= top && back >= front);
            }
            
            /// Return true if the other box is a part of this one
            bool contains(const Box &def) const
            {
            	return (def.left >= left && def.top >= top && def.front >= front &&
					def.right <= right && def.bottom <= bottom && def.back <= back);
            }
            
            /// Get the width of this box
            size_t getWidth() const { return right-left; }
            /// Get the height of this box
            size_t getHeight() const { return bottom-top; }
            /// Get the depth of this box
            size_t getDepth() const { return back-front; }
        };


    /** Class to provide access to common mathematical functions.
        @remarks
            Most of the maths functions are aliased versions of the C runtime
            library functions. They are aliased here to provide future
            optimisation opportunities, either from faster RTLs or custom
            math approximations.
        @note
            <br>This is based on MgcMath.h from
            <a href="http://www.geometrictools.com/">Wild Magic</a>.
    */
    class Math 
    {
   public:
       /** The angular units used by the API. This functionality is now deprecated in favor
	       of discreet angular unit types ( see Degree and Radian above ). The only place
		   this functionality is actually still used is when parsing files. Search for
		   usage of the Angle class for those instances
       */

    protected:

        /// Size of the trig tables as determined by constructor.
        static int mTrigTableSize;

        /// Radian -> index factor value ( mTrigTableSize / 2 * PI )
        static float mTrigTableFactor;
        static float* mSinTable;
        static float* mTanTable;

        /** Private function to build trig tables.
        */
        void buildTrigTables();

		static float SinTable (float fValue);
		static float TanTable (float fValue);
    public:
        /** Default constructor.
            @param
                trigTableSize Optional parameter to set the size of the
                tables used to implement Sin, Cos, Tan
        */
        Math(unsigned int trigTableSize = 4096);

        /** Default destructor.
        */
        ~Math();

		static inline int IAbs (int iValue) { return ( iValue >= 0 ? iValue : -iValue ); }
		static inline int ICeil (float fValue) { return int(ceil(fValue)); }
		static inline int IFloor (float fValue) { return int(floor(fValue)); }
        static int ISign (int iValue);

		static inline float Abs (float fValue) { return float(fabs(fValue)); }
		static inline Degree Abs (const Degree& dValue) { return Degree(fabs(dValue.valueDegrees())); }
		static inline Radian Abs (const Radian& rValue) { return Radian(fabs(rValue.valueRadians())); }
		static Radian ACos (float fValue);
		static Radian ASin (float fValue);
		static inline Radian ATan (float fValue) { return Radian(atan(fValue)); }
		static inline Radian ATan2 (float fY, float fX) { return Radian(atan2(fY,fX)); }
		static inline float Ceil (float fValue) { return float(ceil(fValue)); }
		static inline bool isNaN(float f)
		{
			// std::isnan() is C99, not supported by all compilers
			// However NaN always fails this next test, no other number does.
			return f != f;
		}

        /** Cosine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline float Cos (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? float(cos(fValue.valueRadians())) : SinTable(fValue.valueRadians() + HALF_PI);
		}
        /** Cosine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline float Cos (float fValue, bool useTables = false) {
			return (!useTables) ? float(cos(fValue)) : SinTable(fValue + HALF_PI);
		}

		static inline float Exp (float fValue) { return float(exp(fValue)); }

		static inline float Floor (float fValue) { return float(floor(fValue)); }

		static inline float Log (float fValue) { return float(log(fValue)); }

		/// Stored value of log(2) for frequent use
		static const float LOG2;

		static inline float Log2 (float fValue) { return float(log(fValue)/LOG2); }

		static inline float LogN (float base, float fValue) { return float(log(fValue)/log(base)); }

		static inline float Pow (float fBase, float fExponent) { return float(pow(fBase,fExponent)); }

		template <typename T>
		static T PowInt(T val, int nTimes)
		{
			unsigned int N;
			if (nTimes >= 0)
				N = (unsigned int)nTimes;
			else
				N = (unsigned int)(-nTimes);
			for (int Z = 1; ; val *= val)
			{
				if ((N & 1) != 0)
					Z *= val;
				if ((N >>= 1) == 0)
					return (nTimes < 0 ? 1/Z : Z); 
			}
		}

        static float Sign (float fValue);
		static inline Radian Sign ( const Radian& rValue )
		{
			return Radian(Sign(rValue.valueRadians()));
		}
		static inline Degree Sign ( const Degree& dValue )
		{
			return Degree(Sign(dValue.valueDegrees()));
		}

        /** Sine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline float Sin (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? float(sin(fValue.valueRadians())) : SinTable(fValue.valueRadians());
		}
        /** Sine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline float Sin (float fValue, bool useTables = false) {
			return (!useTables) ? float(sin(fValue)) : SinTable(fValue);
		}

		static inline float Sqr (float fValue) { return fValue*fValue; }

		static inline float Sqrt (float fValue) { return float(sqrt(fValue)); }

        static inline Radian Sqrt (const Radian& fValue) { return Radian(sqrt(fValue.valueRadians())); }

        static inline Degree Sqrt (const Degree& fValue) { return Degree(sqrt(fValue.valueDegrees())); }

        /** Inverse square root i.e. 1 / Sqrt(x), good for vector
            normalisation.
        */
		static float InvSqrt(float fValue);

        static float UnitRandom ();  // in [0,1]

        static float RangeRandom (float fLow, float fHigh);  // in [fLow,fHigh]

        static float SymmetricRandom ();  // in [-1,1]

        /** Tangent function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
		static inline float Tan (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? float(tan(fValue.valueRadians())) : TanTable(fValue.valueRadians());
		}
        /** Tangent function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
		static inline float Tan (float fValue, bool useTables = false) {
			return (!useTables) ? float(tan(fValue)) : TanTable(fValue);
		}


       /** Checks whether a given point is inside a triangle, in a
            2-dimensional (Cartesian) space.
            @remarks
                The vertices of the triangle must be given in either
                trigonometrical (anticlockwise) or inverse trigonometrical
                (clockwise) order.
            @param
                p The point.
            @param
                a The triangle's first vertex.
            @param
                b The triangle's second vertex.
            @param
                c The triangle's third vertex.
            @returns
                If the point resides in the triangle, <b>true</b> is
                returned.
            @par
                If the point is outside the triangle, <b>false</b> is
                returned.
        */
        static bool pointInTri2D(const Vector2& p, const Vector2& a, 
			const Vector2& b, const Vector2& c);

       /** Checks whether a given 3D point is inside a triangle.
       @remarks
            The vertices of the triangle must be given in either
            trigonometrical (anticlockwise) or inverse trigonometrical
            (clockwise) order, and the point must be guaranteed to be in the
			same plane as the triangle
        @param
            p The point.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
		@param 
			normal The triangle plane's normal (passed in rather than calculated
				on demand since the caller may already have it)
        @returns
            If the point resides in the triangle, <b>true</b> is
            returned.
        @par
            If the point is outside the triangle, <b>false</b> is
            returned.
        */
        static bool pointInTri3D(const Vector3& p, const Vector3& a, 
			const Vector3& b, const Vector3& c, const Vector3& normal);
        /** Ray / plane intersection, returns boolean result and distance. */
        static std::pair<bool, float> intersects(const Ray& ray, const Plane& plane);

        /** Ray / sphere intersection, returns boolean result and distance. */
        static std::pair<bool, float> intersects(const Ray& ray, const Sphere& sphere, 
            bool discardInside = true);
        
        /** Ray / box intersection, returns boolean result and distance. */
        static std::pair<bool, float> intersects(const Ray& ray, const AABB& box);

        /** Ray / box intersection, returns boolean result and two intersection distance.
        @param
            ray The ray.
        @param
            box The box.
        @param
            d1 A real pointer to retrieve the near intersection distance
                from the ray origin, maybe <b>null</b> which means don't care
                about the near intersection distance.
        @param
            d2 A real pointer to retrieve the far intersection distance
                from the ray origin, maybe <b>null</b> which means don't care
                about the far intersection distance.
        @returns
            If the ray is intersects the box, <b>true</b> is returned, and
            the near intersection distance is return by <i>d1</i>, the
            far intersection distance is return by <i>d2</i>. Guarantee
            <b>0</b> <= <i>d1</i> <= <i>d2</i>.
        @par
            If the ray isn't intersects the box, <b>false</b> is returned, and
            <i>d1</i> and <i>d2</i> is unmodified.
        */
        static bool intersects(const Ray& ray, const AABB& box,
            float* d1, float* d2);

        /** Ray / triangle intersection, returns boolean result and distance.
        @param
            ray The ray.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
		@param 
			normal The triangle plane's normal (passed in rather than calculated
				on demand since the caller may already have it), doesn't need
                normalised since we don't care.
        @param
            positiveSide Intersect with "positive side" of the triangle
        @param
            negativeSide Intersect with "negative side" of the triangle
        @returns
            If the ray is intersects the triangle, a pair of <b>true</b> and the
            distance between intersection point and ray origin returned.
        @par
            If the ray isn't intersects the triangle, a pair of <b>false</b> and
            <b>0</b> returned.
        */
        static std::pair<bool, float> intersects(const Ray& ray, const Vector3& a,
            const Vector3& b, const Vector3& c, const Vector3& normal,
            bool positiveSide = true, bool negativeSide = true);

        /** Ray / triangle intersection, returns boolean result and distance.
        @param
            ray The ray.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
        @param
            positiveSide Intersect with "positive side" of the triangle
        @param
            negativeSide Intersect with "negative side" of the triangle
        @returns
            If the ray is intersects the triangle, a pair of <b>true</b> and the
            distance between intersection point and ray origin returned.
        @par
            If the ray isn't intersects the triangle, a pair of <b>false</b> and
            <b>0</b> returned.
        */
        static std::pair<bool, float> intersects(const Ray& ray, const Vector3& a,
            const Vector3& b, const Vector3& c,
            bool positiveSide = true, bool negativeSide = true);

        /** Sphere / box intersection test. */
        static bool intersects(const Sphere& sphere, const AABB& box);

        /** Plane / box intersection test. */
        static bool intersects(const Plane& plane, const AABB& box);

        /** Ray / convex plane list intersection test. 
        @param ray The ray to test with
        @param plaeList List of planes which form a convex volume
        @param normalIsOutside Does the normal point outside the volume
        */
        static std::pair<bool, float> intersects(
            const Ray& ray, const vector<Plane>& planeList, 
            bool normalIsOutside);
        /** Ray / convex plane list intersection test. 
        @param ray The ray to test with
        @param plaeList List of planes which form a convex volume
        @param normalIsOutside Does the normal point outside the volume
        */
        static std::pair<bool, float> intersects(
            const Ray& ray, const list<Plane>& planeList, 
            bool normalIsOutside);

        /** Sphere / plane intersection test. 
        @remarks NB just do a plane.getDistance(sphere.getCenter()) for more detail!
        */
        static bool intersects(const Sphere& sphere, const Plane& plane);

        /** Compare 2 reals, using tolerance for inaccuracies.
        */
        static bool RealEqual(float a, float b,
            float tolerance = std::numeric_limits<float>::epsilon());

        /** Calculates the tangent space vector for a given set of positions / texture coords. */
        static Vector3 calculateTangentSpaceVector(
            const Vector3& position1, const Vector3& position2, const Vector3& position3,
            float u1, float v1, float u2, float v2, float u3, float v3);

        /** Build a reflection matrix for the passed in plane. */
        static Matrix4 buildReflectionMatrix(const Plane& p);
        /** Calculate a face normal, including the w component which is the offset from the origin. */
        static Vector4 calculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
        /** Calculate a face normal, no w-information. */
        static Vector3 calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
        /** Calculate a face normal without normalize, including the w component which is the offset from the origin. */
        static Vector4 calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);
        /** Calculate a face normal without normalize, no w-information. */
        static Vector3 calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);

		/** Generates a value based on the Gaussian (normal) distribution function
			with the given offset and scale parameters.
		*/
		static float gaussianDistribution(float x, float offset = 0.0f, float scale = 1.0f);

		/** Clamp a value within an inclusive range. */
		template <typename T>
		static T Clamp(T val, T minval, T maxval)
		{
			assert (minval < maxval && "Invalid clamp range");
			return max(min(val, maxval), minval);
		}

		template <typename T>
		static T Lerp(const T& v0, const T& v1, float s) {return v0 + (v1-v0)*s;}

		template <typename T>
		static T Mad(const T& v0, const T& v1, float s) {return v0 * s + v1;}

		template <typename T>
		static T Min(const T& v0, const T& v1) { return v0 < v1 ? v0 : v1; }

		template <typename T>
		static T Max(const T& v0, const T& v1) { return v0 > v1 ? v0 : v1;}

		static UINT NextPowerOfTwo(UINT v);

		template<class T>
		inline static T InvalidID() { return T(-1); }

		template<class T>
		inline static bool IsValidID(T ind) { return ind != (T)(-1); }

		template<class T>
		inline static bool IsInvalidID(T ind) { return ind == (T)(-1);}


#ifdef WIN32
		inline static float round(float r)
		{
			return (r > 0.0f) ? floor(r + 0.5f) : ceil(r - 0.5f);
		}
#endif

		static Matrix4 makeViewMatrix(const Vector3& position, const Quaternion& orientation, 
			const Matrix4* reflectMatrix = 0);

		/** Get a bounding radius value from a bounding box. */
		static float boundingRadiusFromAABB(const AABB& aabb);


		static Matrix4 MakeLookAtMatrixLH(const Vector3& vEye, const Vector3& vAt, const Vector3& vUp);
		static Matrix4 MakeLookAtMatrixRH(const Vector3& vEye, const Vector3& vAt, const Vector3& vUp);

		// result = v0 + (v1-v0)*s
		//static Vector3 Lerp(const Vector3& v0, const Vector3& v1, Real s);
		// check if sphere contains box
		static bool Contains(const Sphere& sphere, const AABB& box);

		// 判断ray是否与线段v0、v1相交，若ray与线段的距离小于fMaxDistance则返回true
		static bool Intersects(const Ray& ray, const Vector3& v0, const Vector3& v1, float fMaxDistance);
		
		static uint32 FastHash (const char * data, int len, uint32 hashSoFar = 0);

        static const float POS_INFINITY;
        static const float NEG_INFINITY;
        static const float PI;
        static const float TWO_PI;
        static const float HALF_PI;
		static const float FEPS;
		static const float FMAX;


};

static const int M_MIN_INT = 0x80000000;
static const int M_MAX_INT = 0x7fffffff;
static const UINT M_MIN_UNSIGNED = 0x00000000;
static const UINT M_MAX_UNSIGNED = 0xffffffff;


// =====================================================
//	class Vec2
// =====================================================

template<typename T>
class Vec2{
public:
	T x;
	T y;
public:
	Vec2(){
	};

	explicit Vec2(T *p){
		this->x= p[0];
		this->y= p[1];
	}

	explicit Vec2(T xy){
		this->x= xy;
		this->y= xy;
	}

	template<typename S>
	explicit Vec2(const Vec2<S> &v){
		this->x= v.x;
		this->y= v.y;
	}

	Vec2(T x, T y){
		this->x= x;
		this->y= y;
	}

	T *ptr(){
		return reinterpret_cast<T*>(this);
	}

	const T *ptr() const{
		return reinterpret_cast<const T*>(this);
	}

	bool operator ==(const Vec2<T> &v) const{
		return x==v.x && y==v.y;
	}

	bool operator !=(const Vec2<T> &v) const{
		return x!=v.x || y!=v.y;
	}

	Vec2<T> operator +(const Vec2<T> &v) const{
		return Vec2(x+v.x, y+v.y);
	}

	Vec2<T> operator -(const Vec2<T> &v) const{
		return Vec2(x-v.x, y-v.y);
	}

	Vec2<T> operator -() const{
		return Vec2(-x, -y);
	}

	Vec2<T> operator *(const Vec2<T> &v) const{
		return Vec2(x*v.x, y*v.y);
	}

	Vec2<T> operator *(T s) const{
		return Vec2(x*s, y*s);
	}

	Vec2<T> operator /(const Vec2<T> &v) const{
		return Vec2(x/v.x, y/v.y);
	}

	Vec2<T> operator /(T s) const{
		return Vec2(x/s, y/s);
	}

	Vec2<T> operator +=(const Vec2<T> &v){
		x+=v.x; 
		y+=v.y;
		return *this;
	}

	Vec2<T> operator -=(const Vec2<T> &v){
		x-=v.x; 
		y-=v.y;
		return *this;
	}

	Vec2<T> lerp(T t, const Vec2<T> &v) const{
		return *this + (v - *this)*t;
	}

	T dot(const Vec2<T> &v) const{
		return x*v.x+y*v.y;
	}

	float dist(const Vec2<T> &v) const{
		return Vec2<T>(v-*this).length();
	}

	float length() const{
		return static_cast<float>(sqrt(static_cast<float>(x*x + y*y)));
	}

	float dist2q(const Vec2<T> &v) const{
		return Vec2<T>(v-*this).length2q();
	}

	float length2q() const{
		return static_cast<float>(static_cast<float>(x*x + y*y));
	}

	void normalize(){
		T m= length(); 
		x/= m;
		y/= m;
	}

	friend std::ostream& operator << (std::ostream& _stream, const Vec2<T>&  _value)
	{
		_stream << _value.x << " " << _value.y;
		return _stream;
	}
};

typedef Vec2<int> Vec2i;
typedef Vec2<bool> Vec2b;
typedef Vec2<char> Vec2c;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<uint8> Vec2i8;
typedef Vec2<uint16> Vec2i16;



template<typename T>
class Vec3{
public:
	T x;
	T y;
	T z;

public:
	Vec3(){
	};

	explicit Vec3(T *p){
		this->x= p[0];
		this->y= p[1];
		this->z= p[2];
	}

	explicit Vec3(T xyz){
		this->x= xyz;
		this->y= xyz;
		this->z= xyz;
	}

	template<typename S>
	explicit Vec3(const Vec3<S> &v){
		this->x= v.x;
		this->y= v.y;
		this->z= v.z;
	}

	Vec3(T x, T y, T z){
		this->x= x;
		this->y= y;
		this->z= z;
	}

	// 		explicit Vec3(Vec4<T> v){
	// 			this->x= v.x;
	// 			this->y= v.y;
	// 			this->z= v.z;
	// 		}

	T *ptr(){
		return reinterpret_cast<T*>(this);
	}

	const T *ptr() const{
		return reinterpret_cast<const T*>(this);
	}

	bool operator ==(const Vec3<T> &v) const{
		return x==v.x && y==v.y && z==v.z;
	}

	bool operator !=(const Vec3<T> &v) const{
		return x!=v.x || y!=v.y || z!=v.z;
	}

	Vec3<T> operator +(const Vec3<T> &v) const{
		return Vec3(x+v.x, y+v.y, z+v.z);
	}

	Vec3<T> operator -(const Vec3<T> &v) const{
		return Vec3(x-v.x, y-v.y, z-v.z);
	}

	Vec3<T> operator -() const{
		return Vec3(-x, -y, -z);
	}

	Vec3<T> operator *(const Vec3<T> &v) const{
		return Vec3(x*v.x, y*v.y, z*v.z);
	}

	Vec3<T> operator *(T s) const{
		return Vec3(x*s, y*s, z*s);
	}

	Vec3<T> operator /(const Vec3<T> &v) const{
		return Vec3(x/v.x, y/v.y, z/v.z);
	}

	Vec3<T> operator /(T s) const{
		return Vec3(x/s, y/s, z/s);
	}

	Vec3<T> operator +=(const Vec3<T> &v){
		x+=v.x; 
		y+=v.y;
		z+=v.z;
		return *this;
	}

	Vec3<T> operator -=(const Vec3<T> &v){
		x-=v.x; 
		y-=v.y;
		z-=v.z;
		return *this;
	}

	Vec3<T> lerp(T t, const Vec3<T> &v) const{
		return *this + (v - *this) * t;
	}

	T dot(const Vec3<T> &v) const{
		return x*v.x + y*v.y + z*v.z;
	}

	float dist(const Vec3<T> &v) const{
		return Vec3<T>(v-*this).length();
	}

	float dist2q(const Vec3<T> &v) const{
		return Vec3<T>(v-*this).length2q();
	}

	float length() const{
		return static_cast<float>(sqrt((float)(x*x + y*y + z*z)));
	}

	float length2q() const{
		return static_cast<float>(static_cast<float>(x*x + y*y + z*z));
	}

	void normalize(){
		T m= length(); 
		x/= m;
		y/= m;
		z/= m;
	}

	Vec3<T> getNormalized() const{
		T m= length();
		return Vec3<T>(x/m, y/m, z/m);
	}

	Vec3<T> cross(const Vec3<T> &v) const{
		return Vec3<T>(
			this->y*v.z-this->z*v.y,
			this->z*v.x-this->x*v.z,
			this->x*v.y-this->y*v.x);
	}

	Vec3<T> normal(const Vec3<T> &p1, const Vec3<T> &p2) const{
		Vec3<T> rv;
		rv= (p2-*this).cross(p1-*this);
		rv.normalize();
		return rv;
	}

	Vec3<T> normal(const Vec3<T> &p1, const Vec3<T> &p2, const Vec3<T> &p3, const Vec3<T> &p4) const{
		Vec3<T> rv;

		rv= this->normal(p1, p2);
		rv= rv + this->normal(p2, p3);
		rv= rv + this->normal(p3, p4);
		rv= rv + this->normal(p4, p1);
		rv.normalize();
		return rv;
	}

};

typedef Vec3<int> Vec3i;
typedef Vec3<bool> Vec3b;
typedef Vec3<char> Vec3c;
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;


template<typename T>
struct TSize
{
	T width;
	T height;

	TSize() :
	width(0),
		height(0)
	{
	}

	TSize(T const& _width, T const& _height) :
	width(_width),
		height(_height)
	{
	}

	TSize(TSize const& _obj) :
	width(_obj.width),
		height(_obj.height)
	{
	}

	TSize& operator -= (TSize const& _obj)
	{
		width -= _obj.width;
		height -= _obj.height;
		return *this;
	}

	TSize& operator += (TSize const& _obj)
	{
		width += _obj.width;
		height += _obj.height;
		return *this;
	}

	TSize operator - (TSize const& _obj) const
	{
		return TSize(width - _obj.width, height - _obj.height);
	}

	TSize operator + (TSize const& _obj) const
	{
		return TSize(width + _obj.width, height + _obj.height);
	}

	TSize& operator = (TSize const& _obj)
	{
		width = _obj.width;
		height = _obj.height;
		return *this;
	}

	template<typename U>
	TSize& operator = (TSize<U> const& _obj)
	{
		width = _obj.width;
		height = _obj.height;
		return *this;
	}

	bool operator == (TSize const& _obj) const
	{
		return ((width == _obj.width) && (height == _obj.height));
	}

	bool operator != (TSize const& _obj) const
	{
		return !((width == _obj.width) && (height == _obj.height));
	}

	void clear()
	{
		width = height = 0;
	}

	void set(T const& _width, T const& _height)
	{
		width = _width;
		height = _height;
	}

	void swap(TSize& _value)
	{
		TSize tmp = _value;
		_value = *this;
		*this = tmp;
	}

	bool empty() const
	{
		return ((width == 0) && (height == 0));
	}

	std::string print() const
	{
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	static TSize<T> parse(const std::string& _value)
	{
		TSize<T> result;
		std::istringstream stream(_value);
		stream >> result.width >> result.height;
		if (stream.fail())
		{
			return TSize<T>();
		}
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return TSize<T>();
				item = stream.get();
			}
		}
		return result;
	}

	friend std::ostream& operator << (std::ostream& _stream, const TSize<T>&  _value)
	{
		_stream << _value.width << " " << _value.height;
		return _stream;
	}

	friend std::istream& operator >> (std::istream& _stream, TSize<T>&  _value)
	{
		_stream >> _value.width >> _value.height;
		if (_stream.fail())
			_value.clear();
		return _stream;
	}
};
typedef TSize<int> IntSize;
typedef TSize<float> FloatSize;


class Randomizer
{
public:

	//! resets the randomizer
	static void reset();

	//! generates a pseudo random number
	static int rand();
	static Real frand();
private:
	static const int m = 2147483399;			// a non-Mersenne prime
	static const int a = 40692;					// another spectral success story
	static const int q = 2147483399/40692;
	static const int r = 2147483399%40692;		// again less than q
	static const float fMax;

	static int seed;
};

}
#endif
