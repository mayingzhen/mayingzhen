#ifndef ABSOLUTELAYOUT_H_
#define ABSOLUTELAYOUT_H_

#include "Layout.h"

namespace ma
{

/**
 * Defines a Layout for forms and containers that requires the user
 * to specify absolute positions for all contained controls.
 */
class AbsoluteLayout : public Layout
{
    friend class Form;
    friend class Container;
	friend class UISystem;

public:

    /**
     * Get the type of this Layout.
     *
     * @return Layout::LAYOUT_ABSOLUTE
     */
    Layout::Type getType();

protected:

    /**
     * Update the controls contained by the specified container.
     *
     * An AbsoluteLayout does nothing to modify the layout of controls.
     * It simply calls update() on any control that is dirty.
     *
     * @param container The container to update.
     * @param offset The layout offset.
     */
    void update(const Container* container, const Vector2& offset);

private:
    
    /*
     * Constructor.
     */
    AbsoluteLayout();
    
    /*
     * Constructor.
     */
    AbsoluteLayout(const AbsoluteLayout& copy);
    
    /*
     * Destructor.
     */
    virtual ~AbsoluteLayout();

    /**
     * Create an AbsoluteLayout.
     *
     * @return An AbsoluteLayout object.
     */
    static AbsoluteLayout* create();

};

}

#endif