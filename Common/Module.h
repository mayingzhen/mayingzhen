#ifndef	COMMON_MODULE__H__
#define  COMMON_MODULE__H__

#include "Common/CommonExport.h"
#include "Common/stdafx.h"
#include "Common/maMath.h"
#include "Common/BitField.h"

// RTTI
#include "Common/RTTI/Object.h"
#include "Common/RTTI/Class.h"
#include "Common/RTTI/ObjectFactory.h"

// Singleton
#include "Common/Singleton.h"

#include "Common/ReferenceCountObject.h"


COMMON_API void CommonModuleInit();

COMMON_API void CommonModuleShutdown();



#endif
