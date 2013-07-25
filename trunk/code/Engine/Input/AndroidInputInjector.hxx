#include "AndroidInputInjector.h"

namespace ma
{

	AndroidMultiTouch::AndroidMultiTouch():OIS::MultiTouch("DWM", false, 0, 0)
	{

	}
       
	OIS::MultiTouchState& AndroidMultiTouch::getMultiTouchState(int i)
	{
		while(i >= mStates.size())
		{
			if(Platform::GetInstance().GetWindId())
            {
				OIS::MultiTouchState state;
				Platform::GetInstance().GetWindowSize(state.width,state.height)
                mStates.push_back(state);
            }
            return mStates[i];
		}
	}
        
	AndroidInputInjector::AndroidInputInjector(AndroidMultiTouch* touch, AndroidKeyboard* keyboard) 
         :mTouch(touch), mKeyboard(keyboard) 
	{
	}
        
	void AndroidInputInjector::injectKeyEvent(int action, int32_t keyCode)
    {
        if(keyCode == AKEYCODE_BACK)
        {
            OIS::KeyEvent evt(mKeyboard, OIS::KC_ESCAPE, 0);
            if(action == 0)
            {
                mBrowser->keyPressed(evt);
            }
            else
            {
                mBrowser->keyReleased(evt);
            }
        }
    }
        
	void AndroidInputInjector::injectTouchEvent(int action, float x, float y, int pointerId = 0)
    {
        OIS::MultiTouchState &state = mTouch->getMultiTouchState(pointerId);
        
        switch(action)
        {
            case 0:
                state.touchType = OIS::MT_Pressed;
                break;
            case 1:
                state.touchType = OIS::MT_Released;
                break;
            case 2:
                state.touchType = OIS::MT_Moved;
                break;
            case 3:
                state.touchType = OIS::MT_Cancelled;
                break;
            default:
                state.touchType = OIS::MT_None;
        }
        
        if(state.touchType != OIS::MT_None)
        {
            int last = state.X.abs;
            state.X.abs =  (int)x;
            state.X.rel = state.X.abs - last;
            
            last = state.Y.abs;
            state.Y.abs = (int)y;
            state.Y.rel = state.Y.abs - last;
            
            state.Z.abs = 0;
            state.Z.rel = 0;
            
            OIS::MultiTouchEvent evt(mTouch, state);
            
            switch(state.touchType)
            {
                case OIS::MT_Pressed:
                    mBrowser->touchPressed(evt);
                    break;
                case OIS::MT_Released:
                    mBrowser->touchReleased(evt);
                    break;
                case OIS::MT_Moved:
                    mBrowser->touchMoved(evt);
                    break;
                case OIS::MT_Cancelled:
                    mBrowser->touchCancelled(evt);
                    break;
                default:
                    break;
            }
        }
    }
  

}
