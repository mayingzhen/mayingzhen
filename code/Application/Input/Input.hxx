#include "Input.h"

namespace ma
{

	Input::Input()
	{
		memset(&m_curIS,0,sizeof(InputState));
		memset(&m_preIS,0,sizeof(InputState));
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
		m_curIS.m_mouseState.Z.rel = m_curIS.m_mouseState.Z.abs - m_preIS.m_mouseState.Z.abs;
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
