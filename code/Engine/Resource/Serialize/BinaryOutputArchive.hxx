#include "BinaryOutputArchive.h"

namespace ma
{

	BinaryOutputArchive::BinaryOutputArchive()
	{
		m_pStream = NULL;
	}

	BinaryOutputArchive::~BinaryOutputArchive()
	{
	}

	bool BinaryOutputArchive::IsReading() const
	{
		return false;
	}

	void BinaryOutputArchive::Close()
	{
		ASSERT(m_pStream);
		if (m_pStream == NULL)
			return;

		m_pStream->Close();
	}


	bool BinaryOutputArchive::SerializeByte(Uint8* &pData,UINT nSizeToRead,const char* pszLable)
	{
		ASSERT(m_pStream);
		if (m_pStream == NULL)
			return false;

		return m_pStream->Write(pData,nSizeToRead) == nSizeToRead;
	}

	bool BinaryOutputArchive::Open(const char* pszFilename)
	{
		m_pStream = GetArchiveMananger()->Open(pszFilename,false);
		if (m_pStream == NULL)
		{
			Log("fail to open file %s\n",pszFilename);
			return false;
		}

		return true;
	}
}

