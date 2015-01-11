#include "Input.h"


#include <sstream>

#if PLATFORM_ANDROID == 1	
#include "AndroidInputInjector.hxx"
#endif


namespace ma
{
	static Input* gs_input = 0;

	void	SetInput(Input* pInput)
	{
		gs_input = pInput;
	}

	Input*	GetInput()
	{
		ASSERT(gs_input);
		return gs_input;
	}

	Input::Input()
	{
        mInputMgr = NULL;
		mKeyboard = NULL;
		mMouse = NULL;
		mAccelerometer = NULL;
		mTouch = NULL;
		mAndroidInputInjector = NULL;
	}

	Input::~Input()
	{
	}

	void Input::Init(HWND winId)
	{
#if PLATFORM_WIN == 1 
		OIS::ParamList pl;
		size_t winHandle = 0;
		std::ostringstream winHandleStr;
		winHandleStr << (int)winId;
		pl.insert(std::make_pair("WINDOW", winHandleStr.str()));

		pl.insert(std::make_pair(std::string("w32_mouse"),  
			std::string("DISCL_FOREGROUND" ))); 
		pl.insert(std::make_pair(std::string("w32_mouse"),  
			std::string("DISCL_NONEXCLUSIVE"))); 
		pl.insert(std::make_pair(std::string("w32_keyboard"),
			std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_keyboard"),
			std::string("DISCL_NONEXCLUSIVE")));

		mInputMgr = OIS::InputManager::createInputSystem(pl);

		mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, true));
		mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, true));

		mKeyboard->setEventCallback(this);
		mMouse->setEventCallback(this);

#elif PLAFTORM_IOS == 1
		OIS::ParamList pl;
		size_t winHandle = 0;
		std::ostringstream winHandleStr;
		winHandleStr << (int)winId;
		pl.insert(std::make_pair("WINDOW", winHandleStr.str()));

		mInputMgr = OIS::InputManager::createInputSystem(pl);
		
       mTouch = static_cast<OIS::MultiTouch*>(mInputMgr->createInputObject(OIS::OISMultiTouch, true));
		mAccelerometer = static_cast<OIS::JoyStick*>(mInputMgr->createInputObject(OIS::OISJoyStick, true));
		mTouch->setEventCallback(this);

#elif PLATFORM_ANDROID == 1	
		mTouch = new AndroidMultiTouch();
		mKeyboard = new AndroidKeyboard();
		mAndroidInputInjector = new AndroidInputInjector((AndroidMultiTouch*)mTouch, (AndroidKeyboard*)mKeyboard);
#endif

//		ASSERT(false);
// 		int w,h;
// 		Platform::GetInstance().GetWindowSize(w,h);
// 		OnResize(w,h);
	}

	void Input::Shutdown()
	{
		ASSERT(mInputMgr);
		if (mInputMgr == NULL)
			return;
		
		mInputMgr->destroyInputObject(mKeyboard);
		mInputMgr->destroyInputObject(mMouse);
		OIS::InputManager::destroyInputSystem(mInputMgr);

		mKeyboard = NULL;
		mMouse = NULL;
		mInputMgr = NULL;
	}

	void Input::Capture() const
	{
		if(mKeyboard)
		{
			mKeyboard->capture();
		}

		if(mMouse)
		{
			mMouse->capture();
		}

		if(mTouch)
		{
			mTouch->capture();
		}

		if(mAccelerometer)
		{
			mAccelerometer->capture();
		}
	}

	UINT Input::GetTouchStateNumber() const
	{
		return mTouch->getMultiTouchStates().size();
	}

	const OIS::MultiTouchState& Input::GetTouchStateByIndex(int index) const
	{
		const std::vector<OIS::MultiTouchState>& arrState = mTouch->getMultiTouchStates();
		return arrState[index];
	}

	void Input::OnResize(int w,int h)
	{
		if (mMouse)
		{
			mMouse->getMouseState().width = w;
			mMouse->getMouseState().height = h;
		}
        
        if(mTouch)
        {
			const std::vector<OIS::MultiTouchState>& arrTouchState = mTouch->getMultiTouchStates();
			for (UINT i = 0; i < arrTouchState.size(); ++i)
			{
				arrTouchState[i].width = w;
				arrTouchState[i].height = h;
			}
        }
	}

	
	bool Input::keyPressed(const OIS::KeyEvent &arg)
	{
		for (UINT i = 0; i < m_arrKeyListener.size(); ++i)
		{
			m_arrKeyListener[i]->keyPressed(arg);
		}

		return true;
	}

	bool Input::keyReleased(const OIS::KeyEvent &arg)
	{
		for (UINT i = 0; i < m_arrKeyListener.size(); ++i)
		{
			m_arrKeyListener[i]->keyReleased(arg);
		}

		return true;
	}

	bool Input::mouseMoved( const OIS::MouseEvent &arg )
	{
		for (UINT i = 0; i < m_arrMouseListener.size(); ++i)
		{
			m_arrMouseListener[i]->mouseMoved(arg);
		}

		return true;
	}

	bool Input::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		for (UINT i = 0; i < m_arrMouseListener.size(); ++i)
		{
			m_arrMouseListener[i]->mousePressed(arg,id);
		}

		return true;
	}

	bool Input::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		for (UINT i = 0; i < m_arrMouseListener.size(); ++i)
		{
			m_arrMouseListener[i]->mouseReleased(arg,id);
		}

		return true;
	}

	bool Input::touchMoved( const OIS::MultiTouchEvent &arg )
	{
		for (UINT i = 0; i < m_arrTouchListener.size(); ++i)
		{
			m_arrTouchListener[i]->touchMoved(arg);
		}	

		return true;
	}	

	bool Input::touchPressed( const OIS::MultiTouchEvent &arg )
	{
		for (UINT i = 0; i < m_arrTouchListener.size(); ++i)
		{
			m_arrTouchListener[i]->touchPressed(arg);
		}	

		return true;
	}

	bool Input::touchReleased( const OIS::MultiTouchEvent &arg )
	{
		for (UINT i = 0; i < m_arrTouchListener.size(); ++i)
		{
			m_arrTouchListener[i]->touchReleased(arg);
		}	

		return true;
	}

	bool Input::touchCancelled( const OIS::MultiTouchEvent &arg )
	{
		for (UINT i = 0; i < m_arrTouchListener.size(); ++i)
		{
			m_arrTouchListener[i]->touchCancelled(arg);
		}

		return true;
	}

	void Input::AddKeyListener(OIS::KeyListener* pKeyListen)
	{
		m_arrKeyListener.push_back(pKeyListen);
	}

	void Input::AddMouseListener(OIS::MouseListener* pMouseListen) 
	{
		m_arrMouseListener.push_back(pMouseListen);
	}

	void Input::AddTouchListener(OIS::MultiTouchListener* pTouchListen) 
	{
		m_arrTouchListener.push_back(pTouchListen);
	}

	void Input::RemoveKeyListener(OIS::KeyListener* pListen)
	{
		std::vector<OIS::KeyListener*>::iterator it;	
		it = std::find(m_arrKeyListener.begin(),m_arrKeyListener.end(),pListen);
		if (it != m_arrKeyListener.end())
		{
			m_arrKeyListener.erase(it);
		}
	}

	void Input::RemoveMouseListener(OIS::MouseListener* pListen)
	{
		std::vector<OIS::MouseListener*>::iterator it;	
		it = std::find(m_arrMouseListener.begin(),m_arrMouseListener.end(),pListen);
		if (it != m_arrMouseListener.end())
		{
			m_arrMouseListener.erase(it);
		}
	}

	void Input::RemoveTouchListener(OIS::MultiTouchListener* pListen)
	{
		std::vector<OIS::MultiTouchListener*>::iterator it;	
		it = std::find(m_arrTouchListener.begin(),m_arrTouchListener.end(),pListen);
		if (it != m_arrTouchListener.end())
		{
			m_arrTouchListener.erase(it);
		}
	}

}