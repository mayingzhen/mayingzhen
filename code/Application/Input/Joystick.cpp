#include "GamePlayCommon.h"
#include "Joystick.h"

namespace tq
{

const Vector2& Joystick::getValue() const
{
	return _value;
}

void Joystick::setInnerRegionSize(const Vector2& size)
{
	if (_innerSize)
		*_innerSize = size;
}

const Vector2& Joystick::getInnerRegionSize() const
{
	if (_innerSize)
		return *_innerSize;
	else
		return Vector2::ZERO;
}

void Joystick::setOuterRegionSize(const Vector2& size)
{
	if (_outerSize)
		*_outerSize = size;
}

const Vector2& Joystick::getOuterRegionSize() const
{
	if (_outerSize)
		return *_outerSize;
	else
		return Vector2::ZERO;
}

void Joystick::setRelative(bool relative)
{
	_relative = relative;
}

bool Joystick::isRelative() const
{
	return _relative;
}

const unsigned int Joystick::getIndex() const
{
	return _index;
}


Joystick::Joystick() : _radius(1.0f), _relative(true), _innerSize(NULL), _outerSize(NULL)
{
	_displacement = _value = Vector2::ZERO;
    _index = 0;
}

Joystick::~Joystick()
{
	if (_innerSize)
		SAFE_DELETE(_innerSize);
	if (_outerSize)
		SAFE_DELETE(_outerSize);
}

Joystick* Joystick::create(const char* id, Theme::Style* style)
{
	GP_ASSERT(style != NULL);

	Joystick* joystick = new Joystick();
	if (id)
		joystick->_id = id;
	joystick->setStyle(style);

	return joystick;
}

Joystick* Joystick::create(Theme::Style* style, Properties* properties)
{
	Joystick* joystick = new Joystick();
	joystick->initialize(style, properties);
	joystick->_index = properties->getInt("index");
	return joystick;
}

void Joystick::initialize(Theme::Style* style, Properties* properties)
{
	GP_ASSERT(properties != NULL);

	Control::initialize(style, properties);

	if (!properties->exists("radius"))
	{
		ASSERT(false && "Failed to load joystick; required attribute 'radius' is missing.");
		return;
	}
	_radius = properties->getFloat("radius");
	GP_ASSERT(_radius != 0.0f);

	if (properties->exists("relative"))
	{
		setRelative(properties->getBool("relative"));
	}
	else
	{
		setRelative(false);
	}

	Theme::ThemeImage* inner = getImage("inner", _state);
	if (inner)
	{
		_innerSize = new Vector2();
		Vector2 innerSize;
		if (properties->getVector2("innerRegion", &innerSize))
		{
			*_innerSize = Vector2(innerSize.x, innerSize.y);
		}
		else
		{
			const GRectangle& rect = inner->getRegion();
			*_innerSize = Vector2(rect.width, rect.height);
		}
	}

	Theme::ThemeImage* outer = getImage("outer", _state);
	if (outer)
	{
		_outerSize = new Vector2();
		Vector2 outerSize;
		if (properties->getVector2("outerRegion", &outerSize))
		{
			*_outerSize = Vector2(outerSize.x, outerSize.y);
		}
		else
		{
			const GRectangle& rect = outer->getRegion();
			*_outerSize = Vector2(rect.width, rect.height);
		}

		_screenRegion.width = _outerSize->x;
		_screenRegion.height = _outerSize->y;
	}
	else
	{
		if (inner)
		{
			const GRectangle& rect = inner->getRegion();
			_screenRegion.width = rect.width;
			_screenRegion.height = rect.height;
		}
		else
		{
			_screenRegion.width = _radius * 2.0f;
			_screenRegion.height = _screenRegion.width;
		}
	}
}

void Joystick::addListener(Control::Listener* listener, int eventFlags)
{
	if ((eventFlags & Listener::TEXT_CHANGED) == Listener::TEXT_CHANGED)
	{
		ASSERT(false && "TEXT_CHANGED event is not applicable to this control.");
	}

	Control::addListener(listener, eventFlags);
}

bool Joystick::touchEvent(Touch::TouchEvent touchEvent, int x, int y, unsigned int contactIndex)
{
	switch (touchEvent)
	{
	case Touch::TOUCH_PRESS:
		{
			if (_contactIndex == INVALID_CONTACT_INDEX)
			{
				float dx = 0.0f;
				float dy = 0.0f;

				_contactIndex = (int) contactIndex;
				notifyListeners(Listener::PRESS);

            // Get the displacement of the touch from the centre.
            if (!_relative)
            {
                dx = x - _screenRegion.width * 0.5f;
                dy = _screenRegion.height * 0.5f - y;
            }
            else
            {
                _screenRegion.x = x + _bounds.x - _screenRegion.width * 0.5f;
                _screenRegion.y = y + _bounds.y - _screenRegion.height * 0.5f;
            }

				_displacement = Vector2(dx, dy);

				// If the displacement is greater than the radius, then cap the displacement to the
				// radius.

				Vector2 value;
				if ((fabs(_displacement.x) > _radius) || (fabs(_displacement.y) > _radius))
				{
					_displacement.normalise();
					value = _displacement;
					_displacement *= _radius;
				}
				else
				{
					value = _displacement;
					GP_ASSERT(_radius > 0);
					value *= 1.0f / _radius;
				}

				// Check if the value has changed. Won't this always be the case?
				if (_value != value)
				{
					_value = value;
					_dirty = true;
					notifyListeners(Listener::VALUE_CHANGED);
				}

				_state = ACTIVE;
				return _consumeInputEvents;
			}
			break;
		}
	case Touch::TOUCH_MOVE:
		{
			if (_contactIndex == (int) contactIndex)
			{
            float dx = x - ((_relative) ? _screenRegion.x - _bounds.x : 0.0f) - _screenRegion.width * 0.5f;
            float dy = -(y - ((_relative) ? _screenRegion.y - _bounds.y : 0.0f) - _screenRegion.height * 0.5f);

				_displacement = Vector2(dx, dy);

				Vector2 value;
				if ((fabs(_displacement.x) > _radius) || (fabs(_displacement.y) > _radius))
				{
					_displacement.normalise();
					value = _displacement;
					_displacement *= _radius;
				}
				else
				{
					value = _displacement;
					GP_ASSERT(_radius > 0);
					value *= (1.0f / _radius);
				}

				if (_value != value)
				{
					_value = value;
					_dirty = true;
					notifyListeners(Listener::VALUE_CHANGED);
				}

				return _consumeInputEvents;
			}
			break;
		}
	case Touch::TOUCH_RELEASE:
		{
			if (_contactIndex == (int) contactIndex)
			{
				_contactIndex = INVALID_CONTACT_INDEX;

				notifyListeners(Listener::RELEASE);

				// Reset displacement and direction vectors.
				_displacement = Vector2(0.0f, 0.0f);
				Vector2 value(_displacement);
				if (_value != value)
				{
					_value = value;
					_dirty = true;
					notifyListeners(Listener::VALUE_CHANGED);
				}

				_state = NORMAL;

				return _consumeInputEvents;
			}
			break;
		}
	}

	return false;
}

void Joystick::drawImages(SpriteBatch* spriteBatch, const GRectangle& clip)
{
	GP_ASSERT(spriteBatch != NULL);
	spriteBatch->start();

    // If the joystick is not absolute, then only draw if it is active.
    if (!_relative || (_relative && _state == ACTIVE))
    {
        if (!_relative)
        {
            _screenRegion.x = _viewportClipBounds.x + (_viewportClipBounds.width - _screenRegion.width) / 2.0f;
            _screenRegion.y = _viewportClipBounds.y + (_viewportClipBounds.height - _screenRegion.height) / 2.0f;
        }

        // Draw the outer image.
        Theme::ThemeImage* outer = getImage("outer", _state);
        if (outer)
        {
            const Theme::UVs& uvs = outer->getUVs();
            const Vector4& color = outer->getColor();
            if (_relative)
                spriteBatch->draw(_screenRegion.x, _screenRegion.y, _outerSize->x, _outerSize->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color);
            else
                spriteBatch->draw(_screenRegion.x, _screenRegion.y, _outerSize->x, _outerSize->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _viewportClipBounds);
        }

        // Draw the inner image.
        Theme::ThemeImage* inner = getImage("inner", _state);
        if (inner)
        {
            Vector2 position(_screenRegion.x, _screenRegion.y);
            
            // Adjust position to reflect displacement.
            position.x += _displacement.x;
            position.y += -_displacement.y;
            
            // Get the uvs and color and draw.
            const Theme::UVs& uvs = inner->getUVs();
            const Vector4& color = inner->getColor();
            if (_relative)
                spriteBatch->draw(position.x, position.y, _innerSize->x, _innerSize->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color);
            else
                spriteBatch->draw(position.x, position.y, _innerSize->x, _innerSize->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _viewportClipBounds);
        }
    }
    spriteBatch->finish();
}

const char* Joystick::getType() const
{
    return "joystick";
}

}
