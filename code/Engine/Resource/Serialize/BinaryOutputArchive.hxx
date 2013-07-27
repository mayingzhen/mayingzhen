#include "BinaryOutputArchive.h"

namespace ma
{

	BinaryOutputArchive::BinaryOutputArchive()
	{
	}

	BinaryOutputArchive::~BinaryOutputArchive()
	{
		//Close();
	}

	bool BinaryOutputArchive::IsReading() const
	{
		return false;
	}

	void BinaryOutputArchive::Close()
	{
		m_file.close();
	}


	bool BinaryOutputArchive::SerializeByte(Uint8* pData,UINT nSizeToRead,const char* pszLable)
	{
		m_file.write((char*)pData,nSizeToRead);
		return ! m_file.fail();
	}

	bool BinaryOutputArchive::Open(const char* pszFilename)
	{
		m_file.open(pszFilename,std::ios::out | std::ios::binary);

		if ( m_file.fail() )
		{
			Log("fail to open file %s\n",pszFilename);
			return false;
		}

		return true;

	}
}

