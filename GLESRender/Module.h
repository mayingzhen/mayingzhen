#ifndef  _GLESRender_Module__H__
#define  _GLESRender_Module__H__


#include "GLESRender/stdafx.h"


//#include <GLES2/gl2.h>
//#include <EGL/egl.h>

#include "GLESRender/Export.h"
//#include "GLESRender/Base.h"
#include "GLESRender/GLESRenderDevice.h"
#include "GLESRender/GLESTexture.h"
#include "GLESRender/GLESVertexBuffer.h"
#include "GLESRender/GLESIndexBuffer.h"
#include "GLESRender/GLESMapping.h"
#include "GLESRender/GLESVertexDeclaration.h"
#include "GLESRender/GLESTechnique.h"
#include "GLESRender/Effect.h"
#include "GLESRender/Stream.h"
#include "GLESRender/FileSystem.h"






GLESRENDER_API void GLESRenderModuleInit();

GLESRENDER_API void GLESRenderModuleShutdown();

#endif
