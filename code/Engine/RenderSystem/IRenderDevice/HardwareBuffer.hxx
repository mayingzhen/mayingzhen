#include "HardwareBuffer.h"


namespace ma
{

	HardwareBuffer::HardwareBuffer()
	{
		m_Stride = 0;
		m_Size = 0;
		m_Usage = HBU_STATIC;
		m_pLockedData = NULL;
	}

	HardwareBuffer::~HardwareBuffer()
	{
		FreeData();
	}

	void* HardwareBuffer::Lock(LOCK LockFlags)
	{
		if (m_pLockedData)
			return m_pLockedData;

		if (LockFlags == LOCK_DISCARD)
		{
			ASSERT((m_Usage&HBU_DYNAMIC) != 0);
		}
		m_pLockedData = LockImpl(0, m_Size, LockFlags);
		return m_pLockedData;
	}

	void HardwareBuffer::Unlock()
	{
		if (m_pLockedData)
		{
			UnlockImpl();

			m_pLockedData = NULL;
		}
	}

	void HardwareBuffer::SetData(uint8_t* pData,uint32_t nSize,int nStride,HBU_USAGE eUsage)
	{
		FreeData();

		m_Size = nSize;
		m_Stride = nStride;
		m_Usage = eUsage;

		if (pData)
		{
			m_cachedata.resize(nSize);
			memcpy(m_cachedata.data(),pData,nSize);
		}
	}

	void HardwareBuffer::UpdateData(uint32_t nOffset, uint8_t* pData, uint32_t nSize)
	{
		ASSERT(m_Usage == HBU_DYNAMIC);
		uint8_t* pCopyData = new uint8_t[nSize];
		memcpy(pCopyData, pData, nSize);
		GetRenderSystem()->RC_AddRenderCommad([this, nOffset, pCopyData, nSize] {
			this->RT_UpdateData(nOffset, pCopyData, nSize);
			delete pCopyData;
		}
		);
	}

	void HardwareBuffer::FreeData()
	{
		m_cachedata.shrink_to_fit();
	}

}

