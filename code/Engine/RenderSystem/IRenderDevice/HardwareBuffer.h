#pragma once

#include "Engine/RenderSystem/IRenderDevice/RenderDefine.h"

namespace ma
{
	class HardwareBuffer : public Referenced
	{
	public:
		HardwareBuffer();

		virtual ~HardwareBuffer();

		virtual void    Unlock();

		virtual void*	Lock(LOCK LockFlags);

		virtual void	RT_StreamComplete() = 0;

		int             GetSize() const {return m_Size;} 

		HBU_USAGE       GetUsage() const {return m_Usage;}

		int				GetStride() const {return m_Stride;} 

		uint32_t		GetNumber() const {return m_Size / m_Stride;}

		void			SetData(uint8_t* pData,uint32_t nSize,int nStride, HBU_USAGE eUsage = HBU_STATIC);

		void			FreeData();

		void			UpdateData(uint32_t nOffset, uint8_t* pData, uint32_t nSize);

		virtual void	RT_UpdateData(uint32_t nOffset, uint8_t* pData, uint32_t nSize) = 0;

	protected:
		virtual void*	LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlags) = 0;

		virtual void	UnlockImpl() = 0;

	protected:
		int				m_Stride;
		uint32_t        m_Size;
		HBU_USAGE       m_Usage;

		std::vector<uint8_t> m_cachedata;

		void*			m_pLockedData;	
	};
}

