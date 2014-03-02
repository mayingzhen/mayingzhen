#include "GLESDeviceContextAndroid.h"

#include <android/log.h>

namespace ma
{
	static EGLDisplay	__eglDisplay = EGL_NO_DISPLAY;
	static EGLContext	__eglContext = EGL_NO_CONTEXT;
	static EGLSurface	__eglSurface = EGL_NO_SURFACE;
	static EGLConfig	__eglConfig = 0;
	static int			__orientationAngle = 90;

	static const char* __glExtensions;

	bool g_bHardwareETCSupported = false;
	bool g_bHardwareATCSupported = false;
	bool g_bHardwareDDSSupported = false;

	void print(const char* format, ...)
	{
		ASSERT(format);
		va_list argptr;
		va_start(argptr, format);
		__android_log_vprint(ANDROID_LOG_INFO, "native-activity", format, argptr);
		va_end(argptr);
	}


	static EGLenum checkErrorEGL(const char* msg)
	{
		ASSERT(msg);
		static const char* errmsg[] =
		{
			"EGL function succeeded",
			"EGL is not initialized, or could not be initialized, for the specified display",
			"EGL cannot access a requested resource",
			"EGL failed to allocate resources for the requested operation",
			"EGL fail to access an unrecognized attribute or attribute value was passed in an attribute list",
			"EGLConfig argument does not name a valid EGLConfig",
			"EGLContext argument does not name a valid EGLContext",
			"EGL current surface of the calling thread is no longer valid",
			"EGLDisplay argument does not name a valid EGLDisplay",
			"EGL arguments are inconsistent",
			"EGLNativePixmapType argument does not refer to a valid native pixmap",
			"EGLNativeWindowType argument does not refer to a valid native window",
			"EGL one or more argument values are invalid",
			"EGLSurface argument does not name a valid surface configured for rendering",
			"EGL power management event has occurred",
		};
		EGLenum error = eglGetError();
		print("%s: %s.", msg, errmsg[error - EGL_SUCCESS]);
		return error;
	}

	// Initialized EGL resources.
	void GLESDeviceContext::Init(HWND wndHandle)
	{

		// Hard-coded to 32-bit/OpenGL ES 2.0.
		// NOTE: EGL_SAMPLE_BUFFERS, EGL_SAMPLES and EGL_DEPTH_SIZE MUST remain at the beginning of the attribute list
		// since they are expected to be at indices 0-5 in config fallback code later.
		// EGL_DEPTH_SIZE is also expected to
		EGLint eglConfigAttrs[] =
		{
			EGL_SAMPLE_BUFFERS,     1,
			EGL_SAMPLES,            1,
			EGL_DEPTH_SIZE,         24,
			EGL_RED_SIZE,           8,
			EGL_GREEN_SIZE,         8,
			EGL_BLUE_SIZE,          8,
			EGL_ALPHA_SIZE,         8,
			EGL_STENCIL_SIZE,       8,
			EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
			EGL_NONE
		};
		//__multiSampling = samples > 0;

		EGLint eglConfigCount;
		const EGLint eglContextAttrs[] =
		{
			EGL_CONTEXT_CLIENT_VERSION,    2,
			EGL_NONE
		};

		const EGLint eglSurfaceAttrs[] =
		{
			EGL_RENDER_BUFFER,    EGL_BACK_BUFFER,
			EGL_NONE
		};

		if (__eglDisplay == EGL_NO_DISPLAY && __eglContext == EGL_NO_CONTEXT)
		{
			// Get the EGL display and initialize.
			__eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			if (__eglDisplay == EGL_NO_DISPLAY)
			{
				checkErrorEGL("eglGetDisplay");
				return ;
			}

			if (eglInitialize(__eglDisplay, NULL, NULL) != EGL_TRUE)
			{
				checkErrorEGL("eglInitialize");
				return ;
			}

			// Try both 24 and 16-bit depth sizes since some hardware (i.e. Tegra) does not support 24-bit depth
			bool validConfig = false;
			EGLint depthSizes[] = { 24, 16 };
			for (UINT i = 0; i < 2; ++i)
			{
				eglConfigAttrs[1] = 1;//samples > 0 ? 1 : 0;
				eglConfigAttrs[3] = 1;//samples;
				eglConfigAttrs[5] = depthSizes[i];

				if (eglChooseConfig(__eglDisplay, eglConfigAttrs, &__eglConfig, 1, &eglConfigCount) == EGL_TRUE && eglConfigCount > 0)
				{
					validConfig = true;
					break;
				}

				if (1)
				{
					// Try lowering the MSAA sample size until we find a supported config
					int sampleCount = 1/*samples*/;
					while (sampleCount)
					{
						//GP_WARN("No EGL config found for depth_size=%d and samples=%d. Trying samples=%d instead.", depthSizes[i], sampleCount, sampleCount / 2);
						sampleCount /= 2;
						eglConfigAttrs[1] = sampleCount > 0 ? 1 : 0;
						eglConfigAttrs[3] = sampleCount;
						if (eglChooseConfig(__eglDisplay, eglConfigAttrs, &__eglConfig, 1, &eglConfigCount) == EGL_TRUE && eglConfigCount > 0)
						{
							validConfig = true;
							break;
						}
					}

					//__multiSampling = sampleCount > 0;

					if (validConfig)
						break;
				}
				else
				{
					//GP_WARN("No EGL config found for depth_size=%d.", depthSizes[i]);
				}
			}

			if (!validConfig)
			{
				checkErrorEGL("eglChooseConfig");
				return ;
			}

			__eglContext = eglCreateContext(__eglDisplay, __eglConfig, EGL_NO_CONTEXT, eglContextAttrs);
			if (__eglContext == EGL_NO_CONTEXT)
			{
				checkErrorEGL("eglCreateContext");
				return ;
			}
		}

		// EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		// guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		// As soon as we picked a EGLConfig, we can safely reconfigure the
		// ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
		EGLint format;
		eglGetConfigAttrib(__eglDisplay, __eglConfig, EGL_NATIVE_VISUAL_ID, &format);
		ANativeWindow_setBuffersGeometry((ANativeWindow*)wndHandle, 0, 0, format);

		__eglSurface = eglCreateWindowSurface(__eglDisplay, __eglConfig, (ANativeWindow*)wndHandle, eglSurfaceAttrs);
		if (__eglSurface == EGL_NO_SURFACE)
		{
			checkErrorEGL("eglCreateWindowSurface");
			return ;
		}

		if (eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext) != EGL_TRUE)
		{
			checkErrorEGL("eglMakeCurrent");
			return ;
		}

		//eglQuerySurface(__eglDisplay, __eglSurface, EGL_WIDTH, &__width);
		//eglQuerySurface(__eglDisplay, __eglSurface, EGL_HEIGHT, &__height);

		//__orientationAngle = getRotation() * 90;

		// Set vsync.
		eglSwapInterval(__eglDisplay, WINDOW_VSYNC ? 1 : 0);

		// Initialize OpenGL ES extensions.
		__glExtensions = (const char*)glGetString(GL_EXTENSIONS);
		g_bHardwareETCSupported = (strstr(__glExtensions, "GL_OES_compressed_ETC1_RGB8_texture") != NULL);
		g_bHardwareATCSupported = (strstr(__glExtensions, "GL_AMD_compressed_ATC_texture") != NULL);
		g_bHardwareDDSSupported = (strstr(__glExtensions, "GL_NV_texture_compression_s3tc") != NULL);

		if (strstr(__glExtensions, "GL_OES_vertex_array_object") || strstr(__glExtensions, "GL_ARB_vertex_array_object"))
		{
			// Disable VAO extension for now.
			//glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
			//glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArrays");
			//glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
			//glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress("glIsVertexArrayOES");
		}

		char szLog[151] = "";
		int nCurLen = 0;
		int nStrLen = strlen(__glExtensions);
		while(nCurLen < nStrLen)
		{	
			strncpy(szLog, __glExtensions + nCurLen, 150);
			Log("OpenGLES Extension: %s", szLog);
			nCurLen += 150;
		}

		return ;
	}

	void GLESDeviceContext::SwapBuffers()
	{
		int rc = eglSwapBuffers(__eglDisplay, __eglSurface);
		ASSERT(rc == EGL_TRUE);
	}


}