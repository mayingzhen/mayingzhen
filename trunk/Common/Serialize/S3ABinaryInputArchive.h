#ifndef __S3ABinaryInputArchive_H__
#define __S3ABinaryInputArchive_H__

#include <Common/Serialize/S3ASerializeListener.h>

//#include <Common/xmCommonPrefix.h>

class COMMON_API S3ABinaryInputArchive : public S3ASerializeListener
{
	std::ifstream m_file;
public:

	S3ABinaryInputArchive();

	~S3ABinaryInputArchive();

	
	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	bool IsReading() const;

	xmUint Tell();

	void Seek(xmUint nPos);

	bool SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable = "Bytes");

	void SkipByte(xmUint nSizeToSkip);

	bool Open(const char* pszFilename);

	void Close();

	

};

class COMMON_API S3ABinaryMemoryInputArchive : public S3ASerializeListener
{
	const xmUint8* m_pData;
	xmUint32 m_nDataSize;
	xmUint32 m_nCurPos;

public:

	S3ABinaryMemoryInputArchive();

	~S3ABinaryMemoryInputArchive();

	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	bool IsReading()const;

	xmUint Tell();

	void Seek(xmUint nPos);

	bool SerializeByte(xmUint8* pData,xmUint nSizeToRead,const char* pszLable = "Bytes");

	void SkipByte(xmUint nSizeToSkip);

	bool Open(const xmUint8* pData,xmUint32 nSizeInByte);

	void Close();


};


//#include <Animation/Common/xmCommonSuffix.h>

#endif// __S3ABinaryInputArchive_H__