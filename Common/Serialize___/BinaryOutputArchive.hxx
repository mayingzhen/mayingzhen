#include <Common/Serialize/BinaryOutputArchive.h>
//#include <Animation/Common/String/StringHelper.h>

BinaryOutputArchive::BinaryOutputArchive()
{
}


BinaryOutputArchive::~BinaryOutputArchive()
{
}

bool BinaryOutputArchive::IsReading() const
{
	return true;
}
// 
// bool BinaryOutputArchive::Open(const char* pszFilename)
// {
// 	m_file.open(pszFilename, std::ios::out | std::ios::binary);
// 
// 	SSERT_MSG( ! m_file.fail(), "fail to open file" );
// 
// 	return !m_file.fail();
// 
// }
// 
// 
// void BinaryOutputArchive::Close()
// {
// 	m_file.close();
// }
// 
// 
// void BinaryOutputArchive::Serialize(bool& val,const char* pszLable)
// {
// 	m_file.write(reinterpret_cast<const char*>(&val),sizeof(bool));
// }
// 
// void BinaryOutputArchive::Serialize(short& val,const char* pszLable)
// {
// 	m_file.write(reinterpret_cast<const char*>(&val),sizeof(short));
// }
// 
// void BinaryOutputArchive::Serialize(unsigned short& val, const char* pszLabel)
// {
// 	m_file.write(reinterpret_cast<const char*>(&val),sizeof(unsigned short));
// }
// 
// void BinaryOutputArchive::Serialize(unsigned int&val,const char* pszLable)
// {
// 	m_file.write(reinterpret_cast<const char*>(&val),sizeof(unsigned int));
// }
// 
// 
// 
// void BinaryOutputArchive::Serialize(int&val,const char* pszLable)
// {
// 	m_file.write(reinterpret_cast<const char*>(&val),sizeof(int));
// }
// 
// void BinaryOutputArchive::Serialize(unsigned long &val,const char* pszLable)
// {
// 	m_file.write(reinterpret_cast<char*>(&val),sizeof(unsigned long));
// }
// 
// void BinaryOutputArchive::Serialize(long &val,const char* pszLable)
// {
// 	m_file.write(reinterpret_cast<char*>(&val),sizeof(long));
// }
// 
// void BinaryOutputArchive::Serialize(xmUint64&val,const char* pszLable)
// {
// 	m_file.write(reinterpret_cast<const char*>(&val),sizeof(xmUint64));
// }
// 
// 
// void BinaryOutputArchive::Serialize(float& val,const char* pszLable)
// {
// 	m_file.write(reinterpret_cast<const char*>(&val),sizeof(float));
// }
// 
// 
// void BinaryOutputArchive::Serialize(std::string& val,const char* pszLable)
// {
// 	xmUint nSize = (xmUint) val.size() + 1;
// 	m_file.write(reinterpret_cast<const char*>(&nSize),sizeof(xmUint));
// 	m_file.write(val.c_str(),sizeof(char)*nSize);
// }
// 
// bool BinaryOutputArchive::SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable = "Bytes")
// {
// 	m_file.write()
// }

// void BinaryOutputArchive::Serialize(IStringWrapper* val,const char* pszLable)
// {
// 	xmUint nSize = (xmUint) val->GetSize() + 1;
// 	m_file.write(reinterpret_cast<const char*>(&nSize),sizeof(xmUint));
// 	m_file.write(val->GetValue(),sizeof(char)*nSize);
// }

xmUint BinaryOutputArchive::Tell()
{
	if (m_file)
	{
		return (xmUint) m_file.tellp();
	}else{
		return 0;
	}

}

void BinaryOutputArchive::Seek(xmUint nPos)
{
	if (m_file)
	{
		m_file.seekp(nPos,std::ios::beg);
	}

}

bool BinaryOutputArchive::SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable)
{
	m_file.write((char*)pData,nSizeToRead);
	return ! m_file.fail();
}



void BinaryOutputArchive::SkipByte(xmUint nSizeToSkip)
{
	m_file.seekp(nSizeToSkip,std::ios_base::cur);
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
