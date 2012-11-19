//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2010/3/3
// Describe:S3 Animation
//----------------------------------------------------------------------

#include <Common/Serialize/S3ABinaryOutputArchive.h>
//#include <Animation/Common/String/S3AStringHelper.h>

S3ABinaryOutputArchive::S3ABinaryOutputArchive()
{
}


S3ABinaryOutputArchive::~S3ABinaryOutputArchive()
{
}

bool S3ABinaryOutputArchive::IsReading() const
{
	return false;
}

bool S3ABinaryOutputArchive::Open(const char* pszFilename)
{
#if S3_PLATFORM_WIN == 1
	std::wstring filename = S3AStringHelper::ToWideChar(pszFilename);
	m_file.open(filename.c_str(), std::ios::out | std::ios::binary);
#else
    m_file.open(pszFilename, std::ios::out | std::ios::binary);
    
#endif

	S3ASSERT_MSG( ! m_file.fail(), "fail to open file" );

	return !m_file.fail();

}


void S3ABinaryOutputArchive::Close()
{
	m_file.close();
}


void S3ABinaryOutputArchive::SerializeBasicType(bool& val,const char* pszLable)
{
	m_file.write(reinterpret_cast<const char*>(&val),sizeof(bool));
}

void S3ABinaryOutputArchive::SerializeBasicType(short& val,const char* pszLable)
{
	m_file.write(reinterpret_cast<const char*>(&val),sizeof(short));
}

void S3ABinaryOutputArchive::SerializeBasicType(unsigned short& val, const char* pszLabel)
{
	m_file.write(reinterpret_cast<const char*>(&val),sizeof(unsigned short));
}

void S3ABinaryOutputArchive::SerializeBasicType(unsigned int&val,const char* pszLable)
{
	m_file.write(reinterpret_cast<const char*>(&val),sizeof(unsigned int));
}



void S3ABinaryOutputArchive::SerializeBasicType(int&val,const char* pszLable)
{
	m_file.write(reinterpret_cast<const char*>(&val),sizeof(int));
}

void S3ABinaryOutputArchive::SerializeBasicType(unsigned long &val,const char* pszLable)
{
	m_file.write(reinterpret_cast<char*>(&val),sizeof(unsigned long));
}

void S3ABinaryOutputArchive::SerializeBasicType(long &val,const char* pszLable)
{
	m_file.write(reinterpret_cast<char*>(&val),sizeof(long));
}

void S3ABinaryOutputArchive::SerializeBasicType(xmUint64&val,const char* pszLable)
{
	m_file.write(reinterpret_cast<const char*>(&val),sizeof(xmUint64));
}


void S3ABinaryOutputArchive::SerializeBasicType(float& val,const char* pszLable)
{
	m_file.write(reinterpret_cast<const char*>(&val),sizeof(float));
}


void S3ABinaryOutputArchive::SerializeBasicType(std::string& val,const char* pszLable)
{
	xmUint nSize = (xmUint) val.size() + 1;
	m_file.write(reinterpret_cast<const char*>(&nSize),sizeof(xmUint));
	m_file.write(val.c_str(),sizeof(char)*nSize);
}

void S3ABinaryOutputArchive::SerializeBasicType(IS3AStringWrapper* val,const char* pszLable)
{
	xmUint nSize = (xmUint) val->GetSize() + 1;
	m_file.write(reinterpret_cast<const char*>(&nSize),sizeof(xmUint));
	m_file.write(val->GetValue(),sizeof(char)*nSize);
}
