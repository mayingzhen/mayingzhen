#ifndef  _IndexBuffer__H__
#define  _IndexBuffer__H__

#include "RenderDefine.h"

namespace ma
{
	class ENGINE_API IndexBuffer : public RefCountObject
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

	DeclareSmartPtr(IndexBuffer);

}

#endif