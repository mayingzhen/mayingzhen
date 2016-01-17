#ifndef __InputH__
#define __InputH__


namespace ma
{
	class APP_API Input
	{

	public:
		Input();

		~Input();

		void Init(int winId);

		void Shutdown();

		void InjectLoseFocus();

		void InjectInputBegin();

		void InjectInputEnd();

		void InjectKeyPressed(Keyboard::Key keyCode);

		void InjectKeyReleased(Keyboard::Key keyCode);

		void InjectMouseState(const MouseState& state);

		bool IsKeyDown(Keyboard::Key keyCode) const;

		bool IsKeyPressed(Keyboard::Key keyCode) const;

		bool IsKeyReleased(Keyboard::Key keyCode) const;

		bool IsMouseButtonDown(MouseButtonID button) const;

		bool IsMouseButtonPressed(MouseButtonID button) const;

		bool IsMouseButtonReleased(MouseButtonID button) const;

		const MouseState& GetMouseState() const;

		void OnResize(int w,int h);

	private:
		void keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key);
		void mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

	private:
		struct InputState
		{
			char			m_keyState[256];
			MouseState		m_mouseState;
		};

		InputState m_curIS;
		InputState m_preIS;

	};

	extern Input* g_pInput;
	APP_API Input* GetInput();
}

#endif 
