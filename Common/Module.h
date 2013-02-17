#ifndef	COMMON_MODULE__H__
#define  COMMON_MODULE__H__

#include "Common/CommonExport.h"
#include "Common/stdafx.h"
#include "Common/maMath.h"
#include "Common/BitField.h"

#include "Common/IRender/IRender.h"
#include "Common/IRender/IRenderDevice.h"
#include "Common/IRender/IRendMesh.h"
#include "Common/IRender/IRendTexture.h"

#include "Common/Serialize/SerializeListener.h"
#include "Common/Serialize/BinaryInputArchive.h"
#include "Common/Serialize/BinaryOutputArchive.h"
#include "Common/Serialize/XMLInputArchive.h"
#include "Common/Serialize/XMLOutputArchive.h"

#include "Common/Data/MeshData.h"
#include "Common/Data/SkeletonData.h"
#include "Common/Data/AnimationData.h"

///TinyXMl
#include "Common/tinyxml/tinyxml.h"

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
COMMON_API void LogError(xmUint nErrorCode,const char* fmt,...);

COMMON_API void  xmVec3Min(D3DXVECTOR3* pOut,const D3DXVECTOR3* pA,const D3DXVECTOR3* pB);
COMMON_API void  xmVec3Max(D3DXVECTOR3* pOut,const D3DXVECTOR3* pA,const D3DXVECTOR3* pB);

namespace ma
{
	COMMON_API void CommonModuleInit();

	COMMON_API void CommonModuleShutdown();
}


#endif
