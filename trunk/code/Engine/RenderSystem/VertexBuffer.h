#ifndef  _VertexBuffer__H__
#define  _VertexBuffer__H__

#include "RenderDefine.h"

namespace ma
{

	class ENGINE_API VertexBuffer : public RefCountObject
	{
	public:
		VertexBuffer(void* pData = NULL, int nsize = 0, int nStride = 0, USAGE Usgae = USAGE_STATIC)
		{
			m_pData = pData;
			m_Size = nsize;
			m_Stride = nStride;
			m_Usage = Usgae;
		}

		virtual void*	Lock(int iOffsetBytes, int iLockSize, LOCK LockFlags) = 0;
		
		virtual void    Unlock() = 0;

		//virtual void	Active() = 0;

		int             GetSize() const; 
		
		USAGE           GetUsage() const; 
		
		int				GetStride() const; 

		void*			GetData() const;

	protected:
		int				m_Stride;
		int             m_Size;
		USAGE           m_Usage;
		void*			m_pData;
	
	};

	DeclareSmartPtr(VertexBuffer);

		

	inline int VertexBuffer::GetSize() const 
	{ 
		return m_Size; 
	}

	inline USAGE VertexBuffer::GetUsage() const 
	{
		return m_Usage; 
	}

	inline int VertexBuffer::GetStride() const 
	{
		return m_Stride;
	}

	inline void* VertexBuffer::GetData() const
	{
		return m_pData;
	}
}

#endif
