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

		int             GetSize() const { return m_Size; }
		USAGE           GetUsage() const { return m_Usage; }
		//int				GetStride() const {return m_Stride;}

		virtual void	Bind(void* Data, int size, UINT nVertexType, USAGE Usgae = USAGE_STATIC) = 0;

	protected:
		//int				m_Stride;
		int             m_Size;
		USAGE           m_Usage;
		
		//VertexDeclaration* m_pDeclaration;
	};

	DeclareSmartPtr(VertexBuffer);

}

#endif