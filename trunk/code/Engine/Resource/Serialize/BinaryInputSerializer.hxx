#include "BinaryInputSerializer.h"

namespace ma
{
	BinaryInputSerializer::BinaryInputSerializer()
	{
	}


	BinaryInputSerializer::~BinaryInputSerializer()
	{
		//Close();
	}

	void BinaryInputSerializer::SerializeByte(uint8* &pData,UINT nSizeToRead,const char* pszLable)
	{
		if (pData == NULL)
		{
			pData = new uint8[nSizeToRead];
		}

 		int nRead = m_pStream->Read(pData,nSizeToRead);
		ASSERT(nRead == nSizeToRead);
	}

	bool BinaryInputSerializer::Open(Stream* pStream)
	{
		m_pStream = pStream;
		return true;
	}

	bool BinaryInputSerializer::Open(const char* pszFilename)
	{
		m_pStream = GetArchiveMananger()->Open(pszFilename,false);
		ASSERT(m_pStream);

		return m_pStream != NULL;	
	}


	void BinaryInputSerializer::Close()
	{
		m_pStream->Close();
	}

	bool BinaryInputSerializer::IsReading() const
	{
		return true;
	}

}
