#ifndef __Input_H__
#define __Input_H__

#include "OIS.h"

#if PLATFORM_ANDROID == 1
#include "AndroidInputInjector.h"
#endif

namespace ma
{
	class AndroidInputInjector;
	class AndroidMultiTouch;
	class AndroidKeyboard;

	class ENGINE_API Input 
	{

	public:

		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		Input();

		void Init(HWND winId);

		void Shutdown();

		void Capture() const;

		bool IsKeyDown(OIS::KeyCode keyCode) const;

		bool IsMouseButtonDown(OIS::MouseButtonID button) const;

		const OIS::MouseState& GetMouseState() const;

#if PLATFORM_WIN == 1
		OIS::Mouse*			   GetMouse() {return mMouse;}

		OIS::Keyboard*		   GetKeyboard() {return mKeyboard;}

#elif PLAFTORM_IOS == 1 || PLATFORM_ANDROID == 1
		//OIS::MultiTouch*			GetMultiTouch() {return mTouch;}
		//UINT						GetTouchStateNumber() const;
		//const OIS::MultiTouchState& GetTouchStateByIndex(int index) const;
#endif

		void OnResize(int w,int h);

	protected:
		
		OIS::InputManager*	mInputMgr;
		OIS::Mouse*			mMouse;

#if PLATFORM_WIN == 1
		OIS::Keyboard*		mKeyboard;

#elif PLAFTORM_IOS == 1
		OIS::InputManager*	mInputMgr;
		OIS::MultiTouch*	mTouch;        // multitouch device
		OIS::JoyStick*		mAccelerometer;  // accelerometer device

#elif PLATFORM_ANDROID == 1
		AndroidInputInjector* mInputInjector;
		AndroidMultiTouch*	mTouch;
		AndroidKeyboard*	mKeyboard;
#endif

	};

	ENGINE_API void		SetInput(Input* pInput);

	ENGINE_API Input*	GetInput();

}


#endif //__Input_H__