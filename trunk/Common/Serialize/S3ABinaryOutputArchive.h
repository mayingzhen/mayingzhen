//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2010/3/3
// Describe:S3 Animation
//----------------------------------------------------------------------

#ifndef __S3ABinaryOutputArchive_H__
#define __S3ABinaryOutputArchive_H__

#include <Common/Serialize/S3ASerializeListener.h>

//#include <Animation/Common/xmCommonPrefix.h>

class COMMON_API S3ABinaryOutputArchive : public S3ASerializeListener
{
	std::ofstream m_file;

public:

	S3ABinaryOutputArchive();

	~S3ABinaryOutputArchive();

	bool IsReading() const;

	bool Open(const char* pszFilename);

	void Close();

	void SerializeBasicType(bool& val,const char* pszLable = "bool");

	void SerializeBasicType(short& val,const char* pszLable);

	void SerializeBasicType(unsigned short& val, const char* pszLabel);

	void SerializeBasicType(unsigned int&val,const char* pszLable = "unsigned int");

	void SerializeBasicType(int&val,const char* pszLable = "int");

	void SerializeBasicType(unsigned long &val,const char* pszLable = "unsigned long");

	void SerializeBasicType(long &val,const char* pszLable = "long");

	void SerializeBasicType(xmUint64&val,const char* pszLable = "xmUint64");

	void SerializeBasicType(float& val,const char* pszLable = "float");

	void SerializeBasicType(std::string& val,const char* pszLable = "string");

	void SerializeBasicType(IS3AStringWrapper* val,const char* pszLable = "string");


};


//#include <Animation/Common/xmCommonSuffix.h>
#endif// __S3ABinaryOutputArchive_H__