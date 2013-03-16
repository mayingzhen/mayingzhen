#ifndef	COMMON_MODULE__H__
#define  COMMON_MODULE__H__

#include "Common/Export.h"
#include "Common/stdafx.h"

// Math
#include "Common/maMath.h"
#include "Common/AABB.h"
#include "Common/Frustum.h"

// RTTI
#include "Common/RTTI/Object.h"
#include "Common/RTTI/Class.h"
#include "Common/RTTI/ObjectFactory.h"

// Utile
#include "Common/Singleton.h"
#include "Common/ReferenceCountObject.h"
#include "Common/BitField.h"


COMMON_API void CommonModuleInit();

COMMON_API void CommonModuleShutdown();



#endif
