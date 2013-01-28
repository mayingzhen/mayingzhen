#ifndef __InputOIS_H__
#define __InputOIS_H__

#include <ois/OIS.h>

namespace OIS
{
	enum KeyCodeCount
	{
		KC_COUNT = KC_MEDIASELECT + 1,
	};

	enum MouseButtonCount
	{
		MB_COUNT = MB_Button7 + 1,
	};
}

namespace ma
{
	class Input;
	class InputListener;

	FRAMEWORK_API void	SetInput(Input* pInput);

	FRAMEWORK_API Input*	GetInput();

	class FRAMEWORK_API Input : public OIS::MouseListener, public OIS::KeyListener
	{
		struct InputState
		{
			char			m_keyState[256];
			unsigned int	m_modifierState;
			OIS::MouseState	m_mouseState;
		};

		OIS::InputManager*	m_inputMan;
		OIS::Keyboard*		m_keyboard;
		OIS::Mouse*			m_mouse;


		//pre frame's input state
		InputState m_curIS;
		InputState m_preIS;
		

		typedef std::vector<OIS::MouseListener*> VecMouseListener;
		typedef std::vector<OIS::KeyListener*> VecKeyListener;

		VecMouseListener	m_vecMouseListener;
		VecKeyListener		m_vecKeyListener;


	public:

		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		Input();

		void Init(int winId);

		void Shutdown();


		//------------------------------------------------------------------------------
		//Capture input from OIS
		//------------------------------------------------------------------------------
		void CaptureInput();

		//------------------------------------------------------------------------------
		//Inject input from external input system e.g Qt window
		//------------------------------------------------------------------------------
		void InjectInputBegin();

		void InjectInputEnd();

		void InjectKeyPressed(OIS::KeyCode keyCode);

		void InjectKeyReleased(OIS::KeyCode keyCode);

		void InjectMouseStatus(const OIS::MouseState& mouseState);

		


		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------

		bool IsKeyDown(OIS::KeyCode keyCode) const;

		/*!\brief
		return true if key is pressed on current frame
		*/
		bool IsKeyPressed(OIS::KeyCode keyCode) const;

		/*!\brief
		return true if key is up on current frame
		*/
		bool IsKeyReleased(OIS::KeyCode keyCode) const;


		bool IsMouseButtonDown(OIS::MouseButtonID button) const;

		bool IsMouseButtonPressed(OIS::MouseButtonID button) const;

		bool IsMouseButtonReleased(OIS::MouseButtonID button) const;

		const OIS::MouseState* GetMouseState() const;

		void OnResize(int w,int h);

		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		void AddMouseListener(OIS::MouseListener* pListener);

		void RemoveMouseListener(OIS::MouseListener* pListener);

		void AddKeyListener(OIS::KeyListener* pListener);

		void RemoveKeyListener(OIS::KeyListener* pListener);

	protected:
		//------------------------------------------------------------------------------
		//Input Dispatch
		//------------------------------------------------------------------------------

		virtual bool keyPressed(const OIS::KeyEvent &arg);
		virtual bool keyReleased(const OIS::KeyEvent &arg);

		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	};


}


#endif //__InputOIS_H__