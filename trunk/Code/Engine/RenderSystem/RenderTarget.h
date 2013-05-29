#ifndef  _RenderTarget__H__
#define  _RenderTarget__H__

#include "MWTexture.h"

namespace ma
{

class ENGINE_API RenderTarget : public Texture
{
public:
    RenderTarget();
    virtual ~RenderTarget();

    virtual void Stretch(Texture * texture) = 0;

    const TString128 & GetName() const;
    int GetWidth() const;
    int GetHeight() const;
    FORMAT GetFormat() const;
    MULTI_SAMPLE GetMultiSampleType() const;

protected:
    TString128 mName;

    int mWidth;
    int mHeight;
    FORMAT mFormat;
    MULTI_SAMPLE mMSAA;
};

DeclareSmartPtr(RenderTarget);

}