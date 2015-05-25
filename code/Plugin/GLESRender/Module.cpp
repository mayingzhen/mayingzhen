#include "GLESRender/stdafx.h"
#include "Module.h"

bool g_bGL_OES_packed_depth_stencil = false;
bool g_bGL_OES_vertex_array_object = false;
bool g_bGL_OES_mapbuffer = false;
//bool g_bGL_EXT_occlusion_query_boolean = false;
//bool g_bGL_EXT_debug_marker = false;
bool g_bGL_APPLE_framebuffer_multisample = false;

// OpenGL VAO functions.
// #ifdef __ANDROID__
// PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES = NULL;
// PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES = NULL;
// PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES = NULL;
// PFNGLISVERTEXARRAYOESPROC glIsVertexArrayOES = NULL;
// #endif

// #ifndef GL_DEPTH_COMPONENT16_OES
// #define GL_DEPTH_COMPONENT16_OES                                0x81A5
// #endif
// 
// #ifndef GL_ETC1_RGB8_OES
// #define GL_ETC1_RGB8_OES        0
// #endif
// 
// #define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG	0x8C00		
// #define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG	0x8C01
// #define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG	0x8C02	
// #define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG	0x8C03
// 
// #define GL_ATC_RGB_AMD						0x8C92
// #define GL_ATC_RGBA_EXPLICIT_ALPHA_AMD		0x8C93
// #define GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD	0x87EE

GLenum __gl_error_code = GL_NO_ERROR;


#include "GLESRender/GLESTexture.hxx"
#include "GLESRender/GLESVertexBuffer.hxx"
#include "GLESRender/GLESIndexBuffer.hxx"
#include "GLESRender/GLESMapping.hxx"
#include "GLESRender/GLESVertexDeclaration.hxx"
#include "GLESRender/GLESShaderProgram.hxx"
#include "GLESRender/GLESRenderDevice.hxx"
#include "GLESRender/DeviceContext/GLESDeviceContext.hxx"


void GLESRenderModuleInit()
{
	ma::GLESRenderDevice* pGLESRenderDevice = new ma::GLESRenderDevice();
	ma::SetRenderDevice(pGLESRenderDevice);
}


void GLESRenderModuleShutdown()
{
	ma::GLESRenderDevice* pGLESRenderDevice = (ma::GLESRenderDevice*)ma::GetRenderDevice();
	SAFE_DELETE(pGLESRenderDevice);
}

#if PLATFORM_WIN == 1
extern "C" GLESRENDER_API bool dllStartPlugin()
{
	GLESRenderModuleInit();

	return true;
}


extern "C" GLESRENDER_API bool dllStopPlugin()
{
	GLESRenderModuleShutdown();

	return true;
}
#endif