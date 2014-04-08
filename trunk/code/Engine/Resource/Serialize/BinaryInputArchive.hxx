#include "BinaryInputArchive.h"

namespace ma
{
	BinaryInputArchive::BinaryInputArchive()
	{
	}


	BinaryInputArchive::~BinaryInputArchive()
	{
		//Close();
	}

	bool BinaryInputArchive::SerializeByte(Uint8* &pData,UINT nSizeToRead,const char* pszLable)
	{
		if (pData == NULL)
		{
			pData = new Uint8[nSizeToRead];
		}

 		return m_pStream->Read(pData,nSizeToRead) == nSizeToRead;
	}

	bool BinaryInputArchive::Open(Stream* pStream)
	{
		m_pStream = pStream;
		return true;
	}

	bool BinaryInputArchive::Open(const char* pszFilename)
	{
		m_pStream = GetArchiveMananger()->Open(pszFilename,false);
		ASSERT(m_pStream);

		return m_pStream != NULL;	
	}


	void BinaryInputArchive::Close()
	{
		m_pStream->Close();
	}

	bool BinaryInputArchive::IsReading() const
	{
		return true;
	}

}
