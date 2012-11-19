//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2011/1/25
// Describe: S3 Animation
//----------------------------------------------------------------------

#ifndef __S3AArchive_H__
#define __S3AArchive_H__

class IS3AStringWrapper
{
public:

	virtual void		SetValue(const char* pszString) = 0;

	virtual const char* GetValue() const = 0;

	virtual xmUint		GetSize() const = 0;
	
	virtual void		Resize(xmUint nNewSize,char val = '0') = 0;
};

class COMMON_API IS3ASerializeListener
{
public:

	virtual void SerializeBasicType(bool& val,const char* pszLable = "bool" ) = 0;

	virtual void SerializeBasicType(unsigned char& val, const char* pszLabel = "unsigned char") = 0;

	virtual void SerializeBasicType(short& val,const char* pszLable = "short") = 0;	

	virtual void SerializeBasicType(unsigned short& val, const char* pszLabel = "unsigned short") = 0;

	virtual void SerializeBasicType(unsigned int&val,const char* pszLable = "unsigned int") = 0;

	virtual void SerializeBasicType(int&val,const char* pszLable = "int") = 0;

	virtual void SerializeBasicType(unsigned long &val,const char* pszLable = "unsigned long") = 0;

	virtual void SerializeBasicType(long &val,const char* pszLable = "long") = 0;

	virtual void SerializeBasicType(xmUint64&val,const char* pszLable = "xmUint64") = 0;

	virtual void SerializeBasicType(float& val,const char* pszLable = "float") = 0;

	virtual void SerializeBasicType(std::string& val,const char* pszLable = "string") = 0;

	virtual void SerializeBasicType(IS3AStringWrapper* val,const char* pszLable = "string") = 0;

	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------

	virtual void BeginSection(const char* pszLable) = 0;

	virtual void EndSection() = 0;

	//Version for current module
	virtual xmUint GetVersion() const = 0;

	virtual void PushVersion(xmUint nVersion) = 0;

	virtual xmUint PopVersion() = 0;

};

class IS3AXMLInputArchive
{
public:
	virtual void Release() = 0;
	
	virtual bool Open(const char* pszFilename) = 0;

	virtual void Close() = 0;

	virtual IS3ASerializeListener* GetSerializeListener() = 0;
};

class IS3AXMLOutputArchive
{
public:

	virtual void Release() = 0;

	virtual bool Open(const char* pszFilename) = 0;

	virtual void Close() = 0;

	virtual IS3ASerializeListener* GetSerializeListener() = 0;


};
#endif// __S3AArchive_H__