#include "HardwareBuffer.h"


namespace ma
{

	HardwareBuffer::HardwareBuffer()
	{
		m_Stride = 0;
		m_Size = 0;
		m_Usage = USAGE_STATIC;
		m_pData = NULL;
		m_bNeedFreeData = false;
	}

	HardwareBuffer::~HardwareBuffer()
	{
	}

	void HardwareBuffer::SetData(uint8* pData,UINT nSize,int nStride,USAGE eUsage)
	{
		m_pData = pData;
		m_Size = nSize;
		m_Stride = nStride;
		m_Usage = eUsage;
	}

	void HardwareBuffer::FreeData()
	{
		SAFE_DELETE_ARRAY(m_pData);
	}

// 	void HardwareBuffer::Serialize(Serializer& sl, const char* pszLable/* = "HardwareBuffer"*/)
// 	{
// 		sl.BeginSection(pszLable);
// 
// 		uint8* pTemp = (uint8*)m_pData; 
// 
// 		sl.Serialize(m_Stride,"Stride");
// 		sl.Serialize(m_Size,"Size");
// 		sl.SerializeByte(pTemp,m_Size,"Data");
// 
// 		m_pData = pTemp;
// 
// 		if ( sl.IsReading() )
// 		{
// 			m_bNeedFreeData = true;
// 		}
// 
// 		sl.EndSection();
// 	}
}

