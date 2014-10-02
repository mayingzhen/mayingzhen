
#include "Form.h"
#include "AbsoluteLayout.h"
#include "FlowLayout.h"
#include "VerticalLayout.h"
#include "Theme.h"
#include "Label.h"
#include "Button.h"
#include "CheckBox.h"


namespace ma
{


Form::Form() : m_pTheme(NULL), m_pFrameBuffer(NULL), m_pSpriteBatch(NULL),
    m_pNodeMaterial(NULL) , m_rRect(0,0,0,0)
{
	GetInput()->AddKeyListener(this);
	GetInput()->AddMouseListener(this);
	GetInput()->AddTouchListener(this);

	Ref();
}

Form::~Form()
{
    SAFE_DELETE(m_pSpriteBatch);
    //SAFE_DEC_REF(_frameBuffer);
    SAFE_DEC_REF(m_pTheme);
}

bool Form::mouseMoved( const OIS::MouseEvent &arg )
{
	if (!mouseEventInternal(Mouse::MOUSE_MOVE, arg.state.X.abs, arg.state.Y.abs, 0))
	{
		touchEventInternal(Touch::TOUCH_MOVE,arg.state.X.abs,arg.state.Y.abs,0);
	}

	return true;
}

bool Form::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	bool res = false;
	if (id == OIS::MB_Left)
	{
		if (!mouseEventInternal(Mouse::MOUSE_PRESS_LEFT_BUTTON, arg.state.X.abs, arg.state.Y.abs, 0))
		{
			touchEventInternal(Touch::TOUCH_PRESS,arg.state.X.abs,arg.state.Y.abs,0);	
		}
	}
	else if (id == OIS::MB_Middle)
	{
		res = mouseEventInternal(Mouse::MOUSE_PRESS_MIDDLE_BUTTON, arg.state.X.abs, arg.state.Y.abs, 0);
	}
	else if (id == OIS::MB_Right)
	{
		res = mouseEventInternal(Mouse::MOUSE_PRESS_RIGHT_BUTTON, arg.state.X.abs, arg.state.Y.abs, 0);
	}

	return true;
}

bool Form::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (id == OIS::MB_Left)
	{
		if ( !mouseEventInternal(Mouse::MOUSE_RELEASE_LEFT_BUTTON, arg.state.X.abs, arg.state.Y.abs, 0) )
			touchEventInternal(Touch::TOUCH_RELEASE, arg.state.X.abs, arg.state.Y.abs, 0);
	}
	else if (id == OIS::MB_Middle)
	{
		mouseEventInternal(Mouse::MOUSE_RELEASE_MIDDLE_BUTTON, arg.state.X.abs, arg.state.Y.abs, 0);
	}
	else if (id == OIS::MB_Right)
	{
		mouseEventInternal(Mouse::MOUSE_RELEASE_RIGHT_BUTTON, arg.state.X.abs, arg.state.Y.abs, 0);
	}

	return true;
}

bool Form::keyPressed(const OIS::KeyEvent &arg)
{
	keyEventInternal(Keyboard::KEY_PRESS,arg.key);

	return true;
}

bool Form::keyReleased(const OIS::KeyEvent &arg)
{
	keyEventInternal(Keyboard::KEY_RELEASE,arg.key);

	return true;
}

bool Form::touchMoved( const OIS::MultiTouchEvent &arg )
{
	if (!touchEventInternal(Touch::TOUCH_MOVE,arg.state.X.abs,arg.state.Y.abs,0))
	{
		mouseEventInternal(Mouse::MOUSE_MOVE,arg.state.X.abs,arg.state.Y.abs,0);
	}

	return true;
}

bool Form::touchPressed( const OIS::MultiTouchEvent &arg )
{
	touchEventInternal(Touch::TOUCH_PRESS,arg.state.X.abs,arg.state.Y.abs,0);

	return true;
}

bool Form::touchReleased( const OIS::MultiTouchEvent &arg )
{
	touchEventInternal(Touch::TOUCH_RELEASE,arg.state.X.abs,arg.state.Y.abs,0);

	return true;
}

bool Form::touchCancelled( const OIS::MultiTouchEvent &arg )
{
	return true;
}

Form* Form::create(const char* id, Theme::Style* style, Layout::Type layoutType)
{
    ASSERT(style);

    Layout* layout;
    switch (layoutType)
    {
    case Layout::LAYOUT_ABSOLUTE:
        layout = AbsoluteLayout::create();
        break;
    case Layout::LAYOUT_FLOW:
        layout = FlowLayout::create();
        break;
    case Layout::LAYOUT_VERTICAL:
        layout = VerticalLayout::create();
        break;
    default:
        GP_ERROR("Unsupported layout type '%d'.", layoutType);
        break;
    }

    Form* form = new Form();
    if (id)
        form->_id = id;
    form->_style = style;
    form->_layout = layout;
    form->m_pTheme = style->getTheme();
    form->m_pTheme->Ref();

	Rectangle rect = GetRenderSystem()->GetViewPort();
	GetRenderDevice()->MakeOrthoMatrixOffCenter(form->m_matDefaultProjection, 0, rect.width, rect.height, 0, 0.0f, 1.0f);

    form->updateBounds();

    return form;
}

Form* Form::create(const char* url)
{
    // Load Form from .form file.
    Properties* properties = Properties::create(url);
    if (properties == NULL)
    {
        ASSERT(properties);
        return NULL;
    }

    // Check if the Properties is valid and has a valid namespace.
    Properties* formProperties = (strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace();
    assert(formProperties);
    if (!formProperties || !(strcmp(formProperties->getNamespace(), "form") == 0))
    {
        ASSERT(formProperties);
        SAFE_DELETE(properties);
        return NULL;
    }

    // Create new form with given ID, theme and layout.
    const char* themeFile = formProperties->getString("theme");
    const char* layoutString = formProperties->getString("layout");
        
    Layout* layout;
    switch (getLayoutType(layoutString))
    {
    case Layout::LAYOUT_ABSOLUTE:
        layout = AbsoluteLayout::create();
        break;
    case Layout::LAYOUT_FLOW:
        layout = FlowLayout::create();
        break;
    case Layout::LAYOUT_VERTICAL:
        layout = VerticalLayout::create();
        break;
    default:
        GP_ERROR("Unsupported layout type '%d'.", getLayoutType(layoutString));
        break;
    }

    Theme* theme = Theme::create(themeFile);
    ASSERT(theme);

    Form* form = new Form();
    form->_layout = layout;
    form->m_pTheme = theme;

    // Get default projection matrix.
	Rectangle rect = GetRenderSystem()->GetViewPort();
	GetRenderDevice()->MakeOrthoMatrixOffCenter(form->m_matDefaultProjection, 0, rect.width, rect.height, 0, 0.0f, 1.0f);

    Theme::Style* style = NULL;
    const char* styleName = formProperties->getString("style");
    if (styleName)
    {
        style = theme->getStyle(styleName);
    }
    else
    {
        style = theme->getEmptyStyle();
    }
    form->initialize(style, formProperties);

    form->_consumeInputEvents = formProperties->getBool("consumeInputEvents", false);

    // Alignment
    if ((form->_alignment & Control::ALIGN_BOTTOM) == Control::ALIGN_BOTTOM)
    {
        form->_bounds.y = rect.height - form->_bounds.height;
    }
    else if ((form->_alignment & Control::ALIGN_VCENTER) == Control::ALIGN_VCENTER)
    {
        form->_bounds.y = rect.height * 0.5f - form->_bounds.height * 0.5f;
    }

    if ((form->_alignment & Control::ALIGN_RIGHT) == Control::ALIGN_RIGHT)
    {
        form->_bounds.x = rect.width - form->_bounds.width;
    }
    else if ((form->_alignment & Control::ALIGN_HCENTER) == Control::ALIGN_HCENTER)
    {
        form->_bounds.x = rect.width * 0.5f - form->_bounds.width * 0.5f;
    }

    form->_scroll = getScroll(formProperties->getString("scroll"));
    form->_scrollBarsAutoHide = formProperties->getBool("scrollBarsAutoHide");
    if (form->_scrollBarsAutoHide)
    {
        form->_scrollBarOpacity = 0.0f;
    }

    // Add all the controls to the form.
    form->addControls(theme, formProperties);

    SAFE_DELETE(properties);
    
    form->updateBounds();

    return form;
}



Theme* Form::getTheme() const
{
    return m_pTheme;
}

void Form::setSize(float width, float height)
{
	Rectangle rect = GetRenderSystem()->GetViewPort();

    if (_autoWidth)
    {
		width = rect.width;
    }

    if (_autoHeight)
    {
        height = rect.height;
    }

    if (width != 0.0f && height != 0.0f &&
        (width != _bounds.width || height != _bounds.height))
    {
        // Width and height must be powers of two to create a texture.
		unsigned int w = Math::NextPowerOfTwo(width);
        unsigned int h = Math::NextPowerOfTwo(height);
		
		float fHalftw = GetRenderDevice()->GetHalfPixelOffset(0.5f / w);
		float fHalfth = GetRenderDevice()->GetHalfPixelOffset(0.5f / h);
		m_rRect.x = fHalftw;
		m_rRect.y = fHalftw;
		m_rRect.z = width / (float)w + fHalftw;
		m_rRect.w = height / (float)h + fHalfth;
        //_u2 = width / (float)w;
        //_v1 = height / (float)h;

		m_pFrameBuffer = GetRenderSystem()->CreateRenderTarget(w,h);
        ASSERT(m_pFrameBuffer);

        // Re-create projection matrix.
		GetRenderDevice()->MakeOrthoMatrixOffCenter(m_matProjection, 0, width, height, 0, 0.0f, 1.0f);

        // Re-create sprite batch.
        m_pSpriteBatch = new SpriteBatch(m_pFrameBuffer/*->GetTexture()*/);
        ASSERT(m_pSpriteBatch);

		m_pSpriteBatch->GetStateBlock().m_bDepthWrite = false;
		m_pSpriteBatch->GetStateBlock().m_eDepthCheckMode = DCM_NONE;

		Rectangle viewPort(0, 0, width, height);

        // Clear the framebuffer black
	 	Texture* pPreTarget = GetRenderSystem()->SetRenderTarget(m_pFrameBuffer);
		Rectangle preViewPort = GetRenderSystem()->SetViewPort(viewPort);

        m_pTheme->setProjectionMatrix(m_matProjection);
		ColourValue clearColor(0.0f,0.0f,0.0f,0.0f);
		GetRenderSystem()->ClearBuffer(true,false,false,clearColor, 1.0f, 0);
        m_pTheme->setProjectionMatrix(m_matDefaultProjection);

		GetRenderSystem()->SetRenderTarget(pPreTarget);
		GetRenderSystem()->SetViewPort(preViewPort);
    }
    _bounds.width = width;
    _bounds.height = height;
    _dirty = true;
}

void Form::setBounds(const Rectangle& bounds)
{
    setPosition(bounds.x, bounds.y);
    setSize(bounds.width, bounds.height);
}

void Form::setWidth(float width)
{
    setSize(width, _bounds.height);
}

void Form::setHeight(float height)
{
    setSize(_bounds.width, height);
}

void Form::setAutoWidth(bool autoWidth)
{
    if (_autoWidth != autoWidth)
    {
        _autoWidth = autoWidth;
        _dirty = true;

        if (_autoWidth)
        {
            //setSize(_bounds.width, Game::getInstance()->getWidth());
        }
    }
}

void Form::setAutoHeight(bool autoHeight)
{
    if (_autoHeight != autoHeight)
    {
        _autoHeight = autoHeight;
        _dirty = true;

        if (_autoHeight)
        {
            //setSize(_bounds.width, Game::getInstance()->getHeight());
        }
    }
}


void Form::update(float elapsedTime)
{
    if (isDirty())
    {
        updateBounds();

        // Cache themed attributes for performance.
        _skin = getSkin(_state);
        _opacity = getOpacity(_state);

        ASSERT(_layout);
        if (_scroll != SCROLL_NONE)
        {
            updateScroll();
        }
        else
        {
            _layout->update(this, Vector2::ZERO);
        }
    }
}

void Form::updateBounds()
{   
    _clearBounds.set(_absoluteClipBounds);

    // Calculate the clipped bounds.
    float x = 0;
    float y = 0;
    float width = _bounds.width;
    float height = _bounds.height;

    Rectangle clip(0, 0, _bounds.width, _bounds.height);

    float clipX2 = clip.x + clip.width;
    float x2 = clip.x + x + width;
    if (x2 > clipX2)
        width -= x2 - clipX2;

    float clipY2 = clip.y + clip.height;
    float y2 = clip.y + y + height;
    if (y2 > clipY2)
        height -= y2 - clipY2;

    if (x < 0)
    {
        width += x;
        x = -x;
    }
    else
    {
        x = 0;
    }

    if (y < 0)
    {
        height += y;
        y = -y;
    }
    else
    {
        y = 0;
    }

    _clipBounds.set(x, y, width, height);

    // Calculate the absolute bounds.
    x = 0;
    y = 0;
    _absoluteBounds.set(x, y, _bounds.width, _bounds.height);

    // Calculate the absolute viewport bounds. Absolute bounds minus border and padding.
    const Theme::Border& border = getBorder(_state);
    const Theme::Padding& padding = getPadding();

    x += border.left + padding.left;
    y += border.top + padding.top;
    width = _bounds.width - border.left - padding.left - border.right - padding.right;
    height = _bounds.height - border.top - padding.top - border.bottom - padding.bottom;

    _viewportBounds.set(x, y, width, height);

    // Calculate the clip area. Absolute bounds, minus border and padding, clipped to the parent container's clip area.
    clipX2 = clip.x + clip.width;
    x2 = x + width;
    if (x2 > clipX2)
        width = clipX2 - x;

    clipY2 = clip.y + clip.height;
    y2 = y + height;
    if (y2 > clipY2)
        height = clipY2 - y;

    if (x < clip.x)
    {
        float dx = clip.x - x;
        width -= dx;
        x = clip.x;
    }

    if (y < clip.y)
    {
        float dy = clip.y - y;
        height -= dy;
        y = clip.y;
    }
 
    _viewportClipBounds.set(x, y, width, height);
    _absoluteClipBounds.set(x - border.left - padding.left, y - border.top - padding.top,
                            width + border.left + padding.left + border.right + padding.right,
                            height + border.top + padding.top + border.bottom + padding.bottom);
    if (_clearBounds.isEmpty())
    {
        _clearBounds.set(_absoluteClipBounds);
    }

    // Get scrollbar images and diminish clipping bounds to make room for scrollbars.
    if ((_scroll & SCROLL_HORIZONTAL) == SCROLL_HORIZONTAL)
    {
        _scrollBarLeftCap = getImage("scrollBarLeftCap", _state);
        _scrollBarHorizontal = getImage("horizontalScrollBar", _state);
        _scrollBarRightCap = getImage("scrollBarRightCap", _state);

        _viewportClipBounds.height -= _scrollBarHorizontal->getRegion().height;
    }

    if ((_scroll & SCROLL_VERTICAL) == SCROLL_VERTICAL)
    {
        _scrollBarTopCap = getImage("scrollBarTopCap", _state);
        _scrollBarVertical = getImage("verticalScrollBar", _state);
        _scrollBarBottomCap = getImage("scrollBarBottomCap", _state);
        
        _viewportClipBounds.width -= _scrollBarVertical->getRegion().width;
    }
}

void Form::draw()
{
    // The first time a form is drawn, its contents are rendered into a framebuffer.
    // The framebuffer will only be drawn into again when the contents of the form change.
    // If this form has a node then it's a 3D form and the framebuffer will be used
    // to texture a quad.  The quad will be given the same dimensions as the form and
    // must be transformed appropriately by the user, unless they call setQuad() themselves.
    // On the other hand, if this form has not been set on a node, SpriteBatch will be used
    // to render the contents of the framebuffer directly to the display.

    // Check whether this form has changed since the last call to draw() and if so, render into the framebuffer.
    if (isDirty())
    {
        ASSERT(m_pFrameBuffer);

		Rectangle viewPort(0, 0, _bounds.width, _bounds.height);

		Texture* pPreTarget = GetRenderSystem()->SetRenderTarget(m_pFrameBuffer);
		Rectangle preViewProt = GetRenderSystem()->SetViewPort(viewPort);

        ASSERT(m_pTheme);
        m_pTheme->setProjectionMatrix(m_matProjection);
        
        // By setting needsClear to true here, an optimization meant to clear and redraw only areas of the form
        // that have changed is disabled.  Currently, repositioning controls can result in areas of the screen being cleared
        // after another control has been drawn there.  This should probably be done in two passes -- one to clear areas where
        // dirty controls were last frame, and another to draw them where they are now.
        Container::draw(m_pTheme->getSpriteBatch(), Rectangle(0, 0, _bounds.width, _bounds.height),true, false, _bounds.height);
        m_pTheme->setProjectionMatrix(m_matDefaultProjection);

		GetRenderSystem()->SetViewPort(preViewProt);
		GetRenderSystem()->SetRenderTarget(pPreTarget);
    }


	if (!m_pSpriteBatch)
    {
        m_pSpriteBatch = new SpriteBatch(m_pFrameBuffer/*->GetTexture()*/);
        ASSERT(m_pSpriteBatch);
    }

    m_pSpriteBatch->Reset();
	float fTop = m_rRect.x;
	float fLeft = m_rRect.y;
	float fRight = m_rRect.z;
	float fBottom = m_rRect.w;
	GetRenderDevice()->ConvertUV(fTop,fLeft,fRight,fBottom);
	m_pSpriteBatch->Draw(_bounds.x, _bounds.y, 0, _bounds.width, _bounds.height, fLeft, fTop, fRight, fBottom, ColourValue::White);
    m_pSpriteBatch->Render(NULL);
}

const char* Form::getType() const
{
    return "form";
}

static bool shouldPropagateTouchEvent(Control::State state, Touch::TouchEvent evt, const Rectangle& bounds, int x, int y)
{
    return (state != Control::NORMAL ||
            (evt == Touch::TOUCH_PRESS &&
             x >= bounds.x &&
             x <= bounds.x + bounds.width &&
             y >= bounds.y &&
             y <= bounds.y + bounds.height));
}

bool Form::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (this->isEnabled() && this->isVisible())
    {
        // Simply compare with the form's bounds.
        const Rectangle& bounds = this->getBounds();
        if (shouldPropagateTouchEvent(this->getState(), evt, bounds, x, y))
        {
            // Pass on the event's position relative to the form.
            if (this->touchEvent(evt, x - bounds.x, y - bounds.y, contactIndex))
                return true;
        }
      
    }
    return false;
}

bool Form::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    if (this->isEnabled() && this->isVisible() && this->hasFocus())
    {
        if (this->keyEvent(evt, key))
            return true;
    }
    return false;
}

static bool shouldPropagateMouseEvent(Control::State state, Mouse::MouseEvent evt, const Rectangle& bounds, int x, int y)
{
    return (state != Control::NORMAL ||
            ((evt == Mouse::MOUSE_PRESS_LEFT_BUTTON ||
              evt == Mouse::MOUSE_PRESS_MIDDLE_BUTTON ||
              evt == Mouse::MOUSE_PRESS_RIGHT_BUTTON ||
              evt == Mouse::MOUSE_MOVE ||
              evt == Mouse::MOUSE_WHEEL) &&
                x >= bounds.x &&
                x <= bounds.x + bounds.width &&
                y >= bounds.y &&
                y <= bounds.y + bounds.height));
}

bool Form::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (this->isEnabled() && this->isVisible())
    {
	    // Simply compare with the form's bounds.
        const Rectangle& bounds = this->getBounds();
        if (shouldPropagateMouseEvent(this->getState(), evt, bounds, x, y))
        {
            // Pass on the event's position relative to the form.
            if (this->mouseEvent(evt, x - bounds.x, y - bounds.y, wheelDelta))
                return true;
        }
     
    }
    return false;
}



}
