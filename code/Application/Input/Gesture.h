#ifndef GESTURE_H_
#define GESTURE_H_

namespace ma
{

/**
 * Gesture event
 */
class Gesture
{
public:

    /**
     * The gesture event type.
     */
    enum GestureEvent
    {
        GESTURE_NONE = 0,
        GESTURE_SWIPE = 0x1,
        GESTURE_PINCH = 0x2,
        GESTURE_ANY_SUPPORTED = -1,
    };

    enum GestureState
    {
        GESTURESTATE_NONE,
        GESTURESTATE_BEGAN,
        GESTURESTATE_CHANGED,
        GESTURESTATE_ENDED,
        GESTURESTATE_CANCELLED,
        GESTURESTATE_FAILED,
    };
    
    /**
     * The up direction for a swipe event.
     */
    static const int SWIPE_DIRECTION_UP = 1 << 0;

    /**
     * The down direction for a swipe event.
     */
    static const int SWIPE_DIRECTION_DOWN = 1 << 1;

    /**
     * The left direction for a swipe event.
     */
    static const int SWIPE_DIRECTION_LEFT = 1 << 2;

    /**
     * The right direction for a swipe event.
     */
    static const int SWIPE_DIRECTION_RIGHT = 1 << 3;

private:

    /**
     * Constructor. Used internally.
     */
	Gesture(){}
};

}

#endif
