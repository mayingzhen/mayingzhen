#ifndef	COMMON__H__
#define COMMON__H__

#include <Windows.h>

#include <d3dx9math.h>
#include <assert.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

#include "Common/xmBaseType.h"

#define  xmVector3 D3DXVECTOR3
#define  xmVector2 D3DXVECTOR2
#define  xmVector4 D3DXVECTOR4
#define  xmQuaternion D3DXQUATERNION
#define  S3AGUID maGUID
#define  xmMatrix4x4 D3DXMATRIX
#define  xmNodeTransform maNodeTransform

// #define S3A_DXTRACE_MSG(str)              DXTrace( __FILE__, (xmUint32)__LINE__, 0, str, FALSE )
// #define S3A_DXTRACE_ERR(str,hr)           DXTrace( __FILE__, (xmUint32)__LINE__, hr, str, FALSE )
// #define S3A_DXTRACE_ERR_MSGBOX(str,hr)    DXTrace( __FILE__, (xmUint32)__LINE__, hr, str, TRUE )

//#define S3A_D3D_V(x)				{ HRESULT hr = x; if(FAILED(hr)){ S3A_DXTRACE_ERR( L#x,hr); } }
#define S3A_D3D_V(x)				x

#define S3ASSERT(x) 

void S3ALog(const char* fmt,...);
void S3ASSERT_MSG(bool expr,const char* fmt,...);
void S3ALogError(xmUint nErrorCode,const char* fmt,...);

template<class T>
inline T S3AInvalidID()
{
	return T(-1);
}

enum ES3AErrorCode
{
	S3A_OK = 0,
	S3A_ERR_KEY_CONFLICT,
	S3A_ERR_INVALID_CALL,
	S3A_ERR_RETARGET,
	S3A_ERR_FILE_FAIL_TO_OPEN,
	S3A_ERR_NUM
};


#define xm_EPS 1.0e-6f
#define xm_FMAX 1.0e+6f
#define xm_FNAN (xm_FMAX+1.0f)
#define xm_MIN_NODE_SCALE 0.0001f

#define S3A_SAFE_FREE(p)			{ if(p){ free (p); (p) = 0;} }

#define xmSafeDelete(p)			{ if(p){ delete (p); (p) = 0;} }

#define S3A_SAFE_DELETE_ARRAY(p)	{ if(p){ delete [] (p); (p) = 0;} }

#define S3A_SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }

#define S3A_SAFE_SET(r,l)		{ if(l)	{(l)->AddRef();} S3A_SAFE_RELEASE(r); (r) = (l); }

#define S3A_SAFE_INC_REF(p)			{ if(p){ (p)->IncReference();} }

#define S3A_SAFE_DEC_REF(p)			{ if(p){ (p)->DecReference();} (p) = NULL;}

#define S3A_SAFE_SET_REF(pl,pr)		{ S3A_SAFE_INC_REF(pr); S3A_SAFE_DEC_REF(pl); (pl) = (pr);}


#define  F_EPS 1.0e-6f
#define  F_MAX 1.0e+6f

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

void  xmVec3Min(xmVector3* pOut,const xmVector3* pA,const xmVector3* pB);


void  xmVec3Max(xmVector3* pOut,const xmVector3* pA,const xmVector3* pB);



#endif
