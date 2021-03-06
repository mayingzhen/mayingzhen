#include "../Platform.h"
#include "Plugin/GLESRender/GLESBase.h"
//#include "Application/Input/AndroidInputInjector.h"
#include "Application/Input/Gesture.h"
#include "android_native_app_glue.hxx"

#include "bitset"

static std::bitset<3> __gestureEventsProcessed;

struct TouchPointerData
{
	size_t pointerId;
	bool pressed;
	double time;
	int x;
	int y;
};

TouchPointerData __pointer0;
TouchPointerData __pointer1;


namespace ma
{
	AAssetManager* __assetManager;


	static Platform*	__platformIns = NULL;
	static bool			_bRunning = false;

	static int			__orientationAngle = 90;

	static int __primaryTouchId = -1;
	static bool __multiTouch = false;

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
				pApplication->SetWindId((void*)app->window);
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
				// Note: ���յ�APP_CMD_LOST_FOCUS/APP_CMD_TERM_WINDOW��Ϣʱ,
				// �����Ѿ������߳�����,����Ⱦ�̻߳���֪��,�Ӷ�����C3�ڲ�����,
				// ���Ա�����APP_CMD_PAUSEʱ��ֹͣ��Ⱦ.
				//pApplication->StopFrame();
				_bRunning = false;
			}
			break;
		}
	}


	static int32_t onInputEvent(android_app* app, AInputEvent* event)
	{
		if (GetInput() == NULL)
			return 0;

		//AndroidInputInjector* pInputInjector = GetInput()->GetAndroidInputInjector();
		//if (pInputInjector == NULL)
		//	return 0;

		int32_t nEventType = AInputEvent_getType(event);
		if (nEventType == AINPUT_EVENT_TYPE_MOTION)
		{	
			int32_t nMotinAction = AMotionEvent_getAction(event);
			int posx = AMotionEvent_getX(event,0);
			int posy = AMotionEvent_getY(event,0);
			if (nMotinAction == AMOTION_EVENT_ACTION_DOWN)
			{

				//pInputInjector->injectTouchEvent(nMotinAction,posx,posy,0);

			}
			else if (nMotinAction == AMOTION_EVENT_ACTION_UP)
			{
				//pInputInjector->injectTouchEvent(nMotinAction,posx,posy,0);
			}
		}

		return 1;


	}



	void DebugSleep(uint32_t nTime)
	{
		LogInfo("............begine Sleep..............");

		timeval start;
		gettimeofday(&start, 0);
		while (1)
		{
			timeval end;
			gettimeofday(&end, 0);
			LogInfo("............end.tv_sec - start.tv_sec %d..............",end.tv_sec - start.tv_sec);
			if ( (end.tv_sec - start.tv_sec)  > nTime)
				break;
		}

		LogInfo("............end Sleep ..............");
	}


	Platform::Platform()
	{
		__platformIns = this;
	}


	Platform&	Platform::GetInstance()
	{
		return *__platformIns;
	}

	void		Platform::Init()
	{
		// Get the android application's activity.
		ANativeActivity* activity = get_app()->activity;
		ASSERT(activity);
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

		__assetManager = activity->assetManager; 

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

		DebugSleep(6);
			

		//Game::GetInstance().Init();

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

			if (!m_bInit)
			{
				Game::GetInstance().Init();

				m_bInit = true;
			}

			//Update();
			Game::GetInstance().Update();

			if(_bRunning)
			{
				Game::GetInstance().Render();
			}
		}
	}
}


