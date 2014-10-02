#include "BinaryOutputSerializer.h"

namespace ma
{

	BinaryOutputSerializer::BinaryOutputSerializer()
	{
		m_pStream = NULL;
	}

	BinaryOutputSerializer::~BinaryOutputSerializer()
	{
	}

	bool BinaryOutputSerializer::IsReading() const
	{
		return false;
	}

	void BinaryOutputSerializer::Close()
	{
		ASSERT(m_pStream);
		if (m_pStream == NULL)
			return;

		m_pStream->Close();
	}


	void BinaryOutputSerializer::SerializeByte(Uint8* &pData,UINT nSizeToWrite,const char* pszLable)
	{
		ASSERT(m_pStream);
		if (m_pStream == NULL)
			return;

		int nWrite = m_pStream->Write(pData,nSizeToWrite);
		ASSERT(nWrite == nSizeToWrite);
	}

	bool BinaryOutputSerializer::Open(const char* pszFilename)
	{
		m_pStream = GetArchiveMananger()->Create(pszFilename);
		if (m_pStream == NULL)
		{
			Log("fail to open file %s\n",pszFilename);
			return false;
		}

		return true;
	}
}

