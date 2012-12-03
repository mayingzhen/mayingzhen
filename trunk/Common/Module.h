#ifndef	COMMON_MODULE__H__
#define  COMMON_MODULE__H__

#include "Common/CommonExport.h"
#include "Common/Common.h"
#include "Common/maMath.h"

#include "Common/IRender.h"
#include "Common/IRenderDevice.h"
#include "Common/IRendMesh.h"
#include "Common/IRendTexture.h"
#include "Common/IPhysicsScene.h"

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


COMMON_API void Log(const char* fmt,...);
COMMON_API void SSERT_MSG(bool expr,const char* fmt,...);
COMMON_API void LogError(xmUint nErrorCode,const char* fmt,...);

COMMON_API void  xmVec3Min(xmVector3* pOut,const xmVector3* pA,const xmVector3* pB);
COMMON_API void  xmVec3Max(xmVector3* pOut,const xmVector3* pA,const xmVector3* pB);


COMMON_API void CommonModuleInit();
COMMON_API void CommonModuleShutdown();

#endif
