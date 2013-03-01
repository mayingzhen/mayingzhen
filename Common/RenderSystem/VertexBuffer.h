#pragma once

#include "RenderDefine.h"

namespace ma
{

class COMMON_API VertexBuffer : public ReferenceCountObject
{
public:
    VertexBuffer() {}
    virtual         ~VertexBuffer() {}

    virtual void *  Lock(int iOffsetBytes, int iLockSize, int LockFlags) = 0;
    virtual void    Unlock() = 0;

    int             GetSize() const { return mSize; }
    USAGE           GetUsage() const { return mUsage; }

protected:
    int             mSize;
    USAGE           mUsage;
};

//DeclareSmartPtr(VertexBuffer);

}