#ifndef	COMMON_MODULE__H__
#define  COMMON_MODULE__H__

#include "Common/Export.h"


#include "Common/BaseType.h"
#include "Common/Macro.h"

#include "Common/Platform/Platform.h"

#include "Common/Assert.h"
#include "Common/Log.h"

#include "Common/Math/Math.h"
#include "Common/Math/MathBase.h"

#include "Common/AABB.h"
#include "Common/Frustum.h"

// RTTI
#include "Common/RTTI/Object.h"
#include "Common/RTTI/Property.h"
#include "Common/RTTI/Class.h"
#include "Common/RTTI/ObjectFactory.h"

// Utile
#include "Common/Singleton.h"
#include "Common/RefCountObject.h"
#include "Common/BitField.h"


COMMON_API void CommonModuleInit();

COMMON_API void CommonModuleShutdown();



#endif
