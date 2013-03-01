#pragma once

#include "Common/RenderSystem/RenderDefine.h"

namespace ma
{

class COMMON_API IndexBuffer : public ReferenceCountObject
{
public:
    IndexBuffer() {}
    virtual ~IndexBuffer() {}

    virtual void * Lock(int iOffsetBytes, int iLockSize, int LockFlags) = 0;
    virtual void Unlock() = 0;

    int GetSize() const { return mSize; }
    USAGE GetUsage() const { return mUsage; }
    FORMAT GetFormat() const { return mFormat; }

protected:
    int mSize;
    USAGE mUsage;
    FORMAT mFormat;
};

//DeclareSmartPtr(IndexBuffer);

}