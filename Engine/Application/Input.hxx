#include "Input.h"

#include <sstream>

namespace ma
{
	Input* gs_input = 0;


	void	SetInput(Input* pInput)
	{
		gs_input = pInput;
	}

	Input*	GetInput()
	{
		return gs_input;
	}



	Input::Input()
	:m_inputMan(NULL)
	,m_keyboard(NULL)
	,m_mouse(NULL)
	{
		memset(&m_curIS,0,sizeof(InputState));
		memset(&m_preIS,0,sizeof(InputState));
	}


	void Input::Init(int winId)
	{
		if (0 != winId)
		{
			OIS::ParamList pl;

			std::ostringstream wnd;
			wnd << (size_t)winId;
			pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));

			pl.insert(std::make_pair(std::string("w32_mouse"),  
				std::string("DISCL_FOREGROUND" ))); 
			pl.insert(std::make_pair(std::string("w32_mouse"),  
				std::string("DISCL_NONEXCLUSIVE"))); 

			m_inputMan = OIS::InputManager::createInputSystem(pl);


			m_keyboard = (OIS::Keyboard*)m_inputMan->createInputObject( OIS::OISKeyboard, true );
			m_mouse = (OIS::Mouse*)m_inputMan->createInputObject( OIS::OISMouse, true );

			m_keyboard->setEventCallback(this);
			m_mouse->setEventCallback(this);
		}
	} 

	void Input::Shutdown()
	{
		if(NULL != m_inputMan)
		{
			OIS::InputManager::destroyInputSystem(m_inputMan);
		}
	}


	void Input::CaptureInput()
	{
		InjectInputBegin();

		if (m_keyboard != NULL)
		{
			m_keyboard->copyKeyStates(m_curIS.m_keyState);
			m_keyboard->capture();
			
					
		}

		if (NULL != m_mouse)
		{
			m_curIS.m_mouseState = m_mouse->getMouseState();
			m_mouse->capture();
			
		}

		InjectInputEnd();
		
	}

	void Input::InjectInputBegin()
	{
		m_preIS = m_curIS;
		
		if (m_inputMan == NULL)
		{
			m_curIS.m_mouseState.X.rel = 0;
			m_curIS.m_mouseState.Y.rel = 0;
			m_curIS.m_mouseState.Z.rel = 0;
		}



	}

	void Input::InjectInputEnd()
	{
		if (m_inputMan == NULL)
		{
			m_curIS.m_mouseState.X.rel = m_curIS.m_mouseState.X.abs - m_preIS.m_mouseState.X.abs;
			m_curIS.m_mouseState.Y.rel = m_curIS.m_mouseState.Y.abs - m_preIS.m_mouseState.Y.abs;
			//m_curIS.m_mouseState.Z.rel = m_curIS.m_mouseState.Z.abs - m_preIS.m_mouseState.Z.abs;
		}
	}

	void  Input::InjectKeyPressed(OIS::KeyCode keyCode)
	{
		m_curIS.m_keyState[keyCode] = 1;
	}

	void  Input::InjectKeyReleased(OIS::KeyCode keyCode)
	{
		m_curIS.m_keyState[keyCode] = 0;
	}

	void Input::InjectMouseStatus(const OIS::MouseState& mouseState)
	{
		m_curIS.m_mouseState = mouseState;
	}


	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------

	bool Input::IsKeyDown(OIS::KeyCode keyCode) const
	{
		return m_curIS.m_keyState[keyCode] != 0;
	}


	/*!\brief
	return true if key is pressed on current frame
	*/
	bool Input::IsKeyPressed(OIS::KeyCode keyCode) const
	{
		return (m_curIS.m_keyState[keyCode] != 0 && (m_preIS.m_keyState[keyCode] == 0));
	}


	/*!\brief
	return true if key is up on current frame
	*/
	bool Input::IsKeyReleased(OIS::KeyCode keyCode) const
	{
		return (m_curIS.m_keyState[keyCode] == 0 && (m_preIS.m_keyState[keyCode] != 0));
	}



	bool Input::IsMouseButtonDown(OIS::MouseButtonID button) const
	{
		return m_curIS.m_mouseState.buttonDown(button);
	}


	bool Input::IsMouseButtonPressed(OIS::MouseButtonID button) const
	{
		return (m_curIS.m_mouseState.buttonDown(button) && !m_preIS.m_mouseState.buttonDown(button));
	}


	bool Input::IsMouseButtonReleased(OIS::MouseButtonID button) const
	{
		return (!m_curIS.m_mouseState.buttonDown(button) && m_preIS.m_mouseState.buttonDown(button));

	}


	const OIS::MouseState* Input::GetMouseState() const
	{
		return & m_curIS.m_mouseState;
	}

	void Input::OnResize(int w,int h)
	{
		m_curIS.m_mouseState.width = w;
		m_curIS.m_mouseState.height = h;

		m_preIS.m_mouseState.width = w;
		m_preIS.m_mouseState.height = h;

		if (NULL != m_mouse)
		{
			m_mouse->getMouseState().width = w;
			m_mouse->getMouseState().height = h;
		}
		
	}


	void Input::AddMouseListener(OIS::MouseListener* pListener)
	{
		m_vecMouseListener.push_back(pListener);
	}

	void Input::RemoveMouseListener(OIS::MouseListener* pListener)
	{
		VecMouseListener::iterator iter = std::find(m_vecMouseListener.begin(),m_vecMouseListener.end(),pListener);
		if (iter != m_vecMouseListener.end())
		{
			m_vecMouseListener.erase(iter);
		}	
	}


	void Input::AddKeyListener(OIS::KeyListener* pListener)
	{
		m_vecKeyListener.push_back(pListener);
	}

	void Input::RemoveKeyListener(OIS::KeyListener* pListener)
	{
		VecKeyListener::iterator iter = std::find(m_vecKeyListener.begin(),m_vecKeyListener.end(),pListener);
		if (iter != m_vecKeyListener.end())
		{
			m_vecKeyListener.erase(iter);
		}	
	}



	bool Input::keyPressed(const OIS::KeyEvent &arg)
	{
		bool bContinue = true;
		for (UINT nCnt = 0; nCnt < m_vecKeyListener.size(); ++nCnt)
		{
			bContinue &= m_vecKeyListener[nCnt]->keyPressed(arg);
		}
		return bContinue;
		
	}

	bool Input::keyReleased(const OIS::KeyEvent &arg)
	{
		bool bContinue = true;
		for (UINT nCnt = 0; nCnt < m_vecKeyListener.size(); ++nCnt)
		{
			m_vecKeyListener[nCnt]->keyReleased(arg);
		}
		return bContinue;
	}


	bool Input::mouseMoved( const OIS::MouseEvent &arg )
	{
		bool bContinue = true;
		for (UINT nCnt = 0; nCnt < m_vecKeyListener.size(); ++nCnt)
		{
			bContinue &= m_vecMouseListener[nCnt]->mouseMoved(arg);
		}
		return bContinue;
	}

	bool Input::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		bool bContinue = true;
		for (UINT nCnt = 0; nCnt < m_vecKeyListener.size(); ++nCnt)
		{
			bContinue &= m_vecMouseListener[nCnt]->mousePressed(arg,id);
		}
		return bContinue;
	}

	bool Input::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		bool bContinue = true;
		for (UINT nCnt = 0; nCnt < m_vecKeyListener.size(); ++nCnt)
		{
			bContinue &= m_vecMouseListener[nCnt]->mouseReleased(arg,id);
		}
		return bContinue;
	}



}
