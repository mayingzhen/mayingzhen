#include "HardwareBuffer.h"


namespace ma
{

	HardwareBuffer::HardwareBuffer()
	{
		m_Stride = 0;
		m_Size = 0;
		m_Usage = HBU_STATIC;
		m_pData = NULL;
		m_bNeedFreeData = true;
	}

	HardwareBuffer::~HardwareBuffer()
	{
		if (m_bNeedFreeData)
		{
			FreeData();
		}
	}

	void* HardwareBuffer::Lock(LOCK LockFlags)
	{
		if (LockFlags == LOCK_DISCARD)
		{
			ASSERT((m_Usage&HBU_DYNAMIC) != 0);
		}
		return this->Lock(0, m_Size, LockFlags);
	}

	void HardwareBuffer::SetData(uint8* pData,UINT nSize,int nStride,HBU_USAGE eUsage,bool bCopyData)
	{
		if (bCopyData && pData)
		{
			SAFE_DELETE_ARRAY(m_pData);
			m_pData = new uint8[nSize];
			memcpy(m_pData,pData,nSize);
			m_bNeedFreeData = true;
		}
		else
		{
			m_pData = pData;
			m_bNeedFreeData = false;
		}

		m_Size = nSize;
		m_Stride = nStride;
		m_Usage = eUsage;
	}

	void HardwareBuffer::FreeData()
	{
		SAFE_DELETE_ARRAY(m_pData);
	}

}

