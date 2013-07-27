#ifndef _AndroidInputInjector_H_
#define _AndroidInputInjector_H_


namespace ma
{
	/*=============================================================================
     | Android input handling
     =============================================================================*/
    class AndroidMultiTouch : public OIS::MultiTouch
    {
    public:
        AndroidMultiTouch();
        
        /** @copydoc Object::setBuffered */
        virtual void setBuffered(bool buffered){}
        
        /** @copydoc Object::capture */
        virtual void capture(){}
        
        /** @copydoc Object::queryInterface */
        virtual OIS::Interface* queryInterface(OIS::Interface::IType type) {return 0;}
        
        /** @copydoc Object::_initialize */
        virtual void _initialize(){}
        
        OIS::MultiTouchState &getMultiTouchState(int i);
    };
    
    class AndroidKeyboard : public OIS::Keyboard
    {
    public:
        AndroidKeyboard():OIS::Keyboard("DWM", false, 1, 0){}
        
        /** @copydoc Object::setBuffered */
        virtual void setBuffered(bool buffered){}
        
        /** @copydoc Object::capture */
        virtual void capture(){}
        
        /** @copydoc Object::queryInterface */
        virtual OIS::Interface* queryInterface(OIS::Interface::IType type) {return 0;}
        
        /** @copydoc Object::_initialize */
        virtual void _initialize(){}
        
        virtual bool isKeyDown( OIS::KeyCode key ) const{ return false;}
        
        virtual const std::string& getAsString( OIS::KeyCode kc ){
            static std::string defstr = "";
            return defstr;
        }
        
        virtual void copyKeyStates( char keys[256] ) const{}
    };
    
    /*=============================================================================
     | Android input injection
     =============================================================================*/
    class AndroidInputInjector
    {
    private:
        //SampleBrowser* mBrowser;
        AndroidMultiTouch* mTouch;
        AndroidKeyboard* mKeyboard;
        
    public:
        
        AndroidInputInjector(AndroidMultiTouch* touch, AndroidKeyboard* keyboard);
        
        void injectKeyEvent(int action, int32_t keyCode);

        void injectTouchEvent(int action, float x, float y, int pointerId = 0);
    };

}

#endif //_AndroidInputInjector_H_