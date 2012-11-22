#include <Common/Serialize/S3ABinaryInputArchive.h>
//#include <Animation/Common/String/S3AStringHelper.h>

S3ABinaryInputArchive::S3ABinaryInputArchive()
{
}


S3ABinaryInputArchive::~S3ABinaryInputArchive()
{
}



xmUint S3ABinaryInputArchive::Tell()
{
	if (m_file)
	{
		return (xmUint) m_file.tellg();
	}else{
		return 0;
	}
	
}

void S3ABinaryInputArchive::Seek(xmUint nPos)
{
	if (m_file)
	{
		m_file.seekg(nPos,std::ios::beg);
	}
	
}

bool S3ABinaryInputArchive::SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable)
{
	m_file.read((char*)pData,nSizeToRead);
	return ! m_file.fail();
}



void S3ABinaryInputArchive::SkipByte(xmUint nSizeToSkip)
{
	m_file.seekg(nSizeToSkip,std::ios_base::cur);
}


bool S3ABinaryInputArchive::Open(const char* pszFilename)
{
   m_file.open(pszFilename,std::ios::in | std::ios::binary);

	if ( m_file.fail() )
	{
		S3ALog("fail to open file %s\n",pszFilename);
		return false;
	}

	return true;
	
}


void S3ABinaryInputArchive::Close()
{
	m_file.close();
}

bool S3ABinaryInputArchive::IsReading() const
{
	return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

S3ABinaryMemoryInputArchive::S3ABinaryMemoryInputArchive()
:m_pData(NULL)
,m_nDataSize(0)
,m_nCurPos(0)
{
}


S3ABinaryMemoryInputArchive::~S3ABinaryMemoryInputArchive()
{
}

bool S3ABinaryMemoryInputArchive::IsReading()const
{
	return true;
}

xmUint S3ABinaryMemoryInputArchive::Tell()
{
	return m_nCurPos;
}

void S3ABinaryMemoryInputArchive::Seek(xmUint nPos)
{
	if (nPos < m_nDataSize)
	{
		m_nCurPos = nPos;
	}else{
		S3ALog("Fail to seek %d : out of file boundary\n",nPos);
	}	
}

bool S3ABinaryMemoryInputArchive::SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable)
{
	bool bLoadOK = true;
	if (m_nCurPos + nSizeToRead <= m_nDataSize)
	{
		memcpy(pData,m_pData+m_nCurPos,nSizeToRead);
		m_nCurPos += nSizeToRead;
	}else{
		S3ALog("Fail to read %d bytes at position %d : out of file boundary\n",nSizeToRead,m_nCurPos);
		bLoadOK = false;
	}
	return bLoadOK;
}



void S3ABinaryMemoryInputArchive::SkipByte(xmUint nSizeToSkip)
{
	if (m_nCurPos + nSizeToSkip <= m_nDataSize)
	{
		m_nCurPos += nSizeToSkip;
	}else{
		S3ALog("Fail to skip %d bytes at position %d : out of file boundary\n",nSizeToSkip,m_nCurPos);
	}
	
}

bool S3ABinaryMemoryInputArchive::Open(const xmUint8* pData,xmUint32 nSizeInByte)
{
	m_pData = pData;
	m_nDataSize = nSizeInByte;
	m_nCurPos = 0;
	return true;
}


void S3ABinaryMemoryInputArchive::Close()
{
	m_pData = NULL;
	m_nDataSize = 0;
	m_nCurPos = 0;
}
