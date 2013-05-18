#ifndef  _GLESRender_StdAfx__H__
#define  _GLESRender_StdAfx__H__

#include "Common/Module.h"
#include "Engine/Module.h"

//#define  GL_CHECK_ERROR 

#define WIN32_LEAN_AND_MEAN
#define GLEW_STATIC
#include <GL/glew.h>
#define USE_VAO




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
#if defined(__QNX__) && defined(USE_BLACKBERRY_GAMEPAD)
	typedef screen_device_t GamepadHandle;
#elif defined(USE_XINPUT)
	typedef unsigned long GamepadHandle;
#else
	typedef unsigned int GamepadHandle;
#endif
}

#ifdef NDEBUG
#define GL_ASSERT( gl_code ) gl_code
#else
#define GL_ASSERT( gl_code ) do \
{ \
	gl_code; \
	__gl_error_code = glGetError(); \
	GP_ASSERT(__gl_error_code == GL_NO_ERROR); \
} while(0)
#endif

extern GLenum __gl_error_code;

// Assert macros.
#ifdef _DEBUG
#define GP_ASSERT(expression) assert(expression)
#else
#define GP_ASSERT(expression)
#endif

// Error macro.
#ifdef GP_ERRORS_AS_WARNINGS
#define GP_ERROR GP_WARN
#else
#define GP_ERROR(...) do \
{ \
	assert(0); \
	std::exit(-1); \
} while (0)
#endif

#include <algorithm>
#include <cmath>
#include <cctype>

using std::memcpy;
using std::fabs;
using std::sqrt;
using std::cos;
using std::sin;
using std::tan;
using std::isspace;
using std::isdigit;
using std::toupper;
using std::tolower;
using std::size_t;
using std::min;
using std::max;
using std::modf;

#endif