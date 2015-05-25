#include "AndroidInputInjector.h"
#include "OIS.h"
#include <android/input.h>

namespace ma
{

	AndroidMultiTouch::AndroidMultiTouch():OIS::MultiTouch("DWM", false, 0, 0)
	{
		mStates.resize(1);
	}
       
	OIS::MultiTouchState& AndroidMultiTouch::getMultiTouchState(int i)
	{
		while(i >= mStates.size())
		{
			mStates.resize(i);
		}
		return mStates[i];
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
                GetInput()->keyPressed(evt);
            }
            else
            {
                GetInput()->keyReleased(evt);
            }
        }
    }
        
	void AndroidInputInjector::injectTouchEvent(int action, float x, float y, int pointerId)
    {
		LogInfo("injectTouchEvent action = %d x = %f y = %f pointerId = %d",action,x,y,pointerId);
		ASSERT(mTouch);
		if (mTouch == NULL)
			return;

		OIS::MultiTouchState& state = mTouch->getMultiTouchState(pointerId);
        
        switch(action)
        {
            case AMOTION_EVENT_ACTION_DOWN:
                state.touchType = OIS::MT_Pressed;
				LogInfo("touchType MT_Pressed ...........");
                break;
            case AMOTION_EVENT_ACTION_UP:
                state.touchType = OIS::MT_Released;
				LogInfo("touchType MT_Released ...........");
                break;
            case AMOTION_EVENT_ACTION_MOVE:
                state.touchType = OIS::MT_Moved;
				LogInfo("touchType MT_Moved ...........");
                break;
            case AMOTION_EVENT_ACTION_CANCEL:
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
				 GetInput()->touchPressed(evt);
				 break;
			 case OIS::MT_Released:
				 GetInput()->touchReleased(evt);
				 break;
			 case OIS::MT_Moved:
				 GetInput()->touchMoved(evt);
				 break;
			 case OIS::MT_Cancelled:
				 GetInput()->touchCancelled(evt);
				 break;
			 default:
				 break;
			}
		}
    }
 
}
