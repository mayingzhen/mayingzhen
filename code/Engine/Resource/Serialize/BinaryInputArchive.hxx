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

 		return m_pStream->read(pData,nSizeToRead,1) == 1;
	}

	bool BinaryInputArchive::Open(Stream* pStream)
	{
		m_pStream = pStream;
		return true;
	}

	bool BinaryInputArchive::Open(const char* pszFilename)
	{
// 		m_file.open(pszFilename,std::ios::in | std::ios::binary);
// 
// 		bool resFail =  m_file.fail(); 
// 		if (resFail)
// 		{
// 			Log("fail to open file %s\n",pszFilename);
// 			return false;
// 		}
		m_pStream = FileStream::create(pszFilename,"rb");
		ASSERT(m_pStream);

		return m_pStream != NULL;	
	}


	void BinaryInputArchive::Close()
	{
		m_pStream->close();
	}

	bool BinaryInputArchive::IsReading() const
	{
		return true;
	}

}
