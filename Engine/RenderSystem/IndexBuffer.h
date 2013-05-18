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

		virtual void*	Lock(int iOffsetBytes, int iLockSize, int LockFlags) = 0;
		virtual void	Unlock() = 0;

		virtual void	Bind(void* Data, int size, INDEX_TYPE eIndexType, USAGE Usgae = USAGE_STATIC) = 0;

		int			GetSize() const { return m_nSize; }
		USAGE		GetUsage() const { return m_eUsage; }
		INDEX_TYPE	GetIndexType() {return m_eType;}
		int			GetStride() {return m_eType == INDEX_TYPE_U16 ? sizeof(Uint16) : sizeof(Uint32);}

	protected:
		int			m_nSize;
		USAGE		m_eUsage;
		INDEX_TYPE	m_eType;
	};

	DeclareSmartPtr(IndexBuffer);

}

#endif