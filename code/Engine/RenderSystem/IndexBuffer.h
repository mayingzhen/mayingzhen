#ifndef  _IndexBuffer__H__
#define  _IndexBuffer__H__

#include "RenderDefine.h"

namespace ma
{
	class ENGINE_API IndexBuffer : public RefCountObject
	{
	public:
		IndexBuffer(void* Data = NULL, int size = 0, INDEX_TYPE eIndexType = INDEX_TYPE_U16, USAGE Usgae = USAGE_NO) 
		{
			m_pData = Data;
			m_nSize = size;
			m_eType = eIndexType;
			m_eUsage = Usgae;
		}
		

		virtual void*	Lock(int iOffsetBytes, int iLockSize, LOCK LockFlags) = 0;
		
		virtual void	Unlock() = 0;

		virtual void	Active() = 0;

		int				GetSize() const { return m_nSize; }
		
		USAGE			GetUsage() const { return m_eUsage; }
		
		INDEX_TYPE		GetIndexType() {return m_eType;}
		
		int				GetStride() {return m_eType == INDEX_TYPE_U16 ? sizeof(Uint16) : sizeof(Uint32);}

		void*			GetData() {return m_pData;}

	protected:
		int			m_nSize;
		USAGE		m_eUsage;
		INDEX_TYPE	m_eType;
		void*		m_pData;
	};

	DeclareSmartPtr(IndexBuffer);

}

#endif