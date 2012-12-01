#ifndef	COMMON_MODULE__H__
#define  COMMON_MODULE__H__

#include "Common/CommonExport.h"
#include "Common/Common.h"
#include "Common/maMath.h"

#include "Common/IRender.h"
#include "Common/IRenderDevice.h"
#include "Common/IRendMesh.h"
#include "Common/IRendTexture.h"
#include "Common/IPhysicsScene.h"
#include "Common/ResourceBuilder.h"
#include "Common/IMesh.h"

COMMON_API void CommonModuleInit();
COMMON_API void CommonModuleShutdown();

#endif
