#ifndef TOUCH_H_
#define TOUCH_H_

namespace ma
{

/**
 * Touch event
 */
class Touch
{
public:

    /**
     * The touch event type.
     */
    enum TouchEvent
    {
        TOUCH_PRESS,
        TOUCH_RELEASE,
        TOUCH_MOVE
    };

private:

    /**
     * Constructor. Used internally.
     */
    Touch();
};

}

#endif
