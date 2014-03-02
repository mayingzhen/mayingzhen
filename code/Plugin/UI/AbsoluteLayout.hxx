
#include "Control.h"
#include "AbsoluteLayout.h"
#include "Container.h"

namespace ma
{

static AbsoluteLayout* __instance;

AbsoluteLayout::AbsoluteLayout()
{
}

AbsoluteLayout::~AbsoluteLayout()
{
    __instance = NULL;
}

AbsoluteLayout* AbsoluteLayout::create()
{
    if (!__instance)
    {
        __instance = new AbsoluteLayout();
    }
    else
    {
        __instance->ref();
    }

    return __instance;
}

Layout::Type AbsoluteLayout::getType()
{
    return Layout::LAYOUT_ABSOLUTE;
}

void AbsoluteLayout::update(const Container* container, const Vector2& offset)
{
    ASSERT(container);

    // An AbsoluteLayout does nothing to modify the layout of Controls.
    const std::vector<Control*>& controls = container->getControls();
    for (size_t i = 0, count = controls.size(); i < count; i++)
    {
        Control* control = controls[i];
        ASSERT(control);

        align(control, container);
        control->update(container, offset);
    }
}

}