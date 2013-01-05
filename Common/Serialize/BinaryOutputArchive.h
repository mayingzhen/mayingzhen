#ifndef __BinaryOutputArchive_H__
#define __BinaryOutputArchive_H__

#include <common/Serialize/SerializeListener.h>

//#include <Animation/Common/xmCommonPrefix.h>

namespace ma
{



class COMMON_API BinaryOutputArchive : public SerializeListener
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
// 	void Serialize(xmUint64&val,const char* pszLable = "xmUint64");
// 
// 	void Serialize(float& val,const char* pszLable = "float");
// 
// 	void Serialize(std::string& val,const char* pszLable = "string");

	//void Serialize(IStringWrapper* val,const char* pszLable = "string");

	bool IsReading() const;

	xmUint Tell();

	void Seek(xmUint nPos);

	bool SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable = "Bytes");

	void SkipByte(xmUint nSizeToSkip);

	bool Open(const char* pszFilename);

	void Close();



};

}

//#include <Animation/Common/xmCommonSuffix.h>
#endif// __BinaryOutputArchive_H__