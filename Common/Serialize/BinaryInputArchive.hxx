#include <Common/Serialize/BinaryInputArchive.h>
//#include <Animation/Common/String/StringHelper.h>

namespace ma
{


BinaryInputArchive::BinaryInputArchive()
{
}


BinaryInputArchive::~BinaryInputArchive()
{
	Close();
}



xmUint BinaryInputArchive::Tell()
{
	if (m_file)
	{
		return (xmUint) m_file.tellg();
	}else{
		return 0;
	}
	
}

void BinaryInputArchive::Seek(xmUint nPos)
{
	if (m_file)
	{
		m_file.seekg(nPos,std::ios::beg);
	}
	
}

bool BinaryInputArchive::SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable)
{
	m_file.read((char*)pData,nSizeToRead);
	return ! m_file.fail();
}



void BinaryInputArchive::SkipByte(xmUint nSizeToSkip)
{
	m_file.seekg(nSizeToSkip,std::ios_base::cur);
}


bool BinaryInputArchive::Open(const char* pszFilename)
{
   m_file.open(pszFilename,std::ios::in | std::ios::binary);

	if ( m_file.fail() )
	{
		Log("fail to open file %s\n",pszFilename);
		return false;
	}

	return true;
	
}


void BinaryInputArchive::Close()
{
	m_file.close();
}

bool BinaryInputArchive::IsReading() const
{
	return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

BinaryMemoryInputArchive::BinaryMemoryInputArchive()
:m_pData(NULL)
,m_nDataSize(0)
,m_nCurPos(0)
{
}


BinaryMemoryInputArchive::~BinaryMemoryInputArchive()
{
}

bool BinaryMemoryInputArchive::IsReading()const
{
	return true;
}

xmUint BinaryMemoryInputArchive::Tell()
{
	return m_nCurPos;
}

void BinaryMemoryInputArchive::Seek(xmUint nPos)
{
	if (nPos < m_nDataSize)
	{
		m_nCurPos = nPos;
	}else{
		Log("Fail to seek %d : out of file boundary\n",nPos);
	}	
}

bool BinaryMemoryInputArchive::SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable)
{
	bool bLoadOK = true;
	if (m_nCurPos + nSizeToRead <= m_nDataSize)
	{
		memcpy(pData,m_pData+m_nCurPos,nSizeToRead);
		m_nCurPos += nSizeToRead;
	}else{
		Log("Fail to read %d bytes at position %d : out of file boundary\n",nSizeToRead,m_nCurPos);
		bLoadOK = false;
	}
	return bLoadOK;
}



void BinaryMemoryInputArchive::SkipByte(xmUint nSizeToSkip)
{
	if (m_nCurPos + nSizeToSkip <= m_nDataSize)
	{
		m_nCurPos += nSizeToSkip;
	}else{
		Log("Fail to skip %d bytes at position %d : out of file boundary\n",nSizeToSkip,m_nCurPos);
	}
	
}

bool BinaryMemoryInputArchive::Open(const xmUint8* pData,xmUint32 nSizeInByte)
{
	m_pData = pData;
	m_nDataSize = nSizeInByte;
	m_nCurPos = 0;
	return true;
}


void BinaryMemoryInputArchive::Close()
{
	m_pData = NULL;
	m_nDataSize = 0;
	m_nCurPos = 0;
}

}
