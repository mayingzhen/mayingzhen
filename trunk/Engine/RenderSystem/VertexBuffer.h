#ifndef  _VertexBuffer__H__
#define  _VertexBuffer__H__

#include "RenderDefine.h"

namespace ma
{

	class ENGINE_API VertexBuffer : public RefCountObject
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

	DeclareSmartPtr(VertexBuffer);

}

#endif