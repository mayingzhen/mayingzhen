#include "Common/stdafx.h"
#include "Common/Module.h"


// Math
#include "Common/maMath.hxx"
#include "Common/AABB.hxx"
#include "Common/Frustum.hxx"


// RTTI
#include "Common/RTTI/Class.hxx"
#include "Common/RTTI/Object.hxx"
#include "Common/RTTI/ObjectFactory.hxx"


#include "Common/ReferenceCountObject.hxx"


using namespace ma;

void CommonModuleInit()
{
	ClassManager* pClsMan = new ClassManager();

	ObjectFactoryManager* pObjMan = new ObjectFactoryManager();

	Object::StaticInitClass();
}

void CommonModuleShutdown()
{
	Object::StaticShutdownClass();

	ObjectFactoryManager::GetInstance().Shutdown();
	delete ObjectFactoryManager::GetInstancePtr();

	ClassManager::GetInstance().Shutdown();
	delete ClassManager::GetInstancePtr();
}
