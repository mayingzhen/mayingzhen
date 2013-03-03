#ifndef	COMMON_MODULE__H__
#define  COMMON_MODULE__H__

#include "Common/CommonExport.h"
#include "Common/stdafx.h"
#include "Common/maMath.h"
#include "Common/BitField.h"

// RTTI
#include "Common/RTTI/Object.h"
#include "Common/RTTI/Class.h"
#include "Common/RTTI/ObjectFactory.h"

// Singleton
#include "Common/Singleton.h"

#include "Common/ReferenceCountObject.h"


template<class T>
T InvalidID()
{
	return T(-1);
}

template<class T> 
bool IsValidID(T ind)
{
	return ind != (T)(-1);
}

template<class T> 
bool IsInValidID(T ind)
{
	return ind == (T)(-1);
}

template<class T>
inline const T& maMax(const T& a,const T& b)
{
	return a > b ? a : b;
}



COMMON_API void Log(const char* fmt,...);
COMMON_API void SSERT_MSG(bool expr,const char* fmt,...);
COMMON_API void LogError(UINT nErrorCode,const char* fmt,...);

COMMON_API void  xmVec3Min(D3DXVECTOR3* pOut,const D3DXVECTOR3* pA,const D3DXVECTOR3* pB);
COMMON_API void  xmVec3Max(D3DXVECTOR3* pOut,const D3DXVECTOR3* pA,const D3DXVECTOR3* pB);

namespace ma
{
	COMMON_API void CommonModuleInit();

	COMMON_API void CommonModuleShutdown();
}


#endif
