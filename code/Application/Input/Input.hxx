#include "Input.h"

namespace ma
{

	Input::Input()
	{
		memset(&m_curIS,0,sizeof(InputState));
		memset(&m_preIS,0,sizeof(InputState));
	
		Game::GetInstance().mkeyEvent.notify(this,&Input::keyEvent);
		Game::GetInstance().mMouseEvent.notify(this,&Input::mouseEvent);
	}


	Input::~Input()
	{
		Game::GetInstance().mkeyEvent.remove(this,&Input::keyEvent);
		Game::GetInstance().mMouseEvent.remove(this,&Input::mouseEvent);
	}

	void Input::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (Keyboard::KEY_PRESS == evt)
		{
			InjectKeyPressed(key);
		}
		else if (Keyboard::KEY_RELEASE == evt)
		{
			InjectKeyReleased(key);
		}
	}

	void Input::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
	{
		MouseState ms = this->GetMouseState();
		ms.X.rel = x - ms.X.abs;
		ms.X.abs = x;
		ms.Y.rel = y - ms.Y.abs;
		ms.Y.abs = y;
		ms.Z.rel = wheelDelta;

		if (evt == Mouse::MOUSE_MOVE)
		{
		}
		else if (evt == Mouse::MOUSE_WHEEL)
		{
			//ms.Z.rel = wheelDelta;
		}
		else if (evt == Mouse::MOUSE_PRESS_LEFT_BUTTON)
		{
			ms.buttons |= (1L << MB_Left);
		}
		else if (evt == Mouse::MOUSE_RELEASE_LEFT_BUTTON)
		{
			ms.buttons &= ~(1L << MB_Left);
		}
		else if (evt == Mouse::MOUSE_PRESS_MIDDLE_BUTTON)
		{
			ms.buttons |= (1L << MB_Middle);
		}
		else if (evt == Mouse::MOUSE_RELEASE_MIDDLE_BUTTON)
		{
			ms.buttons &= ~(1L << MB_Middle);
		}
		else if (evt == Mouse::MOUSE_PRESS_RIGHT_BUTTON)
		{
			ms.buttons |= (1L << MB_Right);
		}
		else if (evt == Mouse::MOUSE_RELEASE_RIGHT_BUTTON)
		{
			ms.buttons &= ~(1L << MB_Right);
		}

		InjectMouseState(ms);
	}

	void Input::InjectLoseFocus()
	{
		memset(m_curIS.m_keyState,0,sizeof(m_curIS.m_keyState));
		m_curIS.m_mouseState.clear();
	}

	void Input::InjectInputBegin()
	{
		m_preIS = m_curIS;

		m_curIS.m_mouseState.X.rel = 0;
		m_curIS.m_mouseState.Y.rel = 0;
		m_curIS.m_mouseState.Z.rel = 0;
	}

	void Input::InjectInputEnd()
	{
		m_curIS.m_mouseState.X.rel = m_curIS.m_mouseState.X.abs - m_preIS.m_mouseState.X.abs;
		m_curIS.m_mouseState.Y.rel = m_curIS.m_mouseState.Y.abs - m_preIS.m_mouseState.Y.abs;
		//m_curIS.m_mouseState.Z.rel = m_curIS.m_mouseState.Z.abs - m_preIS.m_mouseState.Z.abs;
	}

	void Input::InjectKeyPressed(Keyboard::Key keyCode)
	{
		m_curIS.m_keyState[keyCode] = 1;
	}

	void Input::InjectKeyReleased(Keyboard::Key keyCode)
	{
		m_curIS.m_keyState[keyCode] = 0;
	}

	void Input::InjectMouseState(const MouseState& state)
	{
		m_curIS.m_mouseState = state;
	}

	bool Input::IsKeyDown(Keyboard::Key keyCode) const
	{
		return m_curIS.m_keyState[keyCode] != 0;
	}

	bool Input::IsKeyPressed(Keyboard::Key keyCode) const
	{
		return m_curIS.m_keyState[keyCode] != 0 && m_preIS.m_keyState[keyCode] == 0;
	}

	bool Input::IsKeyReleased(Keyboard::Key keyCode) const
	{
		return m_curIS.m_keyState[keyCode] == 0 && m_preIS.m_keyState[keyCode] != 0;
	}

	bool Input::IsMouseButtonDown(MouseButtonID button) const
	{
		return m_curIS.m_mouseState.buttonDown(button);
	}

	bool Input::IsMouseButtonPressed(MouseButtonID button) const
	{
		return m_curIS.m_mouseState.buttonDown(button) && !m_preIS.m_mouseState.buttonDown(button);
	}

	bool Input::IsMouseButtonReleased(MouseButtonID button) const
	{
		return !m_curIS.m_mouseState.buttonDown(button) && m_preIS.m_mouseState.buttonDown(button);
	}

	const MouseState& Input::GetMouseState() const
	{
		return m_curIS.m_mouseState;
	}

	void Input::OnResize(int w,int h)
	{
		m_curIS.m_mouseState.width = w;
		m_curIS.m_mouseState.height = h;

		m_preIS.m_mouseState.width = w;
		m_preIS.m_mouseState.height = h;
	}

	Input* g_pInput = NULL;
	Input* GetInput()
	{
		return g_pInput;
	}
}
