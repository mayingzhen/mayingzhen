#include "Common/stdafx.h"
#include "Common/Module.h"

#include "Common/maMath.hxx"

#include "Common/IRender/IRender.hxx"
#include "Common/IRender/IRenderDevice.hxx"

#include "Common/Serialize/SerializeListener.hxx"
#include "Common/Serialize/BinaryInputArchive.hxx"
#include "Common/Serialize/BinaryOutputArchive.hxx"

#include "Common/Data/MeshData.hxx"
#include "Common/Data/AnimationData.hxx"
#include "Common/Data/SkeletonData.hxx"




void Log(const char* fmt,...) {}
void SSERT_MSG(bool expr,const char* fmt,...){}
void LogError(xmUint nErrorCode,const char* fmt,...){}

void  xmVec3Min(D3DXVECTOR3* pOut,const D3DXVECTOR3* pA,const D3DXVECTOR3* pB)
{
	pOut->x = pA->x < pB->x ? pA->x : pB->x;
	pOut->y = pA->y < pB->y ? pA->y : pB->y;
	pOut->z = pA->z < pB->z ? pA->z : pB->z;
}


void  xmVec3Max(D3DXVECTOR3* pOut,const D3DXVECTOR3* pA,const D3DXVECTOR3* pB)
{
	pOut->x = pA->x > pB->x ? pA->x : pB->x;
	pOut->y = pA->y > pB->y ? pA->y : pB->y;
	pOut->z = pA->z > pB->z ? pA->z : pB->z;
}


void CommonModuleInit()
{
}

void CommonModuleShutdown()
{
}
