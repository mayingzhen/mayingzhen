#ifndef MOUSE_H_
#define MOUSE_H_

namespace ma
{

	//! Button ID for mouse devices
	enum MouseButtonID
	{
		MB_Left = 0, MB_Right, MB_Middle,
		MB_Button3, MB_Button4,	MB_Button5, MB_Button6,	MB_Button7
	};

	//! Axis component
	class Axis 
	{
	public:
		Axis() : abs(0), rel(0), absOnly(false) {};

		//! Absoulte and Relative value components
		int abs, rel;

		//! Indicates if this Axis only supports Absoulte (ie JoyStick)
		bool absOnly;

		//! Used internally by OIS
		void clear()
		{
			abs = rel = 0;
		}
	};

	/**
		Represents the state of the mouse
		All members are valid for both buffered and non buffered mode
	*/
	class MouseState
	{
	public:
		MouseState() : width(50), height(50), buttons(0) {};

		/** Represents the height/width of your display area.. used if mouse clipping
		or mouse grabbed in case of X11 - defaults to 50.. Make sure to set this
		and change when your size changes.. */
		mutable int width, height;

		//! X Axis component
		Axis X;

		//! Y Axis Component
		Axis Y;

		//! Z Axis Component
		Axis Z;

		//! represents all buttons - bit position indicates button down
		int buttons;

		//! Button down test
		inline bool buttonDown( MouseButtonID button ) const
		{
			return ((buttons & ( 1L << button )) == 0) ? false : true;
		}

		//! Clear all the values
		void clear()
		{
			X.clear();
			Y.clear();
			Z.clear();
			buttons = 0;
		}
	};


/**
 * Mouse event
 */
class Mouse
{
public:

    /**
     * The mouse event type.
     */
    enum MouseEvent
    {
        MOUSE_NONE = 0,
        MOUSE_PRESS_LEFT_BUTTON = 1,
        MOUSE_RELEASE_LEFT_BUTTON = 2, 
        MOUSE_PRESS_MIDDLE_BUTTON = 3,
        MOUSE_RELEASE_MIDDLE_BUTTON = 4,
        MOUSE_PRESS_RIGHT_BUTTON = 5,
        MOUSE_RELEASE_RIGHT_BUTTON = 6,
        MOUSE_MOVE = 7,
        MOUSE_WHEEL = 8,
    };


private:

    /**
     * Constructor. Used internally.
     */
	Mouse(){}
};

}

#endif