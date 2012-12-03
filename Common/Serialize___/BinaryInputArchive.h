#ifndef __BinaryInputArchive_H__
#define __BinaryInputArchive_H__

#include <Common/Serialize/SerializeListener.h>

//#include <Common/xmCommonPrefix.h>

class COMMON_API BinaryInputArchive : public SerializeListener
{
	std::ifstream m_file;
public:

	BinaryInputArchive();

	~BinaryInputArchive();

	
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

class COMMON_API BinaryMemoryInputArchive : public SerializeListener
{
	const xmUint8* m_pData;
	xmUint32 m_nDataSize;
	xmUint32 m_nCurPos;

public:

	BinaryMemoryInputArchive();

	~BinaryMemoryInputArchive();

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

#endif// __BinaryInputArchive_H__