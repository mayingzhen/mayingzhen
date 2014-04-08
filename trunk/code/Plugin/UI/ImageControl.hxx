//#include "Base.h"
#include "ImageControl.h"

namespace ma
{

ImageControl::ImageControl() :
    _srcRegion(Rectangle::empty()), _dstRegion(Rectangle::empty()), _batch(NULL),
    _tw(0.0f), _th(0.0f), _uvs(Theme::UVs::full())
{
}

ImageControl::~ImageControl()
{
    SAFE_DELETE(_batch);
}

ImageControl* ImageControl::create(const char* id, Theme::Style* style)
{
    ASSERT(style);

    ImageControl* imageControl = new ImageControl();
    if (id)
        imageControl->_id = id;
    imageControl->setStyle(style);

    imageControl->_consumeInputEvents = false;
    imageControl->_focusIndex = -2;

    return imageControl;
}

ImageControl* ImageControl::create(Theme::Style* style, Properties* properties)
{
    ImageControl* imageControl = new ImageControl();
    imageControl->initialize(style, properties);

    imageControl->_consumeInputEvents = false;
    imageControl->_focusIndex = -2;

    return imageControl;
}

void ImageControl::initialize(Theme::Style* style, Properties* properties)
{
    ASSERT(properties);

    Control::initialize(style, properties);

    std::string path;
    if (properties->getPath("path", &path))
    {
        setImage(path.c_str());
    }

    if (properties->exists("srcRegion"))
    {
        Vector4 region;
        properties->getVector4("srcRegion", &region);
        setRegionSrc(region.x, region.y, region.z, region.w);
    }

    if (properties->exists("dstRegion"))
    {
        Vector4 region;
        properties->getVector4("dstRegion", &region);
        setRegionDst(region.x, region.y, region.z, region.w);
    }
}

void ImageControl::setImage(const char* path)
{
    SAFE_DELETE(_batch);
    TexturePtr texture = LoadResourceSync<Texture>(path);
    _batch = new SpriteBatch(texture);
    _tw = 1.0f / texture->getWidth();
    _th = 1.0f / texture->getHeight();
    //texture->release();
}

void ImageControl::setRegionSrc(float x, float y, float width, float height)
{
    _srcRegion.set(x, y, width, height);

    _uvs.u1 = x * _tw;
    _uvs.u2 = (x + width) * _tw;
    _uvs.v1 = 1.0f - (y * _th);
    _uvs.v2 = 1.0f - ((y + height) * _th);
}

void ImageControl::setRegionSrc(const Rectangle& region)
{
    setRegionSrc(region.x, region.y, region.width, region.height);
}

const Rectangle& ImageControl::getRegionSrc() const
{
    return _srcRegion;
}

void ImageControl::setRegionDst(float x, float y, float width, float height)
{
    _dstRegion.set(x, y, width, height);
}

void ImageControl::setRegionDst(const Rectangle& region)
{
    setRegionDst(region.x, region.y, region.width, region.height);
}

const Rectangle& ImageControl::getRegionDst() const
{
    return _dstRegion;
}

const char* ImageControl::getType() const
{
    return "image";
}

void ImageControl::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    spriteBatch->Render(NULL);

    // An ImageControl is not part of the texture atlas but should use the same projection matrix.
    _batch->SetProjectionMatrix(spriteBatch->GetProjectionMatrix());

    Vector4 color = Vec4One();
    color.w *= _opacity;

    _batch->Reset();
    if (_dstRegion.isEmpty())
    {
        _batch->Draw(_viewportBounds.x, _viewportBounds.y, _viewportBounds.width, _viewportBounds.height,
            _uvs.u1, _uvs.v1, _uvs.u2, _uvs.v2, color, _viewportClipBounds);
    }
    else
    {
        _batch->Draw(_viewportBounds.x + _dstRegion.x, _viewportBounds.y + _dstRegion.y,
            _dstRegion.width, _dstRegion.height,
            _uvs.u1, _uvs.v1, _uvs.u2, _uvs.v2, color, _viewportClipBounds);
    }
    _batch->Render(NULL);

    spriteBatch->Reset();
}

}