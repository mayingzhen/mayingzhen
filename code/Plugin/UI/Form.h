#ifndef FORM_H_
#define FORM_H_


#include "Container.h"

namespace ma
{

class Theme;

/**
 * Top-level container of UI controls.  The following properties are available for forms:

 @verbatim
    form <formID>
    {
        // Form properties.
        theme       = <Path to .theme File> // See Theme.h.
        layout      = <Layout::Type>        // A value from the Layout::Type enum.  e.g.: LAYOUT_VERTICAL
        style       = <styleID>             // A style from the referenced theme.
        position   = <x, y>                // Position of the form on-screen, measured in pixels.
        alignment  = <Control::Alignment>  // Note: 'position' will be ignored.
        autoWidth  = <bool>                // Will result in a form the width of the display.
        autoHeight = <bool>                // Will result in a form the height of the display.
        size       = <width, height>       // Size of the form, measured in pixels.
        width      = <width>               // Can be used in place of 'size', e.g. with 'autoHeight = true'
        height     = <height>              // Can be used in place of 'size', e.g. with 'autoWidth = true'
        consumeEvents = <bool>             // Whether the form propagates input events to the Game's input event handler. Default is false
      
        // All the nested controls within this form.
        container { }
        label { }
        textBox { }
        button { }
        checkBox { }
        radioButton { }
        slider { }
    }
 @endverbatim
 */
class UI_API Form : public Container, 
	public OIS::MouseListener, public OIS::KeyListener, public OIS::MultiTouchListener
{
    friend class Platform;
    friend class Game;

public:

	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

	virtual bool touchMoved( const OIS::MultiTouchEvent &arg );
	virtual bool touchPressed( const OIS::MultiTouchEvent &arg );
	virtual bool touchReleased( const OIS::MultiTouchEvent &arg );
	virtual bool touchCancelled( const OIS::MultiTouchEvent &arg );

    /**
     * Creates a form using the data from the Properties object defined at the specified URL, 
     * where the URL is of the format "<file-path>.<extension>#<namespace-id>/<namespace-id>/.../<namespace-id>"
     * (and "#<namespace-id>/<namespace-id>/.../<namespace-id>" is optional). 
     * 
     * @param url The URL pointing to the Properties object defining the form. 
     * @script{create}
     */
	static Form* create(const char* url);

    /**
     * Create a new Form.
     *
     * @param id The Form's ID.
     * @param style The Form's style.
     * @param layoutType The form's layout type.
     *
     * @return The new Form.
     * @script{create}
     */
    static Form* create(const char* id, Theme::Style* style, Layout::Type layoutType = Layout::LAYOUT_ABSOLUTE);

	 /**
     * Updates all visible, enabled forms.
     */
    //static void updateInternal(float elapsedTime);

    /**
     * Get a form from its ID.
     *
     * @param id The ID of the form to search for.
     *
     * @return A form with the given ID, or null if one was not found.
     */
    //static Form* getForm(const char* id);
    
    /**
     * Gets the theme for the form.
     *
     * @return The theme for the form.
     */
    Theme* getTheme() const;

    /**
     * Set the desired size of this form.
     *
     * @param width The width.
     * @param height The height.
     */
    virtual void setSize(float width, float height);

    /**
     * Set the bounds of this form.
     *
     * @param bounds The new bounds to set.
     */
    virtual void setBounds(const Rectangle& bounds);

    /** 
     * Set the desired width of the form.
     *
     * @param width The width.
     */
    virtual void setWidth(float width);

    /** 
     * Set the desired height of the form.
     *
     * @param height The height.
     */
    virtual void setHeight(float height);

    /**
     * Set this form's width to that of the display.
     *
     * @param autoWidth Whether to set this form's width to that of the display.
     */
    virtual void setAutoWidth(bool autoWidth);

    /**
     * Set this form's height to that of the display.
     *
     * @param autoHeight Whether to set this form's height to that of the display.
     */
    virtual void setAutoHeight(bool autoHeight);

    /**
     * Attach this form to a node.
     *
     * A form can be drawn as part of the 3-dimensional world if it is attached to a node.
     * The form's contents will be rendered into a framebuffer which will be used to texture a quad.
     * This quad will be given the same dimensions as the form and must be transformed appropriately.
     * Alternatively, a quad can be set explicitly on a form with the setQuad() methods.
     *
     * @param node The node to attach this form to.
     */
    //void setNode(Node* node);

    /**
     * Updates each control within this form, and positions them according to its layout.
     */
    void update(float elapsedTime);

    /**
     * Draws this form.
     */
    void draw();

    /**
     * @see Control::getType
     */
    const char* getType() const;


	    /**
     * Propagate touch events to enabled forms.
     *
     * @return Whether the touch event was consumed by a form.
     */
    bool touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Propagate key events to enabled forms.
     *
     * @return Whether the key event was consumed by a form.
     */
    bool keyEventInternal(Keyboard::KeyEvent evt, int key);

    /**
     * Propagate mouse events to enabled forms.
     *
     * @return True if the mouse event is consumed or false if it is not consumed.
     *
     * @see Mouse::MouseEvent
     */
    bool mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

private:
    
    /**
     * Constructor.
     */
    Form();

    /**
     * Constructor.
     */
    Form(const Form& copy);

    /**
     * Destructor.
     */
    virtual ~Form();

    /**
     * Initialize a quad for this form in order to draw it in 3D.
     *
     * @param mesh The mesh to create a model from.
     */
    //void initializeQuad(Mesh* mesh);

    /**
     * Update this form's bounds.
     */
    void updateBounds();


    Theme* _theme;                      // The Theme applied to this Form.
    RenderTarget* _frameBuffer;          // FBO the Form is rendered into for texturing the quad. 
    SpriteBatch* _spriteBatch;
    Material* _nodeMaterial;            // Material for rendering this Form in 3d space.
    //float _u2;
    //float _v1;
	Vector4	m_rRect;
    Matrix4x4 _projectionMatrix;           // Orthographic projection matrix to be set on SpriteBatch objects when rendering into the FBO.
    Matrix4x4 _defaultProjectionMatrix;
};

}

#endif
