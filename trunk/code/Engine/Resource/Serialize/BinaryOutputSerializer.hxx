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


	void BinaryOutputSerializer::SerializeByte(uint8* &pData,UINT nSizeToWrite,const char* pszLable)
	{
		ASSERT(m_pStream);
		if (m_pStream == NULL)
			return;

		int nWrite = m_pStream->Write(pData,nSizeToWrite);
		ASSERT(nWrite == nSizeToWrite);
	}

	bool BinaryOutputSerializer::Open(const char* pszFilename)
	{
		string strFullName = GetArchiveMananger()->GetSaveDir();
		strFullName += pszFilename;

		m_pStream = GetArchiveMananger()->Create(strFullName.c_str());
		if (m_pStream == NULL)
		{
			LogError("fail to open file %s\n",strFullName.c_str());
			return false;
		}

		return true;
	}
}

