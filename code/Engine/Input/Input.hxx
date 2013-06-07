#include "Input.h"

//#include "OISInputManager.h"
//#include "OISException.h"
//#include "OISKeyboard.h"
//#include "OISMouse.h"
//#include "OISJoyStick.h"
//#include "OISEvents.h"
#include <sstream>

namespace ma
{
	Input* gs_input = 0;


	ENGINE_API void	SetInput(Input* pInput)
	{
		gs_input = pInput;
	}

	ENGINE_API Input*	GetInput()
	{
		return gs_input;
	}



Input::Input()
:m_inputMan(NULL)
{
	memset(&m_curIS,0,sizeof(InputState));
	memset(&m_preIS,0,sizeof(InputState));

	m_keyboard = NULL;
	m_mouse = NULL;
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
		pl.insert(std::make_pair(std::string("w32_keyboard"),
			std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_keyboard"),
			std::string("DISCL_NONEXCLUSIVE")));

#if S3_PLATFORM_WIN == 1

		m_inputMan = OIS::InputManager::createInputSystem(pl);

		m_keyboard = (OIS::Keyboard*)m_inputMan->createInputObject( OIS::OISKeyboard, true );
		m_mouse = (OIS::Mouse*)m_inputMan->createInputObject( OIS::OISMouse, true );

		m_keyboard->setEventCallback(this);
		m_mouse->setEventCallback(this);
#endif
	}

}

void Input::Shutdown()
{
	if(NULL != m_inputMan)
	{
#if S3_PLATFORM_WIN == 1
		OIS::InputManager::destroyInputSystem(m_inputMan);
#endif
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

void Input::InjectLoseFocus()
{
	memset(m_curIS.m_keyState,0,sizeof(m_curIS.m_keyState));
	m_curIS.m_mouseState.clear();
	m_curIS.m_modifierState = 0;
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

	m_strIME.clear();
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

void  Input::InjectKeyPressed(OIS::KeyCode keyCode,xmUint text)
{
	m_curIS.m_keyState[keyCode] = 1;

	OIS::KeyEvent event(NULL,keyCode,text);
	keyPressed(event);
}

void  Input::InjectKeyReleased(OIS::KeyCode keyCode,xmUint text)
{
	m_curIS.m_keyState[keyCode] = 0;

	OIS::KeyEvent event(NULL,keyCode,text);
	keyReleased(event);
}

bool Input::InjectMouseMoved( const OIS::MouseEvent &arg )
{
	m_curIS.m_mouseState = arg.state;
	return mouseMoved(arg);
}

bool Input::InjectMousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	m_curIS.m_mouseState = arg.state;
	return mousePressed(arg,id);
}

bool Input::InjectMouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	m_curIS.m_mouseState = arg.state;
	xmInt btnMask = 1 << id;
	m_curIS.m_mouseState.buttons &= ~btnMask;
	return mouseReleased(arg,id);
}


void Input::OnIME(int text)
{
	char pszChar[3];
	*(xmShort*)(&pszChar[0]) = (xmShort)text;
	pszChar[2] = '\0';
	if (0 != text)
	{
		m_strIME += pszChar;
	}

	for (xmInt nCnt = 0; nCnt < m_vecIMEListener.size(); ++nCnt)
	{
		m_vecIMEListener[nCnt]->OnIME(text);
	}
		
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
const char* Input::GetIMEString() const
{
	return m_strIME.length() > 0 ? m_strIME.c_str() : NULL;
}

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

void Input::AddIMEListener(IMEListener* pListener)
{
	m_vecIMEListener.push_back(pListener);
}

void Input::RemoveIMEListener(IMEListener* pListener)
{
	VecIMEListener::iterator iter = std::find(m_vecIMEListener.begin(),m_vecIMEListener.end(),pListener);
	if (iter != m_vecIMEListener.end())
	{
		m_vecIMEListener.erase(iter);
	}	
}



bool Input::keyPressed(const OIS::KeyEvent &arg)
{
	bool bContinue = true;
	for (xmUint nCnt = 0; nCnt < m_vecKeyListener.size(); ++nCnt)
	{
		bContinue &= m_vecKeyListener[nCnt]->keyPressed(arg);
	}
	return bContinue;
	
}



bool Input::keyReleased(const OIS::KeyEvent &arg)
{
	bool bContinue = true;
	for (xmUint nCnt = 0; nCnt < m_vecKeyListener.size(); ++nCnt)
	{
		m_vecKeyListener[nCnt]->keyReleased(arg);
	}
	return bContinue;
}


bool Input::mouseMoved( const OIS::MouseEvent &arg )
{
	bool bContinue = true;
	for (xmUint nCnt = 0; nCnt < m_vecKeyListener.size(); ++nCnt)
	{
		bContinue &= m_vecMouseListener[nCnt]->mouseMoved(arg);
	}
	return bContinue;
}

bool Input::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	bool bContinue = true;
	for (xmUint nCnt = 0; nCnt < m_vecKeyListener.size(); ++nCnt)
	{
		bContinue &= m_vecMouseListener[nCnt]->mousePressed(arg,id);
	}
	return bContinue;
}

bool Input::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	bool bContinue = true;
	for (xmUint nCnt = 0; nCnt < m_vecKeyListener.size(); ++nCnt)
	{
		bContinue &= m_vecMouseListener[nCnt]->mouseReleased(arg,id);
	}
	return bContinue;
}



}