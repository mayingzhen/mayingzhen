#include "ApplicationAndroid.h"



namespace ma
{

	void Platform::MsgProc(android_app* app, int cmd)
	{
		Platform* pApplication = static_cast<Platform*>(app->userData);
		switch (cmd)
		{
		case APP_CMD_INIT_WINDOW:
			{
				//pApplication->OnInitWindow(app->window);
				pApplication->m_windId = (HWND)app->window;
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
				pApplication->StartFrame();
			}
			break;

		case APP_CMD_PAUSE:
			{	
				// Note: 当收到APP_CMD_LOST_FOCUS/APP_CMD_TERM_WINDOW消息时,
				// 窗体已经被主线程销毁,而渲染线程还不知道,从而引起C3内部崩溃,
				// 所以必须在APP_CMD_PAUSE时就停止渲染.
				pApplication->StopFrame();
			}
			break;
		}
	}


	int32_t Platform::InputProc(android_app* app, AInputEvent* event)
	{
		if (app == NULL || event == NULL)
			return 0;

		Input* pInput = GetInput(); 
		if (pInput == NULL)
			return 0;

		Platform* pApplication = static_cast<Platform*>(app->userData);
		int32_t nEventType = AInputEvent_getType(event);
		if (nEventType == AINPUT_EVENT_TYPE_MOTION)
		{	
			int32_t nMotinAction = AMotionEvent_getAction(event);
			int posx = AMotionEvent_getX(event,0);
			int posy = AMotionEvent_getY(event,0);
			if (nMotinAction == AMOTION_EVENT_ACTION_DOWN)
			{
				OIS::MouseState ms = * pInput->GetMouseState();
				ms.X.rel = posx - ms.X.abs;
				ms.X.abs = posx;
				ms.Y.rel = posy - ms.Y.abs;
				ms.Y.abs = posy;
				OIS::MouseButtonID iBtn = OIS::MB_Left;
			
				ms.buttons |= (1L << iBtn);
				OIS::MouseEvent arg(NULL,ms);
				pInput->InjectMousePressed(arg,iBtn);
			
			}
			else if (nMotinAction == AMOTION_EVENT_ACTION_UP)
			{
				OIS::MouseState ms = * pInput->GetMouseState();
				ms.X.rel = posx - ms.X.abs;
				ms.X.abs = posx;
				ms.Y.rel = posy - ms.Y.abs;
				ms.Y.abs = posy;
				OIS::MouseButtonID iBtn = OIS::MB_Left;

				ms.buttons |= (1L << iBtn);
				OIS::MouseEvent arg(NULL,ms);
				pInput->InjectMouseReleased(arg,iBtn);
			}
		}

		return 0;
	}

	Platform::Platform(const char* appID):ApplicationBase(appID)
	{
		m_bRunning = false;
	}

	void		Platform::Init()
	{
		//uSleep(2000); For GDB BUUG
 
		android_app* pAppState = get_app();
		pAppState->userData = this;
		pAppState->onAppCmd = MsgProc;
		pAppState->onInputEvent = InputProc;	

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
	}

	void		Platform::GetWindowSize(int& w, int& h)
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
			while((nRet = ALooper_pollAll(m_bRunning? 0 : -1, NULL, &nEventCount, (void**)&pPollSource)) >= 0) 
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

			Update();

			if( ProcessFrame() )
			{
				Render();
			}
		}
	}
}


