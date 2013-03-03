#include "Common/stdafx.h"
#include "Common/Module.h"

#include "Common/maMath.hxx"

// RTTI
#include "Common/RTTI/Class.hxx"
#include "Common/RTTI/Object.hxx"
#include "Common/RTTI/ObjectFactory.hxx"

#include "Common/ReferenceCountObject.hxx"


using namespace ma;


void Log(const char* fmt,...) {}
void SSERT_MSG(bool expr,const char* fmt,...){}
void LogError(UINT nErrorCode,const char* fmt,...){}

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

namespace ma
{

#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
	#include <Common/RTTIDecl.h>
#undef RTTI_DECL

	void CommonModuleInit()
	{
		ClassManager* pClsMan = new ClassManager();
		new ObjectFactoryManager();

		Object::StaticInitClass();

	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType)
		#include <Common/RTTIDecl.h>
	#undef RTTI_DECL
	}

	void CommonModuleShutdown()
	{
		Object::StaticShutdownClass();

		ObjectFactoryManager::GetInstance().Shutdown();
		delete ObjectFactoryManager::GetInstancePtr();

		ClassManager::GetInstance().Shutdown();
		delete ClassManager::GetInstancePtr();
	}

}

