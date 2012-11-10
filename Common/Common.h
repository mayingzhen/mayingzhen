#ifndef	COMMON__H__
#define COMMON__H__

#include <Windows.h>

#include <d3dx9math.h>
#include <assert.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#define  F_EPS 1.0e-6f
#define  F_MAX 1.0e+6f

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }



#endif
