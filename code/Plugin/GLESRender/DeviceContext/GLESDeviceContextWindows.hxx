#include "GLESDeviceContextWindows.h"

namespace ma
{
	void GLESDeviceContext::Init(HWND wndhandle)
	{
		EGLint numConfigs;
		EGLint majorVersion;
		EGLint minorVersion;
		EGLDisplay display;
		EGLContext context;
		EGLSurface surface;
		EGLConfig config;

		int nSamples = 0;
		EGLint attribList[] =
		{
			EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
			EGL_RED_SIZE,           5,
			EGL_GREEN_SIZE,         6,
			EGL_BLUE_SIZE,          5,
			EGL_ALPHA_SIZE,         EGL_DONT_CARE,
			EGL_DEPTH_SIZE,         16,
			EGL_STENCIL_SIZE,       8,
			EGL_SAMPLE_BUFFERS,     nSamples > 0 ? 1 : 0,
			EGL_SAMPLES,            nSamples,
			EGL_NONE
		};

		// Get Display
		display = eglGetDisplay(GetDC(wndhandle));
		if (display == EGL_NO_DISPLAY)
		{
			ASSERT(false);
			return ;
		}

		// Initialize EGL
		if (!eglInitialize(display, &majorVersion, &minorVersion))
		{
			ASSERT(false);
			return ;
		}

		// Get configs
		if (!eglGetConfigs(display, NULL, 0, &numConfigs))
		{
			ASSERT(false);
			return ;
		}

		// Choose config
		if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs))
		{
			ASSERT(false);
			return ;
		}

		EGLint nDepth = 0, nStencil = 0, nSampleBuffers;
		if (!eglGetConfigAttrib(display, config, EGL_DEPTH_SIZE, &nDepth))
		{
			ASSERT(false);
			return ;
		}
		if (!eglGetConfigAttrib(display, config, EGL_STENCIL_SIZE, &nStencil))
		{
			ASSERT(false);
			return ;
		}
		if (!eglGetConfigAttrib(display, config, EGL_SAMPLE_BUFFERS, &nSampleBuffers))
		{
			ASSERT(false);
			return ;
		}
		LogInfo("EGL Depth:%d, Stencil:%d, SampleBuffers:%d", nDepth, nStencil, nSampleBuffers);

		// Create a surface
		surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)wndhandle, NULL);
		if (surface == EGL_NO_SURFACE)
		{
			EGLint err = eglGetError();
			switch (err)
			{
			case EGL_BAD_MATCH:
				LogError("Check window and EGLConfig attributes to determine compatibility, or verify that the EGLConfig supports rendering to a window");
				break;
			case EGL_BAD_CONFIG:
				LogError("Verify that provided EGLConfig is valid");
				break;
			case EGL_BAD_NATIVE_WINDOW:
				LogError("Verify that provided EGLNativeWindow is valid");
				break;
			case EGL_BAD_ALLOC:
				LogError("Not enough resources available. Handle and recover");
				break;
			default:
				LogError("Failed to eglCreateWindowSurface:%d", err);
				break;
			}
			return ;
		}

		// Create a GL context
		EGLint contextAttribs[] =
		{
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};
		context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
		if (context == EGL_NO_CONTEXT)
		{
			ASSERT(false);
			return;
		}

		// Make the context current
		if (!eglMakeCurrent(display, surface, surface, context))
		{
			ASSERT(false);
			return ;
		}

		m_eglDisplay = display;
		m_eglSurface = surface;
		m_eglContext = context;

		//LogInfo("Init DefaultFBO:%d, HWND:0x%x", mFBO, m_hWnd);
		return ;
	}

	void GLESDeviceContext::SwapBuffers()
	{
		GL_ASSERT( eglSwapBuffers(m_eglDisplay, m_eglSurface) );
	}
}



