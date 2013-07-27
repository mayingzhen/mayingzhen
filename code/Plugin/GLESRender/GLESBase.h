#ifndef __GLESBASE_H_
#define __GLESBASE_H_


// Graphics (OpenGL)
#ifdef __ANDROID__
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
	extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
	extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
	extern PFNGLISVERTEXARRAYOESPROC glIsVertexArray;
	#define glMapBuffer glMapBufferOES
	#define glUnmapBuffer glUnmapBufferOES
	#define GL_WRITE_ONLY GL_WRITE_ONLY_OES
	#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
	#define glClearDepth glClearDepthf
	#define OPENGL_ES
#elif WIN32
	#define WIN32_LEAN_AND_MEAN
	#define GLEW_STATIC
	#include <GL/glew.h>
	#define USE_VAO
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



// Graphics (GLSL)
#define VERTEX_ATTRIBUTE_POSITION_NAME              "a_position"
#define VERTEX_ATTRIBUTE_NORMAL_NAME                "a_normal"
#define VERTEX_ATTRIBUTE_COLOR_NAME                 "a_color"
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
#define GL_ASSERT( gl_code ) do \
{ \
	gl_code; \
	__gl_error_code = glGetError(); \
	ASSERT(__gl_error_code == GL_NO_ERROR); \
} while(0)
#endif

extern GLenum __gl_error_code;



#endif
