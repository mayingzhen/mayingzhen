#ifndef	COMMON__H__
#define COMMON__H__

#include <Windows.h>
#include <time.h>

#include <d3dx9math.h>
#include <assert.h>
#include <math.h>

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

//#define  D3DXVECTOR3 D3DXVECTOR3
//#define  D3DXVECTOR2 D3DXVECTOR2
//#define  D3DXVECTOR4 D3DXVECTOR4
//#define  D3DXQUATERNION D3DXQUATERNION
#define  GUID maGUID
//#define  D3DXMATRIX D3DXMATRIX
//#define  maNodeTransform maNodeTransform

// #define _DXTRACE_MSG(str)              DXTrace( __FILE__, (xmUint32)__LINE__, 0, str, FALSE )
// #define _DXTRACE_ERR(str,hr)           DXTrace( __FILE__, (xmUint32)__LINE__, hr, str, FALSE )
// #define _DXTRACE_ERR_MSGBOX(str,hr)    DXTrace( __FILE__, (xmUint32)__LINE__, hr, str, TRUE )

//#define _D3D_V(x)				{ HRESULT hr = x; if(FAILED(hr)){ _DXTRACE_ERR( L#x,hr); } }
#define _D3D_V(x)				x

#define SSERT(x) 
#define ASSERT assert


enum EErrorCode
{
	_OK = 0,
	_ERR_KEY_CONFLICT,
	_ERR_INVALID_CALL,
	_ERR_RETARGET,
	_ERR_FILE_FAIL_TO_OPEN,
	_ERR_NUM
};


#define xm_EPS 1.0e-6f
#define xm_FMAX 1.0e+6f
#define xm_FNAN (xm_FMAX+1.0f)
#define xm_MIN_NODE_SCALE 0.0001f

#define _SAFE_FREE(p)			{ if(p){ free (p); (p) = 0;} }

#define xmSafeDelete(p)			{ if(p){ delete (p); (p) = 0;} }

#define _SAFE_DELETE_ARRAY(p)	{ if(p){ delete [] (p); (p) = 0;} }

#define _SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }

#define _SAFE_SET(r,l)		{ if(l)	{(l)->AddRef();} _SAFE_RELEASE(r); (r) = (l); }

#define _SAFE_INC_REF(p)			{ if(p){ (p)->IncReference();} }

#define _SAFE_DEC_REF(p)			{ if(p){ (p)->DecReference();} (p) = NULL;}

#define _SAFE_SET_REF(pl,pr)		{ _SAFE_INC_REF(pr); _SAFE_DEC_REF(pl); (pl) = (pr);}


#define  F_EPS 1.0e-6f
#define  F_MAX 1.0e+6f

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } 


#endif
