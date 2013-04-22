#include "BinaryInputArchive.h"

namespace ma
{


BinaryInputArchive::BinaryInputArchive()
{
}


BinaryInputArchive::~BinaryInputArchive()
{
	Close();
}



UINT BinaryInputArchive::Tell()
{
	if (m_file)
	{
		return (UINT) m_file.tellg();
	}else{
		return 0;
	}
	
}

void BinaryInputArchive::Seek(UINT nPos)
{
	if (m_file)
	{
		m_file.seekg(nPos,std::ios::beg);
	}
	
}

bool BinaryInputArchive::SerializeByte(Uint8* pData,UINT nSizeToRead,const char* pszLable)
{
	m_file.read((char*)pData,nSizeToRead);
	return ! m_file.fail();
}



void BinaryInputArchive::SkipByte(UINT nSizeToSkip)
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

UINT BinaryMemoryInputArchive::Tell()
{
	return m_nCurPos;
}

void BinaryMemoryInputArchive::Seek(UINT nPos)
{
	if (nPos < m_nDataSize)
	{
		m_nCurPos = nPos;
	}else{
		Log("Fail to seek %d : out of file boundary\n",nPos);
	}	
}

bool BinaryMemoryInputArchive::SerializeByte(Uint8* pData,UINT nSizeToRead,const char* pszLable)
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



void BinaryMemoryInputArchive::SkipByte(UINT nSizeToSkip)
{
	if (m_nCurPos + nSizeToSkip <= m_nDataSize)
	{
		m_nCurPos += nSizeToSkip;
	}else{
		Log("Fail to skip %d bytes at position %d : out of file boundary\n",nSizeToSkip,m_nCurPos);
	}
	
}

bool BinaryMemoryInputArchive::Open(const Uint8* pData,Uint32 nSizeInByte)
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
