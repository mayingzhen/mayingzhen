#include "../Platform.h"
#include <windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ma
{

	static Keyboard::Key getKey(WPARAM win32KeyCode, bool shiftDown)
	{
		switch (win32KeyCode)
		{
		case VK_PAUSE:
			return Keyboard::KEY_PAUSE;
		case VK_SCROLL:
			return Keyboard::KEY_SCROLL_LOCK;
		case VK_PRINT:
			return Keyboard::KEY_PRINT;
		case VK_ESCAPE:
			return Keyboard::KEY_ESCAPE;
		case VK_BACK:
			return Keyboard::KEY_BACKSPACE;
		case VK_TAB:
			return shiftDown ? Keyboard::KEY_BACK_TAB : Keyboard::KEY_TAB;
		case VK_RETURN:
			return Keyboard::KEY_RETURN;
		case VK_CAPITAL:
			return Keyboard::KEY_CAPS_LOCK;
		case VK_SHIFT:
			return Keyboard::KEY_SHIFT;
		case VK_CONTROL:
			return Keyboard::KEY_CTRL;
		case VK_MENU:
			return Keyboard::KEY_ALT;
		case VK_APPS:
			return Keyboard::KEY_MENU;
		case VK_LSHIFT:
			return Keyboard::KEY_SHIFT;
		case VK_RSHIFT:
			return Keyboard::KEY_SHIFT;
		case VK_LCONTROL:
			return Keyboard::KEY_CTRL;
		case VK_RCONTROL:
			return Keyboard::KEY_CTRL;
		case VK_LMENU:
			return Keyboard::KEY_ALT;
		case VK_RMENU:
			return Keyboard::KEY_ALT;
		case VK_LWIN:
		case VK_RWIN:
			return Keyboard::KEY_HYPER;
		case VK_BROWSER_SEARCH:
			return Keyboard::KEY_SEARCH;
		case VK_INSERT:
			return Keyboard::KEY_INSERT;
		case VK_HOME:
			return Keyboard::KEY_HOME;
		case VK_PRIOR:
			return Keyboard::KEY_PG_UP;
		case VK_DELETE:
			return Keyboard::KEY_DELETE;
		case VK_END:
			return Keyboard::KEY_END;
		case VK_NEXT:
			return Keyboard::KEY_PG_DOWN;
		case VK_LEFT:
			return Keyboard::KEY_LEFT_ARROW;
		case VK_RIGHT:
			return Keyboard::KEY_RIGHT_ARROW;
		case VK_UP:
			return Keyboard::KEY_UP_ARROW;
		case VK_DOWN:
			return Keyboard::KEY_DOWN_ARROW;
		case VK_NUMLOCK:
			return Keyboard::KEY_NUM_LOCK;
		case VK_ADD:
			return Keyboard::KEY_KP_PLUS;
		case VK_SUBTRACT:
			return Keyboard::KEY_KP_MINUS;
		case VK_MULTIPLY:
			return Keyboard::KEY_KP_MULTIPLY;
		case VK_DIVIDE:
			return Keyboard::KEY_KP_DIVIDE;
		case VK_NUMPAD7:
			return Keyboard::KEY_KP_HOME;
		case VK_NUMPAD8:
			return Keyboard::KEY_KP_UP;
		case VK_NUMPAD9:
			return Keyboard::KEY_KP_PG_UP;
		case VK_NUMPAD4:
			return Keyboard::KEY_KP_LEFT;
		case VK_NUMPAD5:
			return Keyboard::KEY_KP_FIVE;
		case VK_NUMPAD6:
			return Keyboard::KEY_KP_RIGHT;
		case VK_NUMPAD1:
			return Keyboard::KEY_KP_END;
		case VK_NUMPAD2:
			return Keyboard::KEY_KP_DOWN;
		case VK_NUMPAD3:
			return Keyboard::KEY_KP_PG_DOWN;
		case VK_NUMPAD0:
			return Keyboard::KEY_KP_INSERT;
		case VK_DECIMAL:
			return Keyboard::KEY_KP_DELETE;
		case VK_F1:
			return Keyboard::KEY_F1;
		case VK_F2:
			return Keyboard::KEY_F2;
		case VK_F3:
			return Keyboard::KEY_F3;
		case VK_F4:
			return Keyboard::KEY_F4;
		case VK_F5:
			return Keyboard::KEY_F5;
		case VK_F6:
			return Keyboard::KEY_F6;
		case VK_F7:
			return Keyboard::KEY_F7;
		case VK_F8:
			return Keyboard::KEY_F8;
		case VK_F9:
			return Keyboard::KEY_F9;
		case VK_F10:
			return Keyboard::KEY_F10;
		case VK_F11:
			return Keyboard::KEY_F11;
		case VK_F12:
			return Keyboard::KEY_F12;
		case VK_SPACE:
			return Keyboard::KEY_SPACE;
		case 0x30:
			return shiftDown ? Keyboard::KEY_RIGHT_PARENTHESIS : Keyboard::KEY_ZERO;
		case 0x31:
			return shiftDown ? Keyboard::KEY_EXCLAM : Keyboard::KEY_ONE;
		case 0x32:
			return shiftDown ? Keyboard::KEY_AT : Keyboard::KEY_TWO;
		case 0x33:
			return shiftDown ? Keyboard::KEY_NUMBER : Keyboard::KEY_THREE;
		case 0x34:
			return shiftDown ? Keyboard::KEY_DOLLAR : Keyboard::KEY_FOUR;
		case 0x35:
			return shiftDown ? Keyboard::KEY_PERCENT : Keyboard::KEY_FIVE;
		case 0x36:
			return shiftDown ? Keyboard::KEY_CIRCUMFLEX : Keyboard::KEY_SIX;
		case 0x37:
			return shiftDown ? Keyboard::KEY_AMPERSAND : Keyboard::KEY_SEVEN;
		case 0x38:
			return shiftDown ? Keyboard::KEY_ASTERISK : Keyboard::KEY_EIGHT;
		case 0x39:
			return shiftDown ? Keyboard::KEY_LEFT_PARENTHESIS : Keyboard::KEY_NINE;
		case VK_OEM_PLUS:
			return shiftDown ? Keyboard::KEY_EQUAL : Keyboard::KEY_PLUS;
		case VK_OEM_COMMA:
			return shiftDown ? Keyboard::KEY_LESS_THAN : Keyboard::KEY_COMMA;
		case VK_OEM_MINUS:
			return shiftDown ? Keyboard::KEY_UNDERSCORE : Keyboard::KEY_MINUS;
		case VK_OEM_PERIOD:
			return shiftDown ? Keyboard::KEY_GREATER_THAN : Keyboard::KEY_PERIOD;
		case VK_OEM_1:
			return shiftDown ? Keyboard::KEY_COLON : Keyboard::KEY_SEMICOLON;
		case VK_OEM_2:
			return shiftDown ? Keyboard::KEY_QUESTION : Keyboard::KEY_SLASH;
		case VK_OEM_3:
			return shiftDown ? Keyboard::KEY_TILDE : Keyboard::KEY_GRAVE;
		case VK_OEM_4:
			return shiftDown ? Keyboard::KEY_LEFT_BRACE : Keyboard::KEY_LEFT_BRACKET;
		case VK_OEM_5:
			return shiftDown ? Keyboard::KEY_BAR : Keyboard::KEY_BACK_SLASH;
		case VK_OEM_6:
			return shiftDown ? Keyboard::KEY_RIGHT_BRACE : Keyboard::KEY_RIGHT_BRACKET;
		case VK_OEM_7:
			return shiftDown ? Keyboard::KEY_QUOTE : Keyboard::KEY_APOSTROPHE;
		case 0x41:
			return shiftDown ? Keyboard::KEY_CAPITAL_A : Keyboard::KEY_A;
		case 0x42:
			return shiftDown ? Keyboard::KEY_CAPITAL_B : Keyboard::KEY_B;
		case 0x43:
			return shiftDown ? Keyboard::KEY_CAPITAL_C : Keyboard::KEY_C;
		case 0x44:
			return shiftDown ? Keyboard::KEY_CAPITAL_D : Keyboard::KEY_D;
		case 0x45:
			return shiftDown ? Keyboard::KEY_CAPITAL_E : Keyboard::KEY_E;
		case 0x46:
			return shiftDown ? Keyboard::KEY_CAPITAL_F : Keyboard::KEY_F;
		case 0x47:
			return shiftDown ? Keyboard::KEY_CAPITAL_G : Keyboard::KEY_G;
		case 0x48:
			return shiftDown ? Keyboard::KEY_CAPITAL_H : Keyboard::KEY_H;
		case 0x49:
			return shiftDown ? Keyboard::KEY_CAPITAL_I : Keyboard::KEY_I;
		case 0x4A:
			return shiftDown ? Keyboard::KEY_CAPITAL_J : Keyboard::KEY_J;
		case 0x4B:
			return shiftDown ? Keyboard::KEY_CAPITAL_K : Keyboard::KEY_K;
		case 0x4C:
			return shiftDown ? Keyboard::KEY_CAPITAL_L : Keyboard::KEY_L;
		case 0x4D:
			return shiftDown ? Keyboard::KEY_CAPITAL_M : Keyboard::KEY_M;
		case 0x4E:
			return shiftDown ? Keyboard::KEY_CAPITAL_N : Keyboard::KEY_N;
		case 0x4F:
			return shiftDown ? Keyboard::KEY_CAPITAL_O : Keyboard::KEY_O;
		case 0x50:
			return shiftDown ? Keyboard::KEY_CAPITAL_P : Keyboard::KEY_P;
		case 0x51:
			return shiftDown ? Keyboard::KEY_CAPITAL_Q : Keyboard::KEY_Q;
		case 0x52:
			return shiftDown ? Keyboard::KEY_CAPITAL_R : Keyboard::KEY_R;
		case 0x53:
			return shiftDown ? Keyboard::KEY_CAPITAL_S : Keyboard::KEY_S;
		case 0x54:
			return shiftDown ? Keyboard::KEY_CAPITAL_T : Keyboard::KEY_T;
		case 0x55:
			return shiftDown ? Keyboard::KEY_CAPITAL_U : Keyboard::KEY_U;
		case 0x56:
			return shiftDown ? Keyboard::KEY_CAPITAL_V : Keyboard::KEY_V;
		case 0x57:
			return shiftDown ? Keyboard::KEY_CAPITAL_W : Keyboard::KEY_W;
		case 0x58:
			return shiftDown ? Keyboard::KEY_CAPITAL_X : Keyboard::KEY_X;
		case 0x59:
			return shiftDown ? Keyboard::KEY_CAPITAL_Y : Keyboard::KEY_Y;
		case 0x5A:
			return shiftDown ? Keyboard::KEY_CAPITAL_Z : Keyboard::KEY_Z;
		default:
			return Keyboard::KEY_NONE;
		}
	}

	LRESULT CALLBACK WndProc(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
			return true;

		if (ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureMouse)
			return true;

		Platform* pCurApp = reinterpret_cast<Platform*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
		
		static bool shiftDown = false;
		static bool capsOn = false;

		switch (message)
		{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_DISPLAYCHANGE:
		case WM_SIZE:
			{
				if(wParam != SIZE_MINIMIZED)
				{
					RECT rect;
					GetClientRect(hwnd, &rect);
					uint32_t nWidth = rect.right - rect.left;
					uint32_t nHeight = rect.bottom - rect.top;
					if (Game::GetInstance().m_OnWindowSizedWidth != nWidth || 
						Game::GetInstance().m_OnWindowSizedHeight != nHeight)
					{
						Game::GetInstance().m_OnWindowSizedWidth = nWidth;
						Game::GetInstance().m_OnWindowSizedHeight = nHeight;
						Game::GetInstance().m_bOnWindowSized = true;
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			Game::GetInstance().mMouseEvent.trigger(Mouse::MOUSE_PRESS_LEFT_BUTTON, LOWORD(lParam), HIWORD(lParam),0);
			break;
		case WM_LBUTTONUP:
			Game::GetInstance().mMouseEvent.trigger(Mouse::MOUSE_RELEASE_LEFT_BUTTON, LOWORD(lParam), HIWORD(lParam),0);
			break;
		case WM_RBUTTONDOWN:
			Game::GetInstance().mMouseEvent.trigger(Mouse::MOUSE_PRESS_RIGHT_BUTTON, LOWORD(lParam), HIWORD(lParam),0);
			break;
		case WM_RBUTTONUP:
			Game::GetInstance().mMouseEvent.trigger(Mouse::MOUSE_RELEASE_RIGHT_BUTTON, LOWORD(lParam), HIWORD(lParam),0);
			break;
		case WM_MBUTTONDOWN:
			Game::GetInstance().mMouseEvent.trigger(Mouse::MOUSE_PRESS_MIDDLE_BUTTON, LOWORD(lParam), HIWORD(lParam),0);
			break;
		case WM_MBUTTONUP:
			Game::GetInstance().mMouseEvent.trigger(Mouse::MOUSE_RELEASE_MIDDLE_BUTTON, LOWORD(lParam), HIWORD(lParam),0);
			break;
		case WM_MOUSEMOVE:
			Game::GetInstance().mMouseEvent.trigger(Mouse::MOUSE_MOVE, LOWORD(lParam), HIWORD(lParam), 0);
			break;
		case WM_MOUSEWHEEL:
			tagPOINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			ScreenToClient(hwnd, &point);
			Game::GetInstance().mMouseEvent.trigger(Mouse::MOUSE_WHEEL,point.x, point.y, GET_WHEEL_DELTA_WPARAM(wParam) / 120);
			break;

		case WM_KEYDOWN:
			if (wParam == VK_SHIFT || wParam == VK_LSHIFT || wParam == VK_RSHIFT)
				shiftDown = true;

			if (wParam == VK_CAPITAL)
				capsOn = !capsOn;

			// Suppress key repeats.
			if ((lParam & 0x40000000) == 0)
				Game::GetInstance().mkeyEvent.trigger(Keyboard::KEY_PRESS, getKey(wParam, shiftDown ^ capsOn));
			break;

		case WM_KEYUP:
			if (wParam == VK_SHIFT || wParam == VK_LSHIFT || wParam == VK_RSHIFT)
				shiftDown = false;

			Game::GetInstance().mkeyEvent.trigger(Keyboard::KEY_RELEASE, getKey(wParam, shiftDown ^ capsOn));
			break;

		case WM_CHAR:
			// Suppress key repeats.
			if ((lParam & 0x40000000) == 0)
				Game::GetInstance().mkeyEvent.trigger(Keyboard::KEY_CHAR, getKey(wParam, shiftDown ^ capsOn));
			break;

		case WM_UNICHAR:
			// Suppress key repeats.
			if ((lParam & 0x40000000) == 0)
				Game::GetInstance().mkeyEvent.trigger(Keyboard::KEY_CHAR, getKey(wParam, shiftDown ^ capsOn));
			break;

		case WM_SETFOCUS:
			break;

		case WM_KILLFOCUS:
			break;
		case WM_INPUTLANGCHANGE:
		case WM_IME_SETCONTEXT:
		case WM_IME_STARTCOMPOSITION:
		case WM_IME_COMPOSITION:
		case WM_IME_NOTIFY:
		case WM_IME_ENDCOMPOSITION:
			//if(Platform::imeEventInternal(message, wParam, lParam))
			//	return S_OK;
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam); 

		}
		return 0;
	}

	static Platform* gplatform = NULL;

	Platform&	Platform::GetInstance()
	{
		return *gplatform;
	}

	Platform::Platform()
	{
		gplatform = this;

		m_bInit = false;
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
			MessageBox(NULL, "This program requires Windows NT!", szAppName, MB_ICONERROR) ;
			return;
		}

		uint32_t dwWindowStyle = WS_OVERLAPPEDWINDOW;
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

		SetWindowLongPtr((HWND)m_windId, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ShowWindow((HWND)m_windId, SW_SHOW) ;

		SetWindowText((HWND)m_windId,Game::GetInstance().GetGameName());
	}

	void Platform::Shutdown()
	{
	}

	void Platform::GetWindowSize(int& w, int& h) const
	{
		RECT rect; 
		::GetClientRect((HWND)m_windId,&rect);
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
	}

	void Platform::SetWindowSize(int w,int h)
	{
		RECT rect;
		int ioldW,ioldH,inewW,inewH;
		GetWindowRect((HWND)m_windId,&rect);
		inewW = rect.right - rect.left;
		inewH = rect.bottom - rect.top;
		
		GetWindowSize(ioldW,ioldH);

		inewW += (w - ioldW);
		inewH += (h - ioldH);

		::MoveWindow((HWND)m_windId,rect.left,rect.top,inewW,inewH,true);
	}

	void Platform::Run()
	{
		bool gotMsg;
		MSG msg;

		::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
		
		while(WM_QUIT != msg.message)
		{
			gotMsg = ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ? true : false;

			if (gotMsg)
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				if (!m_bInit)
				{
					Game::GetInstance().Init();

					ImGui_ImplWin32_Init(m_windId);

					m_bInit = true;
				}

				ImGui_ImplWin32_NewFrame();

				Game::GetInstance().Update();
				
				Game::GetInstance().Render();
			}
		}
	}
}


