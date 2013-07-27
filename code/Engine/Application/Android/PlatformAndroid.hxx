#include "../Platform.h"
#include "Plugin/GLESRender/GLESBase.h"

#include "android_native_app_glue.hxx"




namespace ma
{
	bool g_bHardwareETCSupported = false;
	bool g_bHardwareATCSupported = false;
	bool g_bHardwareDDSSupported = false;

	static Platform*	__platformIns = NULL;
	static bool			_bRunning = false;
	static EGLDisplay	__eglDisplay = EGL_NO_DISPLAY;
	static EGLContext	__eglContext = EGL_NO_CONTEXT;
	static EGLSurface	__eglSurface = EGL_NO_SURFACE;
	static EGLConfig	__eglConfig = 0;
	static int			__orientationAngle = 90;
	// OpenGL VAO functions.
	static const char* __glExtensions;
	PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = NULL;
	PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = NULL;
	PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = NULL;
	PFNGLISVERTEXARRAYOESPROC glIsVertexArray = NULL;

	#define GESTURE_TAP_DURATION_MAX    200
	#define GESTURE_SWIPE_DURATION_MAX  400
	#define GESTURE_SWIPE_DISTANCE_MIN  50

	void onAppCmd(android_app* app, int cmd)
	{
		Platform* pApplication = __platformIns;
		switch (cmd)
		{
		case APP_CMD_INIT_WINDOW:
			{
				pApplication->SetWindId((HWND)app->window);
			}
			break;

		case APP_CMD_WINDOW_RESIZED:
			{
			}
			break;

		case APP_CMD_TERM_WINDOW:
			pApplication->Shutdown();
			break;

		case APP_CMD_GAINED_FOCUS:
			{
				_bRunning = true;
				//pApplication->StartFrame();
			}
			break;

		case APP_CMD_PAUSE:
			{	
				// Note: 当收到APP_CMD_LOST_FOCUS/APP_CMD_TERM_WINDOW消息时,
				// 窗体已经被主线程销毁,而渲染线程还不知道,从而引起C3内部崩溃,
				// 所以必须在APP_CMD_PAUSE时就停止渲染.
				//pApplication->StopFrame();
				_bRunning = false;
			}
			break;
		}
	}


	int32_t onInputEvent(android_app* app, AInputEvent* event)
	{

		// 		if (mInputInjector)
		// 		{
		// 			if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
		// 			{
		// 				int action = (int)(AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction(event));
		// 
		// 				if(action == 0)
		// 					mInputInjector->injectTouchEvent(2, AMotionEvent_getRawX(event, 0), AMotionEvent_getRawY(event, 0) );
		// 
		// 				mInputInjector->injectTouchEvent(action, AMotionEvent_getRawX(event, 0), AMotionEvent_getRawY(event, 0) );
		// 			}
		// 			else 
		// 			{
		// 				mInputInjector->injectKeyEvent(AKeyEvent_getAction(event), AKeyEvent_getKeyCode(event));
		// 			}
		// 
		// 			return 1;
		// 		}
		// 		return 0;


		// 		if (app == NULL || event == NULL)
		// 			return 0;
		// 
		// 		Input* pInput = GetInput(); 
		// 		if (pInput == NULL)
		// 			return 0;
		// 
		// 		Platform* pApplication = static_cast<Platform*>(app->userData);
		// 		int32_t nEventType = AInputEvent_getType(event);
		// 		if (nEventType == AINPUT_EVENT_TYPE_MOTION)
		// 		{	
		// 			int32_t nMotinAction = AMotionEvent_getAction(event);
		// 			int posx = AMotionEvent_getX(event,0);
		// 			int posy = AMotionEvent_getY(event,0);
		// 			if (nMotinAction == AMOTION_EVENT_ACTION_DOWN)
		// 			{
		// 				OIS::MouseState ms = * pInput->GetMouseState();
		// 				ms.X.rel = posx - ms.X.abs;
		// 				ms.X.abs = posx;
		// 				ms.Y.rel = posy - ms.Y.abs;
		// 				ms.Y.abs = posy;
		// 				OIS::MouseButtonID iBtn = OIS::MB_Left;
		// 			
		// 				ms.buttons |= (1L << iBtn);
		// 				OIS::MouseEvent arg(NULL,ms);
		// 				pInput->InjectMousePressed(arg,iBtn);
		// 			
		// 			}
		// 			else if (nMotinAction == AMOTION_EVENT_ACTION_UP)
		// 			{
		// 				OIS::MouseState ms = * pInput->GetMouseState();
		// 				ms.X.rel = posx - ms.X.abs;
		// 				ms.X.abs = posx;
		// 				ms.Y.rel = posy - ms.Y.abs;
		// 				ms.Y.abs = posy;
		// 				OIS::MouseButtonID iBtn = OIS::MB_Left;
		// 
		// 				ms.buttons |= (1L << iBtn);
		// 				OIS::MouseEvent arg(NULL,ms);
		// 				pInput->InjectMouseReleased(arg,iBtn);
		// 			}
		// 		}
		// 
		// 		return 0;
	}

	extern void print(const char* format, ...)
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
	static bool initEGL()
	{
		//int samples = 0;
		//Properties* config = Game::getInstance()->getConfig()->getNamespace("window", true);
		//if (config)
		//{
		//	samples = std::max(config->getInt("samples"), 0);
		//}

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
				return false;
			}

			if (eglInitialize(__eglDisplay, NULL, NULL) != EGL_TRUE)
			{
				checkErrorEGL("eglInitialize");
				return false;
			}

			// Try both 24 and 16-bit depth sizes since some hardware (i.e. Tegra) does not support 24-bit depth
			bool validConfig = false;
			EGLint depthSizes[] = { 24, 16 };
			for (unsigned int i = 0; i < 2; ++i)
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
				return false;
			}

			__eglContext = eglCreateContext(__eglDisplay, __eglConfig, EGL_NO_CONTEXT, eglContextAttrs);
			if (__eglContext == EGL_NO_CONTEXT)
			{
				checkErrorEGL("eglCreateContext");
				return false;
			}
		}

		// EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		// guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		// As soon as we picked a EGLConfig, we can safely reconfigure the
		// ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
		EGLint format;
		eglGetConfigAttrib(__eglDisplay, __eglConfig, EGL_NATIVE_VISUAL_ID, &format);
		ANativeWindow_setBuffersGeometry(get_app()->window, 0, 0, format);

		__eglSurface = eglCreateWindowSurface(__eglDisplay, __eglConfig, get_app()->window, eglSurfaceAttrs);
		if (__eglSurface == EGL_NO_SURFACE)
		{
			checkErrorEGL("eglCreateWindowSurface");
			return false;
		}

		if (eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext) != EGL_TRUE)
		{
			checkErrorEGL("eglMakeCurrent");
			return false;
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
			glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
			glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArrays");
			glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
			glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress("glIsVertexArrayOES");
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

		return true;
	}

	static void destroyEGLSurface()
	{
		if (__eglDisplay != EGL_NO_DISPLAY)
		{
			eglMakeCurrent(__eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		}

		if (__eglSurface != EGL_NO_SURFACE)
		{
			eglDestroySurface(__eglDisplay, __eglSurface);
			__eglSurface = EGL_NO_SURFACE;
		}
	}

	static void destroyEGLMain()
	{
		destroyEGLSurface();

		if (__eglContext != EGL_NO_CONTEXT)
		{
			eglDestroyContext(__eglDisplay, __eglContext);
			__eglContext = EGL_NO_CONTEXT;
		}

		if (__eglDisplay != EGL_NO_DISPLAY)
		{
			eglTerminate(__eglDisplay);
			__eglDisplay = EGL_NO_DISPLAY;
		}
	}

	void Sleep(unsigned nTime)
	{
		Log("............begine Sleep..............");

		timeval start;
		gettimeofday(&start, 0);
		while (1)
		{
			timeval end;
			gettimeofday(&end, 0);
			Log("............end.tv_sec - start.tv_sec %d..............",end.tv_sec - start.tv_sec);
			if ( (end.tv_sec - start.tv_sec)  > nTime)
				break;
		}

		Log("............end Sleep ..............");
	}


	Platform::Platform()
	{
		__platformIns = this;
		//m_bRunning = false;
	}

	void		Platform::swapBuffers()
	{

	}

	void		Platform::sleep(long ms)
	{

	}

	Platform&	Platform::GetInstance()
	{
		return *__platformIns;
	}

	void		Platform::Init()
	{
		// Get the android application's activity.
		ANativeActivity* activity = get_app()->activity;
		JavaVM* jvm = activity->vm;
		JNIEnv* env = NULL;
		jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
		jint res = jvm->AttachCurrentThread(&env, NULL);
		if (res == JNI_ERR)
		{
			ASSERT("Failed to retrieve JVM environment when entering message pump.");
			return; 
		}
		ASSERT(env);

		android_app* pAppState = get_app();
		pAppState->userData = this;
		pAppState->onAppCmd = onAppCmd;
		pAppState->onInputEvent = onInputEvent;	

		m_windId = NULL;

		while (NULL == m_windId)
		{
			// Read all pending events.
			int ident;
			int events;
			android_poll_source* source;

			do
			{
				ident = ALooper_pollAll(0, NULL, &events, reinterpret_cast<void**>(&source));

				// Process this event.
				if (source != NULL)
				{
					source->process(pAppState, source);
				}

				// Check if we are exiting.
				if (pAppState->destroyRequested != 0)
				{
					return;
				}
			} while ((NULL == m_windId) && (ident >= 0));
		}

		Sleep(4);

		initEGL();

		Game::GetInstance().Init();

	}

	void Platform::Shutdown()
	{
		Game::GetInstance().Shutdown();
	}

	void Platform::GetWindowSize(int& w, int& h) const
	{
		if (m_windId == NULL)
		{
			w = 0;
			h = 0;
		}
		else
		{
			w = ANativeWindow_getWidth((ANativeWindow*)m_windId);
			h = ANativeWindow_getHeight((ANativeWindow*)m_windId);
		}
	}

	void		Platform::Run()
	{
		while(true) 
		{
			android_app* pAppState = get_app();

			// Read all pending events.
			int nRet = 0;
			int nEventCount = 0;
			android_poll_source* pPollSource = NULL;
			while((nRet = ALooper_pollAll(_bRunning? 0 : -1, NULL, &nEventCount, (void**)&pPollSource)) >= 0) 
			{
				// Process this event.
				if(pPollSource != NULL) 
				{
					pPollSource->process(pAppState, pPollSource);
				}

				// Check if we are exiting.
				if(pAppState->destroyRequested != 0)
				{
					return;
				}
			}

			//Update();
			Game::GetInstance().Update();

			if( _bRunning)
			{
				Game::GetInstance().Render();
			}

			int rc = eglSwapBuffers(__eglDisplay, __eglSurface);
			ASSERT(rc == EGL_TRUE);

		}
	}


}


