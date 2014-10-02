#ifndef __Input_H__
#define __Input_H__

#include "OIS.h"



namespace ma
{
	class AndroidInputInjector;
	class AndroidMultiTouch;
	class AndroidKeyboard;

	class ENGINE_API Input : public OIS::KeyListener, OIS::MouseListener, OIS::MultiTouchListener
	{

	public:

		//------------------------------------------------------------------------------
		//
		//------------------------------------------------------------------------------
		Input();

		void						Init(HWND winId);

		void						Shutdown();

		void						Capture() const;

		bool						IsKeyDown(OIS::KeyCode keyCode) const {return mKeyboard ? mKeyboard->isKeyDown(keyCode) : false;}
 
		bool						IsMouseButtonDown(OIS::MouseButtonID button) const {return mMouse ? GetMouseState().buttonDown(button) : false;}

		const OIS::MouseState&		GetMouseState() const {return mMouse->getMouseState();}

		OIS::Mouse*					GetMouse() {return mMouse;}

		OIS::Keyboard*				GetKeyboard() {return mKeyboard;}

		OIS::MultiTouch*			GetMultiTouch() {return mTouch;}

		UINT						GetTouchStateNumber() const;

		const OIS::MultiTouchState& GetTouchStateByIndex(int index) const;

		void						OnResize(int w,int h);

		/// OIS Event Listen
		virtual bool				keyPressed(const OIS::KeyEvent &arg);
		virtual bool				keyReleased(const OIS::KeyEvent &arg);	

		virtual bool				mouseMoved( const OIS::MouseEvent &arg );	
		virtual bool				mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );	
		virtual bool				mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
		virtual bool				touchMoved( const OIS::MultiTouchEvent &arg );
		virtual bool				touchPressed( const OIS::MultiTouchEvent &arg );
		virtual bool				touchReleased( const OIS::MultiTouchEvent &arg );
		virtual bool				touchCancelled( const OIS::MultiTouchEvent &arg );

		// Input Listen	
		void						AddKeyListener(OIS::KeyListener* pKeyListen) {m_arrKeyListener.push_back(pKeyListen);}
		
		void						AddMouseListener(OIS::MouseListener* pMouseListen) {m_arrMouseListener.push_back(pMouseListen);}
		
		void						AddTouchListener(OIS::MultiTouchListener* pTouchListen) {m_arrTouchListener.push_back(pTouchListen);}
	
		void						RemoveKeyListener(OIS::KeyListener* pListen); 

		void						RemoveMouseListener(OIS::MouseListener* pListen); 

		void						RemoveTouchListener(OIS::MultiTouchListener* pListen); 

		AndroidInputInjector*		GetAndroidInputInjector() {return mAndroidInputInjector;}

	protected:
		
		OIS::InputManager*			mInputMgr;

		OIS::Mouse*					mMouse;
	
		OIS::Keyboard*				mKeyboard;

		OIS::MultiTouch*			mTouch;        // multitouch device

		OIS::JoyStick*				mAccelerometer;  // accelerometer device

		AndroidInputInjector*		mAndroidInputInjector;

		std::vector<OIS::KeyListener*>		m_arrKeyListener;
		std::vector<OIS::MouseListener*>    m_arrMouseListener;
		std::vector<OIS::MultiTouchListener*>	m_arrTouchListener;
	};

	ENGINE_API void		SetInput(Input* pInput);

	ENGINE_API Input*	GetInput();

}


#endif //__Input_H__