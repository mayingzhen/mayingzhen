#include "../Platform.h"
#include <windowsx.h>
#include <shellapi.h>



namespace ma
{

	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Platform* pCurApp = reinterpret_cast<Platform*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));

		static bool shiftDown = false;
		static bool capsOn = false;

		switch (message)
		{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;

		case WM_DESTROY:
			//ApplicationBase::shutdownInternal();
			Game::GetInstance().Shutdown();
			PostQuitMessage(0);
			return 0;

		case WM_SIZE:
			// Window was resized.
			//Game::GetInstance().resizeEvent((UINT)(short)LOWORD(lParam), (UINT)(short)HIWORD(lParam));
			break;
		}

		return DefWindowProc(hwnd, message, wParam, lParam); 
	}

	static Platform* gplatform = NULL;

	Platform&	Platform::GetInstance()
	{
		return *gplatform;
	}

	Platform::Platform()
	{
		gplatform = this;
	}

	void Platform::Init()
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);

		char szAppName[] = "ApplicationWin" ;
		WNDCLASSA    wndclass ;

		wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
		wndclass.lpfnWndProc   = WndProc ;
		wndclass.cbClsExtra    = 0 ;
		wndclass.cbWndExtra    = 0 ;
		wndclass.hInstance     = GetModuleHandle(NULL);
		wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
		wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
		wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
		wndclass.lpszMenuName  = NULL ;
		wndclass.lpszClassName = szAppName ;

		if (!RegisterClass(&wndclass))
		{
			MessageBox(NULL, "This program requires Windows NT!", 
				szAppName, MB_ICONERROR) ;
			return;
		}

		DWORD dwWindowStyle = WS_OVERLAPPEDWINDOW;
		RECT rc;
		SetRect( &rc, 0, 0, 800, 600 );        
		AdjustWindowRect( &rc, dwWindowStyle, false );

		m_windId = CreateWindow(szAppName,                  // window class name
			"ApplicationWin",			// window caption
			WS_OVERLAPPEDWINDOW,        // window style
			CW_USEDEFAULT,              // initial x position
			CW_USEDEFAULT,              // initial y position
			rc.right-rc.left, /*CW_USEDEFAULT*/       // initial x size
			rc.bottom-rc.top, /*CW_USEDEFAULT*/       // initial y size
			NULL,                       // parent window handle
			NULL,                       // window menu handle
			hInstance,                  // program instance handle
			NULL) ;                     // creation parameters

		SetWindowLongPtr(m_windId, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ShowWindow (m_windId, SW_SHOW) ;

		SetWindowText(m_windId,Game::GetInstance().GetGameName());

		Game::GetInstance().Init();
	}

	void Platform::Shutdown()
	{

	}

	void Platform::GetWindowSize(int& w, int& h) const
	{
		RECT rect; 
		::GetClientRect(m_windId,&rect);
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
	}

	void Platform::SetWindowSize(int w,int h)
	{
		RECT rect;
		int ioldW,ioldH,inewW,inewH;
		GetWindowRect(m_windId,&rect);
		inewW = rect.right - rect.left;
		inewH = rect.bottom - rect.top;
		
		GetWindowSize(ioldW,ioldH);

		inewW += (w - ioldW);
		inewH += (h - ioldH);

		::MoveWindow(m_windId,rect.left,rect.top,inewW,inewH,true);
	}

	void Platform::Run()
	{
		bool gotMsg;
		MSG msg;

		::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
		
		while(WM_QUIT != msg.message)
		{
			//if (GetActiveWindow() == m_windId)
			//{
				gotMsg = ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ? true : false;
			//}
			//else
			//{
			//	gotMsg = ::GetMessage(&msg, NULL, 0, 0) ? true : false;
			//}
			
			if (gotMsg)
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				//Update();
				Game::GetInstance().Update();
				
				//if (GetActiveWindow() == m_windId)
				//{
					Game::GetInstance().Render();
				//}

				output_all_code_time();
			}
		}
	}
}


