#include "Application/ApplicationWin.h"

namespace ma
{
	ApplicationWin* g_pAppliction = NULL;

	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		HDC         hdc ;
		PAINTSTRUCT ps ;
		RECT        rect ;

		if (g_pAppliction && hwnd == g_pAppliction->GetMainHWND())
		{
			switch (message)
			{
			case WM_SIZE:
				if (SIZE_MINIMIZED == wParam)
				{
				}
				else 
				{
					RECT rcCurrentClient;
					GetClientRect(hwnd,&rcCurrentClient);

					if( rcCurrentClient.top == 0 && rcCurrentClient.bottom == 0 )
					{
						// Rapidly clicking the task bar to minimize and restore a window
						// can cause a WM_SIZE message with SIZE_RESTORED when 
						// the window has actually become minimized due to rapid change
						// so just ignore this message
					}
					else
					{
						if (NULL != g_pAppliction)
						{
							g_pAppliction->OnResize(rcCurrentClient.right-rcCurrentClient.left,rcCurrentClient.bottom - rcCurrentClient.top);
						}
					}
				}
				break;
			case WM_PAINT:
				//g_pAppliction->Render();
				return 0 ;

			case WM_DESTROY:
				PostQuitMessage (0) ;
				return 0 ;
			}
		}

		return DefWindowProc (hwnd, message, wParam, lParam) ;
	}

	ApplicationWin::ApplicationWin(const char* appID):Application(appID)
	{
		g_pAppliction = this;
	}

	int	ApplicationWin::CreateMainWnd(int argc, char* argv[])
	{
		HINSTANCE hInstance = GetModuleHandleA(NULL);

		char szAppName[] = "ApplicationWin" ;
		MSG          msg ;
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

		if (!RegisterClassA(&wndclass))
		{
			MessageBoxA (NULL, "This program requires Windows NT!", 
				szAppName, MB_ICONERROR) ;
			return 0 ;
		}
		m_hMainWnd = CreateWindowA(szAppName,                  // window class name
			"ApplicationWin",			// window caption
			WS_OVERLAPPEDWINDOW,        // window style
			CW_USEDEFAULT,              // initial x position
			CW_USEDEFAULT,              // initial y position
			CW_USEDEFAULT,              // initial x size
			CW_USEDEFAULT,              // initial y size
			NULL,                       // parent window handle
			NULL,                       // window menu handle
			hInstance,                  // program instance handle
			NULL) ;                     // creation parameters

		ShowWindow (m_hMainWnd, SW_SHOW) ;

		SetWindowTextA(m_hMainWnd,m_strAppID.c_str());

		return (int)m_hMainWnd;
	}

	void ApplicationWin::GetWindowSize(int& w, int& h)
	{
		RECT rect; 
		GetClientRect(m_hMainWnd,&rect);
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
	}

	void ApplicationWin::Run()
	{
		MSG msg;

		while (GetMessage (&msg, NULL, 0, 0))
		{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;

			Sleep(10);

			Update();

			Render();
		}
	}
}

