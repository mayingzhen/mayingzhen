#ifndef __GLESBASE_H_
#define __GLESBASE_H_


// Graphics (OpenGL)
#ifdef __ANDROID__
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>

	// VAO
	extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
	extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
	extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
	extern PFNGLISVERTEXARRAYOESPROC glIsVertexArray;

	// MapBuffer
	extern PFNGLMAPBUFFEROESPROC glMapBuffer;
	extern PFNGLUNMAPBUFFEROESPROC glUnmapBuffer;
	extern PFNGLGETBUFFERPOINTERVOESPROC glGetBufferPointerv;

	//#define glMapBuffer glMapBufferOES
	//#define glUnmapBuffer glUnmapBufferOES
	#define GL_WRITE_ONLY GL_WRITE_ONLY_OES
	#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
	#define glClearDepth glClearDepthf
	#define OPENGL_ES
	#define  glPushGroupMarkerEXT(x,y)
	#define  glPopGroupMarkerEXT()
#elif WIN32
	#include "GLES2/gl2.h"
	#include "GLES2/gl2ext.h"
	#include "GLES2/egl.h"

	#define WIN32_LEAN_AND_MEAN
	#define GLEW_STATIC
	#define USE_VAO
	//#define GL_OES_mapbuffer	

	#define OPENGL_ES 1

	#define PFNGLMAPBUFFERRANGEPROC PFNGLMAPBUFFERRANGEEXTPROC
	#define PFNGLFLUSHMAPPEDBUFFERRANGEPROC PFNGLFLUSHMAPPEDBUFFERRANGEEXTPROC
	#define GL_MAP_READ_BIT GL_MAP_READ_BIT_EXT
	#define GL_MAP_WRITE_BIT GL_MAP_WRITE_BIT_EXT
	#define GL_MAP_INVALIDATE_RANGE_BIT GL_MAP_INVALIDATE_RANGE_BIT_EXT
	#define GL_MAP_INVALIDATE_BUFFER_BIT GL_MAP_INVALIDATE_BUFFER_BIT_EXT
	#define GL_MAP_FLUSH_EXPLICIT_BIT GL_MAP_FLUSH_EXPLICIT_BIT_EXT
	#define GL_MAP_UNSYNCHRONIZED_BIT GL_MAP_UNSYNCHRONIZED_BIT_EXT

	#define GL_WRITE_ONLY GL_WRITE_ONLY_OES
	#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
	//#define glMapBuffer glMapBufferOES
	//#define glUnmapBuffer glUnmapBufferOES

	#define eglGetProcAddress glewGetProcAddress

#elif __linux__
	#define GLEW_STATIC
	#include <GL/glew.h>
	#define USE_VAO
#elif __APPLE__
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>
		#define glBindVertexArray glBindVertexArrayOES
		#define glDeleteVertexArrays glDeleteVertexArraysOES
		#define glGenVertexArrays glGenVertexArraysOES
		#define glIsVertexArray glIsVertexArrayOES
        #define GL_WRITE_ONLY GL_WRITE_ONLY_OES
        #define glMapBuffer glMapBufferOES
        #define glUnmapBuffer glUnmapBufferOES
		#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
		#define glClearDepth glClearDepthf
		#define OPENGL_ES
		#define USE_VAO
		#ifdef __arm__
		#define USE_NEON
		#endif
	#elif TARGET_OS_MAC
		#include <OpenGL/gl.h>
		#include <OpenGL/glext.h>
		#define glBindVertexArray glBindVertexArrayAPPLE
		#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
		#define glGenVertexArrays glGenVertexArraysAPPLE
		#define glIsVertexArray glIsVertexArrayAPPLE
		#define USE_VAO
	#else
		#error "Unsupported Apple Device"
	#endif
#endif

extern bool g_bGL_OES_packed_depth_stencil;
extern bool g_bGL_OES_vertex_array_object;
extern bool g_bGL_OES_mapbuffer;
extern bool g_bGL_EXT_occlusion_query_boolean;
extern bool g_bGL_EXT_debug_marker;
extern bool g_bGL_APPLE_framebuffer_multisample;

// Graphics (GLSL)
#define VERTEX_ATTRIBUTE_POSITION_NAME              "a_position"
#define VERTEX_ATTRIBUTE_NORMAL_NAME                "a_normal"
#define VERTEX_ATTRIBUTE_COLOR0_NAME                "a_color0"
#define VERTEX_ATTRIBUTE_COLOR1_NAME                "a_color1"
#define VERTEX_ATTRIBUTE_TANGENT_NAME               "a_tangent"
#define VERTEX_ATTRIBUTE_BINORMAL_NAME              "a_binormal"
#define VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME          "a_blendWeights"
#define VERTEX_ATTRIBUTE_BLENDINDICES_NAME          "a_blendIndices"
#define VERTEX_ATTRIBUTE_TEXCOORD0_NAME				"a_texCoord0"
#define VERTEX_ATTRIBUTE_TEXCOORD1_NAME				"a_texCoord1"

// Hardware buffer
namespace ma
{
	/** Vertex attribute. */
	typedef GLint VertexAttribute;
	/** Vertex buffer handle. */
	typedef GLuint VertexBufferHandle;
	/** Index buffer handle. */
	typedef GLuint IndexBufferHandle;
	/** Texture handle. */
	typedef GLuint TextureHandle;
	/** Frame buffer handle. */
	typedef GLuint FrameBufferHandle;
	/** Render buffer handle. */
	typedef GLuint RenderBufferHandle;
	/** Gamepad handle definitions vary by platform. */
	typedef unsigned int GamepadHandle;
}

#ifdef NDEBUG
#define GL_ASSERT( gl_code ) gl_code
#else
#define GL_ASSERT( gl_code ) \
{ \
	gl_code; \
	int e = glGetError(); \
	if (e != 0) \
	{ \
	const char * errorString = ""; \
	switch(e) \
	{ \
		case GL_INVALID_ENUM:       errorString = "GL_INVALID_ENUM";        break; \
		case GL_INVALID_VALUE:      errorString = "GL_INVALID_VALUE";       break; \
		case GL_INVALID_OPERATION:  errorString = "GL_INVALID_OPERATION";   break; \
		case GL_OUT_OF_MEMORY:      errorString = "GL_OUT_OF_MEMORY";       break; \
		default:                                                            break; \
	} \
	char msgBuf[1024]; \
	sprintf(msgBuf, "OpenGL ES2 error 0x%04X %s in %s at line %i, ID:%d\n", e, errorString, __FUNCTION__, __LINE__, e); \
	LogError(msgBuf);\
	} \
}
#endif



#endif
