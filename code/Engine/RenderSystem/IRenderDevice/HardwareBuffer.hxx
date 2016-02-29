#include "HardwareBuffer.h"


namespace ma
{

	HardwareBuffer::HardwareBuffer()
	{
		m_Stride = 0;
		m_Size = 0;
		m_Usage = USAGE_STATIC;
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

	void HardwareBuffer::SetData(uint8* pData,UINT nSize,int nStride,USAGE eUsage,bool bCopyData)
	{
		if (bCopyData)
		{
			SAFE_DELETE_ARRAY(m_pData);
			m_pData = new uint8[nSize];
			memcpy(m_pData,pData,nSize);
			m_Size = nSize;
			m_Stride = nStride;
			m_Usage = eUsage;
			m_bNeedFreeData = true;
		}
		else
		{
			m_pData = pData;
			m_Size = nSize;
			m_Stride = nStride;
			m_Usage = eUsage;
			m_bNeedFreeData = false;
		}
	}

	void HardwareBuffer::FreeData()
	{
		SAFE_DELETE_ARRAY(m_pData);
	}

}

