#include "Common/Common.h"


void Log(const char* fmt,...) {}
void SSERT_MSG(bool expr,const char* fmt,...){}
void LogError(xmUint nErrorCode,const char* fmt,...){}

void  xmVec3Min(xmVector3* pOut,const xmVector3* pA,const xmVector3* pB)
{
	pOut->x = pA->x < pB->x ? pA->x : pB->x;
	pOut->y = pA->y < pB->y ? pA->y : pB->y;
	pOut->z = pA->z < pB->z ? pA->z : pB->z;
}


void  xmVec3Max(xmVector3* pOut,const xmVector3* pA,const xmVector3* pB)
{
	pOut->x = pA->x > pB->x ? pA->x : pB->x;
	pOut->y = pA->y > pB->y ? pA->y : pB->y;
	pOut->z = pA->z > pB->z ? pA->z : pB->z;
}
