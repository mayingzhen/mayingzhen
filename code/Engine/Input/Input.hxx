#include "Input.h"


#include <sstream>

namespace ma
{
	static Input* gs_input = 0;

	void	SetInput(Input* pInput)
	{
		gs_input = pInput;
	}

	Input*	GetInput()
	{
		return gs_input;
	}


	Input::Input()
	{
		mInputMgr = NULL;
		mKeyboard = NULL;
		mMouse = NULL;
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

#elif PLAFTORM_IOS == 1
		OIS::ParamList pl;
		size_t winHandle = 0;
		std::ostringstream winHandleStr;
		winHandleStr << winId;
		pl.insert(std::make_pair("WINDOW", winHandleStr.str()));
		mInputMgr = OIS::InputManager::createInputSystem(pl);
		mTouch = static_cast<OIS::MultiTouch*>(mInputMgr->createInputObject(OIS::OISMultiTouch, true));
		mAccelerometer = static_cast<OIS::JoyStick*>(mInputMgr->createInputObject(OIS::OISJoyStick, true));

#elif PLATFORM_ANDROID == 1	
		mTouch = new AndroidMultiTouch();
		mKeyboard = new AndroidKeyboard();
		mInputInjector = new AndroidInputInjector(mTouch, mKeyboard);
#endif
	
	}

	void Input::Shutdown()
	{
		if(mInputMgr)
		{
			OIS::InputManager::destroyInputSystem(mInputMgr);
		}
	}

	void Input::Capture() const
	{
		if(mKeyboard)
			mKeyboard->capture();
		if(mMouse)
			mMouse->capture();

		//if(mTouch)
		//	mTouch->capture();

		//if(mAccelerometer)
		//	mAccelerometer->capture();
	}

	bool Input::IsKeyDown(OIS::KeyCode keyCode) const
	{
		return mKeyboard && mKeyboard->isKeyDown(keyCode);
	}


	bool Input::IsMouseButtonDown(OIS::MouseButtonID button) const
	{
		return mMouse->getMouseState().buttonDown(button);
	}

	const OIS::MouseState& Input::GetMouseState() const
	{
		return mMouse->getMouseState();
	}

#if PLAFTORM_IOS == 1 || PLATFORM_ANDROID == 1
	UINT Input::GetTouchStateNumber() const
	{
		return mTouch.getMultiTouchStates().size();
	}

	const OIS::MultiTouchState& Input::GetTouchStateByIndex(int index) const
	{
		std::vector<MultiTouchState>& arrState = mTouch.getMultiTouchStates();
		return arrState[index];
	}
#endif

	void Input::OnResize(int w,int h)
	{
		if (NULL != mMouse)
		{
			mMouse->getMouseState().width = w;
			mMouse->getMouseState().height = h;
		}	
	}

}