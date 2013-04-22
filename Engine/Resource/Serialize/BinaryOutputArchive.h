#ifndef __BinaryOutputArchive_H__
#define __BinaryOutputArchive_H__

#include "Engine/Resource/Serialize/SerializeListener.h"

namespace ma
{



class ENGINE_API BinaryOutputArchive : public SerializeListener
{
	std::ofstream m_file;

public:

	BinaryOutputArchive();

	~BinaryOutputArchive();

// 	bool IsReading() const;
// 
// 	bool Open(const char* pszFilename);
// 
// 	void Close();
// 
// 	void Serialize(bool& val,const char* pszLable = "bool");
// 
// 	void Serialize(short& val,const char* pszLable);
// 
// 	void Serialize(unsigned short& val, const char* pszLabel);
// 
// 	void Serialize(unsigned int&val,const char* pszLable = "unsigned int");
// 
// 	void Serialize(int&val,const char* pszLable = "int");
// 
// 	void Serialize(unsigned long &val,const char* pszLable = "unsigned long");
// 
// 	void Serialize(long &val,const char* pszLable = "long");
// 
// 	void Serialize(Uint64&val,const char* pszLable = "Uint64");
// 
// 	void Serialize(float& val,const char* pszLable = "float");
// 
// 	void Serialize(std::string& val,const char* pszLable = "string");

	//void Serialize(IStringWrapper* val,const char* pszLable = "string");

	bool IsReading() const;

	UINT Tell();

	void Seek(UINT nPos);

	bool SerializeByte(Uint8* pData,UINT nSizeToRead,const char* pszLable = "Bytes");

	void SkipByte(UINT nSizeToSkip);

	bool Open(const char* pszFilename);

	void Close();



};

}

#endif// __BinaryOutputArchive_H__