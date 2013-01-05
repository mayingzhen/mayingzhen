//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2010/3/3
// Describe:S3 Animation
//----------------------------------------------------------------------

#ifndef __S3AXMLOutputArchive_H__
#define __S3AXMLOutputArchive_H__

#include <Animation/Common/Serialize/SerializeListener.h>

class TiXmlDocument;
class TiXmlElement;
class IS3AAllocator;

#include <Animation/Common/xmCommonPrefix.h>

class COMMON_API S3AXMLOutputArchive : public SerializeListener , public IS3AXMLOutputArchive
{
	std::string		m_strFilename;
	TiXmlDocument*	m_pDoc;
	TiXmlElement*	m_pParentElem;

public:
	S3AXMLOutputArchive();

	~S3AXMLOutputArchive();

	void Release();

	bool IsReading() const;

	bool Open(const char* pszFilename);

	void Close();

	bool OpenToMemory();

	bool SaveToMemory(char** ppData,xmUint* pDataSize,IS3AAllocator* pAlloc);

	void BeginSection(const char* pszLable);

	void EndSection();

	void SerializeBasicType(bool& val,const char* pszLable = "bool");

	void SerializeBasicType(unsigned char& val, const char* pszLabel = "unsigned char");

	void SerializeBasicType(char& val,const char* pszLable);

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

	SerializeListener* GetSerializeListener();

};

#include <Animation/Common/xmCommonSuffix.h>

#endif// __S3AXMLOutputArchive_H__