#ifndef __Macro_h__
#define __Macro_h__

#define SAFE_SET(r,l)		{ if(l)	{(l)->AddRef();} SAFE_RELEASE(r); (r) = (l); }
#define SAFE_INC_REF(p)			{ if(p){ (p)->Ref();} }
#define SAFE_DEC_REF(p)			{ if(p){ (p)->Unref();} (p) = NULL;}
#define SAFE_SET_REF(pl,pr)		{ SAFE_INC_REF(pr); SAFE_DEC_REF(pl); (pl) = (pr);}

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


//Math
// maps unsigned 8 bits/channel to D3DCOLOR
#define COLOR_ARGB(a,r,g,b) \
	((COLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define COLOR_RGBA(r,g,b,a) COLOR_ARGB(a,r,g,b)
#define COLOR_XRGB(r,g,b)   COLOR_ARGB(0xff,r,g,b)

#define COLOR_XYUV(y,u,v)   COLOR_ARGB(0xff,y,u,v)
#define COLOR_AYUV(a,y,u,v) COLOR_ARGB(a,y,u,v)

// maps floating point channels (0.f to 1.f range) to D3DCOLOR
#define COLOR_COLORVALUE(r,g,b,a) \
	COLOR_RGBA((Uint32)((r)*255.f),(Uint32)((g)*255.f),(Uint32)((b)*255.f),(Uint32)((a)*255.f))


//===========================================================================
//
// General purpose utilities
//
//===========================================================================
#define PI    ((float)  3.141592654f)
#define TWO_PI (2.0f*PI)
#define HALF_PI (0.5f*PI)
#define _1BYPI ((float)  0.318309886f)

#define ToRadian( degree ) ((degree) * (PI / 180.0f))
#define ToDegree( radian ) ((radian) * (180.0f / PI))


#define FEPS 1.0e-6f
#define FMAX 1.0e+6f
#define FNAN (FMAX+1.0f)
#define MIN_NODE_SCALE 0.0001f



//===========================================================================
//
// 16 bit floating point numbers
//
//===========================================================================
#define _16F_DIG          3                // # of decimal digits of precision
#define _16F_EPSILON      4.8875809e-4f    // smallest such that 1.0 + epsilon != 1.0
#define _16F_MANT_DIG     11               // # of bits in mantissa
#define _16F_MAX          6.550400e+004    // max value
#define _16F_MAX_10_EXP   4                // max decimal exponent
#define _16F_MAX_EXP      15               // max binary exponent
#define _16F_MIN          6.1035156e-5f    // min positive value
#define _16F_MIN_10_EXP   (-4)             // min decimal exponent
#define _16F_MIN_EXP      (-14)            // min binary exponent
#define _16F_RADIX        2                // exponent radix
#define _16F_ROUNDS       1                // addition rounding: near


// Math
#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)
#define MATH_RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f)      // Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.
#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f
#define MATH_E                      2.71828182845904523536f
#define MATH_LOG10E                 0.4342944819032518f
#define MATH_LOG2E                  1.442695040888963387f
#define MATH_PI                     3.14159265358979323846f
#define MATH_PIOVER2                1.57079632679489661923f
#define MATH_PIOVER4                0.785398163397448309616f
#define MATH_PIX2                   6.28318530717958647693f
#define MATH_EPSILON                0.000001f
#define MATH_CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))
#define M_1_PI                      0.31830988618379067154


#endif
