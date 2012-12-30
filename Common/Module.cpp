#include "Common/Common.h"
#include "Common/Module.h"

#include "Common/maMath.hxx"
#include "Common/Timer.hxx"

#include "Common/IRender/IRender.hxx"
#include "Common/IRender/IRenderDevice.hxx"

#include "Common/IPhysics/IPhysicsDevive.hxx"


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
	ma::Time* pTime = new ma::Time();
	ma::SetTimer(pTime);
}

void CommonModuleShutdown()
{
	ma::Time* pTime = ma::GetTimer();	
	SAFE_DELETE(pTime);
}
