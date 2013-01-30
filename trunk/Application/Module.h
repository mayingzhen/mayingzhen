#ifndef  _Application_Moudule__H__
#define  _Application_Moudule__H__

#include "Application/Export.h"
#include "Application/Input/Input.h"
#include "Application/Timer/Timer.h"

#include "Application/SceneView.h"
#include "Application/Application.h"

//#ifdef WIN32
#include "Application/ApplicationWin.h"
//#endif

Application_API void ApplicationModuleInit();

Application_API void ApplicationModuleShutdown();

#endif
