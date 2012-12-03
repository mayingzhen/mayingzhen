//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2010/3/3
// Describe:S3 Animation
//----------------------------------------------------------------------

#ifndef __S3AXMLInputArchive_H__
#define __S3AXMLInputArchive_H__

#include <Animation/Common/Serialize/S3ASerializeListener.h>

class TiXmlDocument;
class TiXmlElement;

#include <Animation/Common/xmCommonPrefix.h>

class  S3AXMLInputArchive : public S3ASerializeListener , public IS3AXMLInputArchive
{
	std::string		m_strFilename;
	TiXmlDocument*	m_pDoc;
	TiXmlElement*	m_pCurElem;
	xmInt				m_nullChild;

public:
	S3AXMLInputArchive();

	~S3AXMLInputArchive();

	void Release();

	bool IsReading() const;

	bool Open(const char* pszFilename);

	bool OpenFromMemory(const char* pszData);

	void Close();

	TiXmlDocument* GetXMLDocument();

	void BeginSection(const char* pszLable);

	void EndSection();

	void SerializeBasicType(bool& val,const char* pszLable = "bool");

	void SerializeBasicType(unsigned char& val,const char* pszLable);

	void SerializeBasicType(short& val,const char* pszLable);

	void SerializeBasicType(unsigned short& val, const char* pszLabel);

	void SerializeBasicType(unsigned int&val,const char* pszLable = "unsigned int");

	void SerializeBasicType(int&val,const char* pszLable = "int");

	void SerializeBasicType(unsigned long &val,const char* pszLable = "unsigned long");

	void SerializeBasicType(long &val,const char* pszLable = "long");


	void SerializeBasicType(xmUint64&val,const char* pszLable = "xmUint64");

	void SerializeBasicType(float& val,const char* pszLable = "float");

	void SerializeBasicType(std::string& val,const char* pszLable = "string");

	void SerializeBasicType(IS3AStringWrapper* val,const char* pszLable);


	IS3ASerializeListener* GetSerializeListener();

private:
	void NextSiblingElement();
};

#include <Animation/Common/xmCommonSuffix.h>


#endif// __S3AXMLInputArchive_H__