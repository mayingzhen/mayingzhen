#include "Common/Module.h"

// Math
#include "Common/Math/Math.hxx"
#include "Common/Math/MathBase_Portable.hxx"

#include "Common/Platform/Platform.hxx"

#include "Common/AABB.hxx"
#include "Common/Frustum.hxx"

#include "Common/Assert.hxx"
#include "Common/Log.hxx"
#include "Common/RefCountObject.hxx"

// RTTI
#include "Common/RTTI/Class.hxx"
#include "Common/RTTI/Object.hxx"
#include "Common/RTTI/ObjectFactory.hxx"




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
